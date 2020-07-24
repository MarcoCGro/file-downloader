#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    initialize();

    this->endpointRequester = nullptr;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setEndpointRequester(EndpointRequester *endpointRequester)
{
    this->endpointRequester = endpointRequester;
}

QString SettingsDialog::getFilesUrl() const
{
    return this->currentFilesUrl;
}

void SettingsDialog::setFilesUrl(const QString &value)
{
    this->currentFilesUrl = value;
}

QString SettingsDialog::getDownloadsDirectory() const
{
    return this->currentDownloadsDirectory;
}

void SettingsDialog::setDownloadsDirectory(const QString &value)
{
    this->currentDownloadsDirectory = value;
}

void SettingsDialog::initialize()
{
    this->resize(this->width(), 150);
    ui->directoryButton->setIcon(ui->actionDirectory->icon());
    ui->syncButton->setIcon(ui->actionSync->icon());

    this->currentFilesUrl = DEFAULT_DOWNLOADS_URL;
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
        if (currDir.permission(QFile::WriteUser))
            valid = true;
        else
            showMessage("You aren't allowed to write on selected option. Try another folder.");
    }
    else {
        showMessage("Directory selected doesn't exist. Try another folder.");
    }

    return valid;
}

bool SettingsDialog::validateUrl(QString url)
{
    this->endpointRequester->validateRequest(url);

    bool result = this->endpointRequester->isValidRequest();
    if (!result)
        showMessage(this->endpointRequester->getCurrentMessage(), QMessageBox::Critical);

    return result;
}

void SettingsDialog::on_syncButton_clicked()
{
    if (this->endpointRequester == nullptr)
        return;

    bool valid = validateUrl(ui->filesUrlEdit->text());
    if (valid)
        showMessage(this->endpointRequester->getCurrentMessage(), QMessageBox::Information);
}

void SettingsDialog::on_directoryButton_clicked()
{
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
    qt_ntfs_permission_lookup++; // turn checking on

    this->fileDialog.exec();

    if (this->fileDialog.result() == QDialog::Accepted) {
        QString chosenFolder = this->fileDialog.directory().path();
        bool valid = validateFolder(chosenFolder);
        if (valid)
            ui->directoryEdit->setText(chosenFolder);
    }

    qt_ntfs_permission_lookup--; // turn checking off
}

void SettingsDialog::accept()
{
    bool valid = validateFolder(ui->directoryEdit->text());
    if (valid) {
        valid = validateUrl(ui->filesUrlEdit->text());
        if (valid) {
            this->currentFilesUrl = ui->filesUrlEdit->text();
            this->currentDownloadsDirectory = ui->directoryEdit->text();

            this->done(0);
        }
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
