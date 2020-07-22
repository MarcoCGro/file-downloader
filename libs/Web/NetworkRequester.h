#ifndef NETWORKREQUESTER_H
#define NETWORKREQUESTER_H

#include <QtNetwork>

class NetworkRequester : public QObject
{
    Q_OBJECT

public:
    NetworkRequester(QObject *parent = nullptr);
    ~NetworkRequester();

    void validateEndpoint(QString url);
    bool isValidEndpoint();
    QString getCurrentMessage();

protected:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

protected slots:
    void verificationReadyRead();
    void verificationFinished();

private:
    bool validEndpoint;
    QString currentMessage;
};

#endif // NETWORKREQUESTER_H
