#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "libs/Web/EndpointRequester.h"

namespace Ui {
class SettingsDialog;
}

/**
 * @brief The SettingsDialog class contains two fields (download URL and
 * downloads directory) to configure and save the files to be requested.
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 *
 * @see MainWindow, EndpointRequester
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default downloads url provided
     */
    const QString DEFAULT_DOWNLOADS_URL = "https://altomobile.blob.core.windows.net/api/files.json";

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    /**
     * @brief Sets the requester to obtain the files details to download
     *
     * @param endpointRequester Requester instance
     */
    void setEndpointRequester(EndpointRequester *endpointRequester);

    QString getFilesUrl() const;
    void setFilesUrl(const QString &value);

    QString getDownloadsDirectory() const;
    void setDownloadsDirectory(const QString &value);

protected:
    void initialize();

    bool validateFolder(QString chosenFolder);
    bool validateUrl(QString url);

    QString currentFilesUrl;
    QString currentDownloadsDirectory;

private slots:
    void on_syncButton_clicked();
    void on_directoryButton_clicked();

    virtual void accept();
    virtual void reject();

protected:
    EndpointRequester *endpointRequester;

private:
    void showMessage(QString message, QMessageBox::Icon msgType = QMessageBox::Warning);

    QFileDialog fileDialog;
    QMessageBox messageBox;

    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
