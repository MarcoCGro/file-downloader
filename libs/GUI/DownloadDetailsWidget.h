#ifndef DOWNLOADDETAILSWIDGET_H
#define DOWNLOADDETAILSWIDGET_H

#include <QWidget>

#include "libs/Web/FileDownloader.h"
#include "libs/Model/DownloadDetails.h"

namespace Ui {
class DownloadDetailsWidget;
}

class DownloadDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadDetailsWidget(QWidget *parent = nullptr);
    ~DownloadDetailsWidget();

    void setValues(int widgetId, DownloadDetails *downloadDetails);
    void updateId(int id);

    QJsonObject getValuesAsJson();

    void recoverDownload();
    void releaseDownload();

signals:
    void removeWidget(int id);

protected:
    void initialize();

    void startDownload();
    void pauseDownload();
    void resumeDownload();
    void openDownload();

    FileDownloader *fileDownloader;
    DownloadDetails *downloadDetails;

    int widgetId;

protected slots:
    void on_removeButton_clicked();
    void on_stateButton_pressed();

    void updateProgress(int bytesReceived);
    void finishDownload();

    void recoverDownloadMessage();

private:
    QString getBytesLabel(double numBytes);

    Ui::DownloadDetailsWidget *ui;
};

#endif // DOWNLOADDETAILSWIDGET_H
