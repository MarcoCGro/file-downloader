#ifndef JSONVALIDATOR_H
#define JSONVALIDATOR_H

#include <QJsonArray>
#include <QList>

class JsonValidator
{
public:
    JsonValidator(QList<QString> fields);

    QJsonArray jsonFromString(QString &content, bool *valid);

protected:
    bool containsValidFields(QJsonObject obj);

    QList<QString> validFields;
};

#endif // JSONVALIDATOR_H
