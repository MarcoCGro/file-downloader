#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initialize();
    void addElementToDownload(DownloadDetails *downloadDetails);

    QList<DownloadDetailsWidget*> *downloadsWidgets;

    SettingsDialog *settingsDialog;
    EndpointRequester *endpointRequester;

private slots:
    void on_actionRequest_triggered();
    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
