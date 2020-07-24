#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include "NetworkManager.h"
#include "libs/Model/DownloadDetails.h"

class FileDownloader : public NetworkManager
{
    Q_OBJECT

public:
    FileDownloader(QObject *parent = nullptr);
    ~FileDownloader();

    void startDownload(DownloadDetails *downloadDetails);
    void pauseDownload();
    void resumeDownload();

signals:
    void updateProgress(int bytesReceived);
    void recoverDownload();
    void finished();

protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();

protected:
    DownloadDetails *currentDownloadDetails;
    bool acceptRanges;
    QFile *file;
};

#endif // FILEDOWNLOADER_H
