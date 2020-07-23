#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "libs/Web/NetworkRequester.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void setNetworkRequester(NetworkRequester *networkRequester);

    QString getFilesUrl() const;
    QString getDownloadsDirectory() const;

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
    NetworkRequester *networkRequester;

private:
    void showMessage(QString message, QMessageBox::Icon msgType = QMessageBox::Warning);

    QFileDialog fileDialog;
    QMessageBox messageBox;

    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
