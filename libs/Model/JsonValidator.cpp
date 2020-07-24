#include "JsonValidator.h"
#include <QJsonDocument>
#include <QJsonObject>

JsonValidator::JsonValidator(QList<QString> fields)
{
    this->validFields = fields;
}

QJsonObject JsonValidator::jsonFromExtendedContent(QString &content, bool *valid)
{
    QJsonObject jObject;

    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    if (!doc.isNull()) {
        if (doc.isObject()) {
            jObject = doc.object();
            *valid = true;

            if ( jObject.find("DownloadsUrl").value().toString() == "" ||
                 jObject.find("DownloadsDirectory").value().toString() == "") {
                *valid = false;
            }

            QJsonArray jArray = jObject.find("Downloads").value().toArray();
            *valid = validateArray(jArray);
        }
        else {
            *valid = false;
        }
    }
    else {
        *valid = false;
    }

    return jObject;
}

QJsonArray JsonValidator::jsonFromString(QString &content, bool *valid)
{
    QJsonArray array;

    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    if (!doc.isNull()) {
        if (doc.isArray()) {
            array = doc.array();
            *valid = validateArray(array);
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

bool JsonValidator::validateArray(QJsonArray jArray)
{
    bool valid = true;

    for (int i = 0; i < jArray.count(); i++) {
        if ( !jArray.at(i).isObject() || !containsValidFields(jArray.at(i).toObject()) ) {
            valid = false;
            break;
        }
    }

    return valid;
}

bool JsonValidator::containsValidFields(QJsonObject obj)
{
    for (int i = 0; i < this->validFields.count(); i++) {
        if ( obj[this->validFields.at(i)].toString() == "" )
            return false;
    }

    return true;
}
