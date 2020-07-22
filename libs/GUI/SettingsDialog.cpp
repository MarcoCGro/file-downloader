#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    initialize();

    this->networkRequester = nullptr;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setNetworkRequester(NetworkRequester *networkRequester)
{
    this->networkRequester = networkRequester;
}

void SettingsDialog::initialize()
{
    this->resize(this->width(), 150);
    ui->directoryButton->setIcon(ui->actionDirectory->icon());
    ui->syncButton->setIcon(ui->actionSync->icon());

    this->currentFilesUrl = "https://altomobile.blob.core.windows.net/api/files.json";
    ui->filesUrlEdit->setText(this->currentFilesUrl);
    this->currentDownloadsDirectory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    ui->directoryEdit->setText(this->currentDownloadsDirectory);

    this->fileDialog.setDirectory(this->currentDownloadsDirectory);
    this->fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    this->fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
}

bool SettingsDialog::validateFolder(QString chosenFolder)
{
    bool valid = false;

    QFileInfo currDir(chosenFolder);
    if (currDir.exists()) {
        if (currDir.permission(QFile::WriteUser)) {
            ui->directoryEdit->setText(chosenFolder);
            valid = true;
        }
        else {
            showMessage("You aren't allowed to write on selected option. Try another folder.");
        }
    }
    else {
        showMessage("Directory selected doesn't exist. Try another folder.");
    }

    return valid;
}

void SettingsDialog::on_syncButton_clicked()
{
    if (this->networkRequester == nullptr)
        return;

    this->networkRequester->validateEndpoint(ui->filesUrlEdit->text());
    if (this->networkRequester->isValidEndpoint())
        showMessage(this->networkRequester->getCurrentMessage(), QMessageBox::Information);
    else
        showMessage(this->networkRequester->getCurrentMessage(), QMessageBox::Critical);
}

void SettingsDialog::on_directoryButton_clicked()
{
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
    qt_ntfs_permission_lookup++; // turn checking on

    this->fileDialog.exec();

    if (this->fileDialog.result() == QDialog::Accepted) {
        QString chosenFolder = this->fileDialog.directory().path();
        validateFolder(chosenFolder);
    }

    qt_ntfs_permission_lookup--; // turn checking off
}

void SettingsDialog::accept()
{
    QString chosenFolder = ui->directoryEdit->text();
    bool valid = validateFolder(chosenFolder);
    if (valid) {
        this->currentFilesUrl = ui->filesUrlEdit->text();
        this->currentDownloadsDirectory = ui->directoryEdit->text();

        this->done(0);
    }
}

void SettingsDialog::reject()
{
    ui->filesUrlEdit->setText(this->currentFilesUrl);
    ui->directoryEdit->setText(this->currentDownloadsDirectory);

    this->done(0);
}

void SettingsDialog::showMessage(QString message, QMessageBox::Icon msgType)
{
    this->messageBox.setIcon(msgType);
    this->messageBox.setText(message);
    this->messageBox.show();
}
