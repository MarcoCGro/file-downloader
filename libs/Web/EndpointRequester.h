#ifndef ENDPOINTREQUESTER_H
#define ENDPOINTREQUESTER_H

#include "NetworkManager.h"
#include "libs/Model/JsonValidator.h"

/**
 * @brief The EndpointRequester class is subclass of NetworkRequester class,
 * which is used to request the download details from a URL known
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 *
 * @see NetworkRequester, SettingsDialog
 */
class EndpointRequester : public NetworkManager
{
    Q_OBJECT

public:
    /**
     * @brief Fields labels for a download
     */
    const QList<QString> JSON_FIELDS = {"FileName", "Length", "BlobType", "DownloadURI"};

    EndpointRequester(QObject *parent = nullptr);
    ~EndpointRequester();

    /**
     * @brief Request the download details from a resource located
     * on a URL provided
     *
     * @param url Location of the resource (a json file)
     *
     * @see SettingsDialog::DEFAULT_DOWNLOADS_URL, SettingsDialog::currentFilesUrl
     */
    void requestFilesDetails(QString url);
    /**
     * @brief Recover the download details from the resource
     *
     * @return QJsonArray The download details obtained from the resources
     */
    QJsonArray getJsonArray() const;

    /**
     * @brief Validate the resource presence with its content
     *
     * @param url Location of the resource
     */
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
