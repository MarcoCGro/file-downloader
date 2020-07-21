#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    initialize();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::initialize()
{
    this->resize(this->width(), 150);
    ui->directoryButton->setIcon(ui->actionDirectory->icon());
    ui->syncButton->setIcon(ui->actionSync->icon());

    this->currentFilesUri = this->FilesEndpointUri;
    ui->filesUriEdit->setText(this->currentFilesUri);
    this->currentDownloadsDirectory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    ui->directoryEdit->setText(this->currentDownloadsDirectory);

    this->fileDialog.setDirectory(this->currentDownloadsDirectory);
    this->fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    this->fileDialog.setOption(QFileDialog::ShowDirsOnly, true);

    this->messageBox.setIcon(QMessageBox::Warning);
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
            this->messageBox.setText("You aren't allowed to write on selected option. Try another folder.");
            this->messageBox.show();
        }
    }
    else {
        this->messageBox.setText("Directory selected doesn't exist. Try another folder.");
        this->messageBox.show();
    }

    return valid;
}

void SettingsDialog::on_syncButton_clicked()
{
    qDebug("sync \n");
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
        this->currentFilesUri = ui->filesUriEdit->text();
        this->currentDownloadsDirectory = ui->directoryEdit->text();

        this->done(0);
    }
}

void SettingsDialog::reject()
{
    ui->filesUriEdit->setText(this->currentFilesUri);
    ui->directoryEdit->setText(this->currentDownloadsDirectory);

    this->done(0);
}
