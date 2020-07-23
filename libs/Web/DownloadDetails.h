#ifndef DOWNLOADDETAILS_H
#define DOWNLOADDETAILS_H

#include <QJsonObject>
#include <QString>

class DownloadDetails
{
public:
    DownloadDetails(QJsonObject jsonObject);
    DownloadDetails();

    QString getFilename() const;
    void setFilename(const QString &value);

    double getLength() const;
    void setLength(double value);

    QString getBlobType() const;
    void setBlobType(const QString &value);

    QString getDownloadURI() const;
    void setDownloadURI(const QString &value);

    void printData();

protected:
    QString filename;
    double length;
    QString blobType;
    QString downloadURI;
};

#endif // DOWNLOADDETAILS_H
