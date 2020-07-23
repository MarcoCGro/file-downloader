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

    void requestFilesDetails(QString url);
    QJsonArray getJsonArray() const;

    void validateRequest(QString url);
    bool isValidRequest();
    QString getCurrentMessage();

protected slots:
    void filesDetailsReadyRead();
    void filesDetailsFinished();

    void verificationFinished();

protected:
    void verifyResult();
    bool validContent(QString content);

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    JsonValidator *jsonValidator;
    QJsonArray jsonArray;

    QString currentContent;

    bool validRequest;
    QString currentMessage;
};

#endif // NETWORKREQUESTER_H
