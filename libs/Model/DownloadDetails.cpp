#include "DownloadDetails.h"
#include <QDir>

DownloadDetails::DownloadDetails(QJsonObject jsonObject)
{
    this->filename = jsonObject["FileName"].toString();
    this->length = jsonObject["Length"].toString().toInt();
    this->blobType = jsonObject["BlobType"].toString();
    this->downloadURI = jsonObject["DownloadURI"].toString();

    this->state = DownloadState::NON_STARTED;
    this->numBytesReceived = 0;
    this->outputFilename = "";
}

DownloadDetails::DownloadState DownloadDetails::getState() const
{
    return this->state;
}

void DownloadDetails::setState(const DownloadState &value)
{
    this->state = value;
}

int DownloadDetails::getNumBytesReceived() const
{
    return numBytesReceived;
}

void DownloadDetails::setNumBytesReceived(int value)
{
    numBytesReceived = value;
}

QString DownloadDetails::getOutputFilename() const
{
    return this->outputFilename;
}

void DownloadDetails::setOutputFilename(const QString &path)
{
    this->outputFilename = path + QDir::separator() + this->filename.replace(' ', '_') + "." + this->blobType;
}

QString DownloadDetails::getFilename() const
{
    return this->filename;
}

void DownloadDetails::setFilename(const QString &value)
{
    this->filename = value;
}

int DownloadDetails::getLength() const
{
    return this->length;
}

void DownloadDetails::setLength(int value)
{
    this->length = value;
}

QString DownloadDetails::getBlobType() const
{
    return this->blobType;
}

void DownloadDetails::setBlobType(const QString &value)
{
    this->blobType = value;
}

QString DownloadDetails::getDownloadURI() const
{
    return this->downloadURI;
}

void DownloadDetails::setDownloadURI(const QString &value)
{
    this->downloadURI = value;
}

void DownloadDetails::printData()
{
    qDebug(" ------------------------------------------------------- ");
    qDebug("    FileName: %s", this->filename.toStdString().data());
    qDebug("      Length: %d", int(this->length));
    qDebug("    BlobType: %s", this->blobType.toStdString().data());
    qDebug(" DownloadURI: %s", this->downloadURI.toStdString().data());

    qDebug("       State: %d", this->state);
    qDebug("   NReceived: %d", this->numBytesReceived);
    qDebug("  OutputFile: %s", this->outputFilename.toStdString().data());
}
