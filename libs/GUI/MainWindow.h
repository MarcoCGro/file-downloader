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

/**
 * @brief The MainWindow class allows to manage a downloads list through
 * a table view. Also, a tools bar contains options to request/build this
 * list and invoke user setttings.
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 *
 * @see DownloadDetailsWidget, SettingsDialog
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Name of the fields labels contained in a DownloadDetails object
     */
    const QList<QString> JSON_EXTENDED_FIELDS = {"FileName", "Length", "BlobType", "DownloadURI", "State", "AcceptRanges", "NumReceivedBytes", "OutputFilename"};
    /**
     * @brief Name of the file which will save the user settings and the list
     * of current downloads shown
     */
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
