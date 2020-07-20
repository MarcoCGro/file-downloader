#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "DownloadDetailsWidget.h"

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
    void addElementToDownload(QString filename, QString blobType, QString fileSize, QString downloadURI);

    QList<DownloadDetailsWidget*> *downloadsDetails;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
