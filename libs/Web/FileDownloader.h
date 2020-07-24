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

protected slots:
    void downloadReadyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();

protected:
    DownloadDetails *currentDownloadDetails;
    QFile output;
};

#endif // FILEDOWNLOADER_H
