#include "DownloadDetails.h"

DownloadDetails::DownloadDetails(QJsonObject jsonObject)
{
    this->filename = jsonObject["FileName"].toString();
    this->length = jsonObject["Length"].toString().toDouble();
    this->blobType = jsonObject["BlobType"].toString();
    this->downloadURI = jsonObject["DownloadURI"].toString();
}

DownloadDetails::DownloadDetails()
{
    this->filename = "";
    this->length = 0.0;
    this->blobType = "";
    this->downloadURI = "";
}

QString DownloadDetails::getFilename() const
{
    return this->filename;
}

void DownloadDetails::setFilename(const QString &value)
{
    this->filename = value;
}

double DownloadDetails::getLength() const
{
    return this->length;
}

void DownloadDetails::setLength(double value)
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
}
