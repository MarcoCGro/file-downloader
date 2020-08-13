#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include "NetworkManager.h"
#include "libs/Model/DownloadDetails.h"

/**
 * @brief The FileDownloader class is subclass of NetworkRequester class
 * which is used to manage and request the download of a file
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 *
 * @see NetworkRequester
 */
class FileDownloader : public NetworkManager
{
    Q_OBJECT

public:
    FileDownloader(QObject *parent = nullptr);
    ~FileDownloader();

    /**
     * @brief Gets ready and starts the corresponding download
     * described by the object provided
     *
     * @param downloadDetails Details of the file to be downloaded
     */
    void startDownload(DownloadDetails *downloadDetails);
    /**
     * @brief Sets the details of a previous download, after user
     * closed the main application, to resume it eventually
     *
     * @param downloadDetails Details of the download to resume
     */
    void recoverDownload(DownloadDetails *downloadDetails);
    /**
     * @brief Pauses the current download associated
     */
    void pauseDownload();
    /**
     * @brief Resumes the current download associated
     */
    void resumeDownload();

signals:
    void updateProgress(double bytesReceived);
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
