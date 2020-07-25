#ifndef DOWNLOADDETAILS_H
#define DOWNLOADDETAILS_H

#include <QJsonObject>
#include <QString>

/**
 * @brief The DownloadDetails class
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 */
class DownloadDetails
{
public:
    enum DownloadState {
        NON_STARTED = 0,
        IN_PROGRESS,
        PAUSED,
        FINISHED
    };

    DownloadDetails(QJsonObject jsonObject);

    QJsonObject getValuesAsJson();

    DownloadState getState() const;
    void setState(const DownloadState &value);

    bool getAcceptRanges() const;
    void setAcceptRanges(bool value);

    int getNumReceivedBytes() const;
    void setNumReceivedBytes(int value);

    QString getOutputFilename() const;
    void setOutputFilename(const QString &path);

    QString getFilename() const;
    void setFilename(const QString &value);

    int getLength() const;
    void setLength(int value);

    QString getBlobType() const;
    void setBlobType(const QString &value);

    QString getDownloadURI() const;
    void setDownloadURI(const QString &value);

    void printData();

protected:
    DownloadState state;
    bool acceptRanges;
    int numReceivedBytes;
    QString outputFilename;

    QString filename;
    double length;
    QString blobType;
    QString downloadURI;
};

#endif // DOWNLOADDETAILS_H
