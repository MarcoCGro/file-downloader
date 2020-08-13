#include "DownloadDetails.h"

#include <QDir>
#include <QVariant>

DownloadDetails::DownloadDetails(QJsonObject jsonObject)
{
    this->filename = jsonObject["FileName"].toString();
    this->length = jsonObject["Length"].toString().toDouble();
    this->blobType = jsonObject["BlobType"].toString();
    this->downloadURI = jsonObject["DownloadURI"].toString();

    this->state = DownloadState::NON_STARTED;
    QString currState = jsonObject.find("State").value().toString();
    if (currState != "0")
        this->state = DownloadState(currState.toInt());

    QString currAcceptRanges = jsonObject.find("AcceptRanges").value().toString();
    this->acceptRanges = currAcceptRanges == "true";

    this->numReceivedBytes = 0;
    QString currBytesReceived = jsonObject.find("NumReceivedBytes").value().toString();
    if (currState != "0")
        this->numReceivedBytes = currBytesReceived.toInt();

    this->outputFilename = jsonObject["OutputFilename"].toString();
}

QJsonObject DownloadDetails::getValuesAsJson()
{
    QJsonObject jsonObject;

    jsonObject.insert("FileName", this->filename);
    jsonObject.insert("Length", QVariant(this->length).toString());
    jsonObject.insert("BlobType", this->blobType);
    jsonObject.insert("DownloadURI", this->downloadURI);
    jsonObject.insert("State", QString::number(this->state));
    jsonObject.insert("AcceptRanges", this->acceptRanges ? "true" : "false");
    jsonObject.insert("NumReceivedBytes", QVariant(this->numReceivedBytes).toString());
    jsonObject.insert("OutputFilename", this->outputFilename);

    return jsonObject;
}

DownloadDetails::DownloadState DownloadDetails::getState() const
{
    return this->state;
}

void DownloadDetails::setState(const DownloadState &value)
{
    this->state = value;
}

bool DownloadDetails::getAcceptRanges() const
{
    return acceptRanges;
}

void DownloadDetails::setAcceptRanges(bool value)
{
    acceptRanges = value;
}

double DownloadDetails::getNumReceivedBytes() const
{
    return this->numReceivedBytes;
}

void DownloadDetails::setNumReceivedBytes(double value)
{
    this->numReceivedBytes = value;
}

QString DownloadDetails::getOutputFilename() const
{
    return this->outputFilename;
}

void DownloadDetails::setOutputFilename(const QString &path)
{
    QString origFilename = QString(this->filename);
    this->outputFilename = path + QDir::separator() + origFilename.replace(' ', '_') + "." + this->blobType + ".part";
}

void DownloadDetails::setFinalFilename(const QString &filename)
{
    this->outputFilename = filename;
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
    qDebug("     FileName: %s", this->filename.toStdString().data());
    qDebug("       Length: %f", this->length);
    qDebug("     BlobType: %s", this->blobType.toStdString().data());
    qDebug("  DownloadURI: %s", this->downloadURI.toStdString().data());

    qDebug("        State: %d", this->state);
    qDebug(" AcceptRanges: %i", this->acceptRanges);
    qDebug("    NReceived: %d", this->numReceivedBytes);
    qDebug("   OutputFile: %s", this->outputFilename.toStdString().data());
}
