#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QList>

#include "SettingsDialog.h"
#include "DownloadDetailsWidget.h"

#include "libs/Web/EndpointRequester.h"
#include "libs/Model/DownloadDetails.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const QList<QString> JSON_EXTENDED_FIELDS = {"FileName", "Length", "BlobType", "DownloadURI", "State", "AcceptRanges", "NumReceivedBytes"};
    const QString preferencesFilename = "settings.json";

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initialize();

    void loadCurrentDownloads();
    void setDownloads(QJsonArray jsonArray);
    void addElementToDownload(DownloadDetails *downloadDetails);

    void recoverCurrentDownloads();

    virtual void closeEvent(QCloseEvent *event);
    void saveCurrentDownloads();

    void showMessage(QString message, QMessageBox::Icon msgType);

    QList<DownloadDetailsWidget*> *downloadsWidgets;

    SettingsDialog *settingsDialog;
    EndpointRequester *endpointRequester;
    JsonValidator *jsonValidator;

    QMessageBox messageBox;

protected slots:
    void on_actionRequest_triggered();
    void on_actionSettings_triggered();

    void removeDownloadWidget(int widgetId);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
