#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject *parent)
    : NetworkManager(parent)
{
    this->currentDownloadDetails = nullptr;
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::startDownload(DownloadDetails *downloadDetails)
{
    this->currentDownloadDetails = downloadDetails;

    QString filename = this->currentDownloadDetails->getOutputFilename();
    qDebug(" filename: %s ", filename.toStdString().data());

    this->output.setFileName(filename);
    if (!this->output.open(QIODevice::WriteOnly)) {
        this->validRequest = false;
        this->currentMessage = "Problem opening save file for download [ " + this->currentDownloadDetails->getDownloadURI() + " ]";
        return;
    }

    this->request.setUrl(QUrl(this->currentDownloadDetails->getDownloadURI()));
    this->reply = this->manager->get(request);

    connect(this->reply, &QIODevice::readyRead, this, &FileDownloader::downloadReadyRead);
    connect(this->reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(this->reply, &QNetworkReply::finished, this, &FileDownloader::downloadFinished);
}

void FileDownloader::downloadReadyRead()
{
    QByteArray byteArray = this->reply->readAll();
    this->output.write(byteArray);
}

void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug(" bytesReceived: %ld %ld ", bytesReceived, bytesTotal);
}

void FileDownloader::downloadFinished()
{
    qDebug(" NetworkRequester::downloadFinished() ");

    bool success = this->output.flush();
    if (success)
        this->output.close();
}
