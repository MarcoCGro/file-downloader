#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    const QString FilesEndpointUri = "https://altomobile.blob.core.windows.net/api/files.json";

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

protected:
    void initialize();
    bool validateFolder(QString chosenFolder);

    QString currentFilesUri;
    QString currentDownloadsDirectory;

private slots:
    void on_syncButton_clicked();
    void on_directoryButton_clicked();

    virtual void accept();
    virtual void reject();

private:
    Ui::SettingsDialog *ui;

    QFileDialog fileDialog;
    QMessageBox messageBox;
};

#endif // SETTINGSDIALOG_H
