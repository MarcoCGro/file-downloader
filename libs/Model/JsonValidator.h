#ifndef JSONVALIDATOR_H
#define JSONVALIDATOR_H

#include <QJsonArray>
#include <QList>

class JsonValidator
{
public:
    JsonValidator(QList<QString> fields);

    QJsonObject jsonFromExtendedContent(QString &content, bool *valid);
    QJsonArray jsonFromString(QString &content, bool *valid);

protected:
    bool validateArray(QJsonArray jArray);
    bool containsValidFields(QJsonObject obj);

    QList<QString> validFields;
};

#endif // JSONVALIDATOR_H
