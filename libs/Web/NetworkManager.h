#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtNetwork>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    bool isValidRequest();
    QString getCurrentMessage();

protected:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    bool validRequest;
    QString currentMessage;
};

#endif // NETWORKMANAGER_H
