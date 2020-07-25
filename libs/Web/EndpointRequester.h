#ifndef ENDPOINTREQUESTER_H
#define ENDPOINTREQUESTER_H

#include "NetworkManager.h"
#include "libs/Model/JsonValidator.h"

/**
 * @brief The EndpointRequester class
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 */
class EndpointRequester : public NetworkManager
{
    Q_OBJECT

public:
    const QList<QString> JSON_FIELDS = {"FileName", "Length", "BlobType", "DownloadURI"};

    EndpointRequester(QObject *parent = nullptr);
    ~EndpointRequester();

    void requestFilesDetails(QString url);
    QJsonArray getJsonArray() const;

    void validateRequest(QString url);

protected slots:
    void filesDetailsReadyRead();
    void filesDetailsFinished();

    void verificationFinished();

protected:
    void verifyResult();
    bool validContent(QString content);

    JsonValidator *jsonValidator;
    QJsonArray jsonArray;

    QString currentContent;
};

#endif // ENDPOINTREQUESTER_H
