#include "JsonValidator.h"
#include <QJsonDocument>
#include <QJsonObject>

JsonValidator::JsonValidator(QList<QString> fields)
{
    this->validFields = fields;
}

QJsonArray JsonValidator::jsonFromString(QString &content, bool *valid)
{
    QJsonArray array;

    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    if (!doc.isNull()) {
        if (doc.isArray()) {
            array = doc.array();
            *valid = true;

            for (int i = 0; i < array.count(); i++) {
                if ( !array.at(i).isObject() || !containsValidFields(array.at(i).toObject()) ) {
                    *valid = false;
                    break;
                }
            }
        }
        else {
            *valid = false;
        }
    }
    else {
        *valid = false;
    }

    return array;
}

bool JsonValidator::containsValidFields(QJsonObject obj)
{
    for (int i = 0; i < this->validFields.count(); i++) {
        if ( obj[this->validFields.at(i)].toString() == "" )
            return false;
    }

    return true;
}
