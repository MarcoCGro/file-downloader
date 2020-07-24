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

    void setValues(DownloadDetails *downloadDetails);
    QJsonObject getValuesAsJson();

    void recoverDownload();
    void releaseDownload();

protected:
    void initialize();

    void startDownload();
    void pauseDownload();
    void resumeDownload();

    FileDownloader *fileDownloader;
    DownloadDetails *downloadDetails;

protected slots:
    void on_stateButton_pressed();

    void updateProgress(int bytesReceived);
    void finishDownload();

    void recoverDownloadMessage();

private:
    QString getBytesLabel(double numBytes);

    Ui::DownloadDetailsWidget *ui;
};

#endif // DOWNLOADDETAILSWIDGET_H
