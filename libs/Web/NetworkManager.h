#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtNetwork>

/**
 * @brief The NetworkManager class is a base/template class which
 * defines necessary properties used to make network requests.
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    /**
     * @brief Returns if the immediate previous request was successful;
     * so, <code>validRequest</code> attribute should be updated for this
     *
     * @return bool <code>True</code> if immediate previous request was
     * successful; <code>false</code> otherwise
     */
    bool isValidRequest();
    /**
     * @brief Returns a message related to some result after a request;
     * so, <code>currentMessage</code> attibute should be update for this
     *
     * @return
     */
    QString getCurrentMessage();

protected:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    bool validRequest;
    QString currentMessage;
};

#endif // NETWORKMANAGER_H
