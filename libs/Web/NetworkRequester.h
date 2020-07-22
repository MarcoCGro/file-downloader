#ifndef NETWORKREQUESTER_H
#define NETWORKREQUESTER_H

#include <QtNetwork>
#include "JsonValidator.h"

class NetworkRequester : public QObject
{
    Q_OBJECT

public:
    const QList<QString> JSON_FIELDS = {"FileName", "Length", "BlobType", "DownloadURI"};

    NetworkRequester(QObject *parent = nullptr);
    ~NetworkRequester();

    void getFilesDetails(QString url);

    void validateEndpoint(QString url);
    bool isValidEndpoint();
    QString getCurrentMessage();

protected:
    bool validContent(QString content);

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    JsonValidator *jsonValidator;

protected slots:
    void filesDetailsReadyRead();
    void filesDetailsFinished();

    void verificationFinished();

private:
    bool validEndpoint;
    QString currentMessage;
};

#endif // NETWORKREQUESTER_H
