#ifndef JSONVALIDATOR_H
#define JSONVALIDATOR_H

#include <QJsonArray>
#include <QList>

/**
 * @brief The JsonValidator class verifies the content of a json file
 * corresponding a downloads URL provided from main application
 *
 * @author Marco A.Caballero Gro.
 * @version 1.0
 *
 * @see SettingsDialog::DEFAULT_DOWNLOADS_URL, SettingsDialog::currentFilesUrl
 */
class JsonValidator
{
public:
    /**
     * @brief Builds an instance which will verify the content
     * of a json file using the parameter provided
     *
     * @param fields List of labels for presence verification
     *
     * @see MainWindow::JSON_EXTENDED_FIELDS, EndpointRequester::JSON_FIELDS
     */
    JsonValidator(QList<QString> fields);

    /**
     * @brief Validates and returns a json object containing the values
     * from the downloads details
     *
     * @param content Extended json content as a string
     * @param valid Saves if the provided content fits with the fields
     * verification
     *
     * @return QJsonObject Contains the json representation for the
     * extended content of the argument
     */
    QJsonObject jsonFromExtendedContent(QString &content, bool *valid);
    /**
     * @brief Validates and returns a json object containing the values
     * from the downloads details
     *
     * @param content Json content as a string
     * @param valid Saves if the provided content fits with the fields
     * verification
     *
     * @return QJsonObject Contains the json representation for the
     * content of the argument
     */
    QJsonArray jsonFromString(QString &content, bool *valid);

protected:
    bool validateArray(QJsonArray jArray);
    bool containsValidFields(QJsonObject obj);

    QList<QString> validFields;
};

#endif // JSONVALIDATOR_H
