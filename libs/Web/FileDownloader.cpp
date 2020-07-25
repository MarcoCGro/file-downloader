#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject *parent)
    : NetworkManager(parent)
{
    this->currentDownloadDetails = nullptr;
    this->currentError = QNetworkReply::NoError;
    this->file = nullptr;
    this->acceptRanges = false;
}

FileDownloader::~FileDownloader()
{
    if (this->file != nullptr)
        this->file = nullptr;
}

void FileDownloader::startDownload(DownloadDetails *downloadDetails)
{
    this->currentDownloadDetails = downloadDetails;

    QString tmpFilename = this->currentDownloadDetails->getOutputFilename();
    this->file = new QFile(tmpFilename);
    if (QFileInfo::exists(tmpFilename)) {
        QFile::remove(tmpFilename);
        this->file->remove();

        QString auxFilename = getNextAvailableName(tmpFilename);
        this->currentDownloadDetails->setFinalFilename(auxFilename);
        this->file = new QFile(auxFilename);
    }

    if (!this->file->open(QIODevice::ReadWrite | QIODevice::Append)) {
        this->validRequest = false;
        this->currentMessage = "Problem opening save file for download [ " + this->currentDownloadDetails->getDownloadURI() + " ]";
        return;
    }

    this->request = QNetworkRequest(QUrl(this->currentDownloadDetails->getDownloadURI()));
    this->reply = this->manager->get(request);

    if (this->reply->hasRawHeader("Accept-Ranges")) {
        QString qstrAcceptRanges = this->reply->rawHeader("Accept-Ranges");
        this->acceptRanges = qstrAcceptRanges.compare("bytes", Qt::CaseInsensitive) == 0;
        this->currentDownloadDetails->setAcceptRanges(this->acceptRanges);
    }

    connect(this->reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(this->reply, &QNetworkReply::finished, this, &FileDownloader::downloadFinished);
    connect(this->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reportError(QNetworkReply::NetworkError)));
}

void FileDownloader::recoverDownload(DownloadDetails *downloadDetails)
{
    this->currentDownloadDetails = downloadDetails;
    this->acceptRanges = this->currentDownloadDetails->getAcceptRanges();

    QString tmpFilename = this->currentDownloadDetails->getOutputFilename();

    this->file = new QFile(tmpFilename);
    if (!this->acceptRanges) {
        QFile::remove(tmpFilename);
        this->file->remove();

        this->file = new QFile(tmpFilename);
    }

    if (!this->file->open(QIODevice::ReadWrite | QIODevice::Append)) {
        this->validRequest = false;
        this->currentMessage = "Problem opening save file for download [ " + this->currentDownloadDetails->getDownloadURI() + " ]";
        return;
    }

    this->request = QNetworkRequest(QUrl(this->currentDownloadDetails->getDownloadURI()));
}

void FileDownloader::pauseDownload()
{
    disconnect(this->reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    disconnect(this->reply, &QNetworkReply::finished, this, &FileDownloader::downloadFinished);
    disconnect(this->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reportError(QNetworkReply::NetworkError)));

    this->reply->abort();
    this->file->flush();

    QNetworkReply *tmpReply = this->reply;
    tmpReply->deleteLater();

    this->reply = nullptr;
}

void FileDownloader::resumeDownload()
{
    if (this->acceptRanges) {
        qint64 currentBytes = this->currentDownloadDetails->getNumReceivedBytes();
        QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(currentBytes) + "-" + QByteArray::number(this->currentDownloadDetails->getLength());
        qDebug(" Range: %s ", rangeHeaderValue.toStdString().data());
        this->request.setRawHeader("Range", rangeHeaderValue);
    }
    else {
        QString tmpFilename = this->currentDownloadDetails->getOutputFilename();
        this->file->remove();

        this->file = new QFile(tmpFilename);
        if (!this->file->open(QIODevice::ReadWrite | QIODevice::Append)) {
            this->validRequest = false;
            this->currentMessage = "Problem reopening original file for download [ " + this->currentDownloadDetails->getDownloadURI() + " ]";
            return;
        }
    }

    this->reply = this->manager->get(this->request);

    connect(this->reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(this->reply, &QNetworkReply::finished, this, &FileDownloader::downloadFinished);
    connect(this->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reportError(QNetworkReply::NetworkError)));
}

void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesTotal);

    if (!this->acceptRanges && (bytesReceived < this->currentDownloadDetails->getNumReceivedBytes())) {
        emit recoverProgress();
        return;
    }

    QByteArray byteArray = this->reply->readAll();
    this->file->write(byteArray);

    emit updateProgress(int(bytesReceived));
}

void FileDownloader::downloadFinished()
{
    bool success = this->file->flush();
    if (success) {
        QString tmpFilename = this->file->fileName();
        qint64 finalSize = this->file->size();

        this->file->close();
        this->file = nullptr;

        if (this->currentError != QNetworkReply::NoError) {
            this->validRequest = false;
            this->currentMessage = "Selected file can't be downloaded.";
        }
        else if (this->currentDownloadDetails->getLength() == int(finalSize)) {
            QString tmpFilename = this->currentDownloadDetails->getOutputFilename();
            int lastIdx = tmpFilename.lastIndexOf(".");
            QString finalFilename = tmpFilename.left(lastIdx);

            bool result = QFile::rename(tmpFilename, finalFilename);
            if (result) {
                this->validRequest = true;
                this->currentMessage = finalFilename;

                this->currentDownloadDetails->setFinalFilename(finalFilename);
            }
            else {
                this->validRequest = false;
                this->currentMessage = "Your file wasn't saved correctly.";
            }
        }
        else {            
            this->validRequest = false;
            this->currentMessage = "Final length of your file is invalid, try another download.";
        }
    }
    else {
        this->validRequest = false;
        this->currentMessage = "Something wrong when file was written.";
    }

    this->reply->deleteLater();
    this->reply = nullptr;

    emit finished();
}

void FileDownloader::reportError(QNetworkReply::NetworkError error)
{
    this->currentError = error;
}

QString FileDownloader::getNextAvailableName(QString tmpFilename)
{
    QString availableFilename = "";

    int lastIdx = tmpFilename.lastIndexOf('.');
    QString origFilename = tmpFilename.left(lastIdx);

    lastIdx = origFilename.lastIndexOf('.');
    QString baseName = origFilename.left(lastIdx);
    QString ext = origFilename.mid(lastIdx + 1);

    int k = 1;
    do {
        availableFilename = baseName + "_" + QString::number(k) + "." + ext + ".part";
        k++;
    } while (QFileInfo::exists(availableFilename));

    return availableFilename;
}
