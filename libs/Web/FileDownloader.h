#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include "NetworkManager.h"
#include "libs/Model/DownloadDetails.h"

/**
 * @brief The FileDownloader class
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 */
class FileDownloader : public NetworkManager
{
    Q_OBJECT

public:
    FileDownloader(QObject *parent = nullptr);
    ~FileDownloader();

    void startDownload(DownloadDetails *downloadDetails);
    void recoverDownload(DownloadDetails *downloadDetails);
    void pauseDownload();
    void resumeDownload();

signals:
    void updateProgress(int bytesReceived);
    void recoverProgress();
    void finished();

protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();

    void reportError(QNetworkReply::NetworkError error);

protected:
    QString getNextAvailableName(QString origFilename);

    DownloadDetails *currentDownloadDetails;
    QNetworkReply::NetworkError currentError;
    bool acceptRanges;
    QFile *file;
};

#endif // FILEDOWNLOADER_H
