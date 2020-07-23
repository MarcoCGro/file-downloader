#include "NetworkRequester.h"

NetworkRequester::NetworkRequester(QObject *parent)
    : QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);
    this->reply = nullptr;

    this->jsonValidator = new JsonValidator(JSON_FIELDS);
    this->currentContent = "";

    this->validRequest = false;
    this->currentMessage = "";
}

NetworkRequester::~NetworkRequester()
{
    if (this->manager != nullptr) {
        delete this->manager;
        this->manager = nullptr;
    }

    if (this->reply != nullptr) {
        delete this->reply;
        this->reply = nullptr;
    }

    if (this->jsonValidator != nullptr) {
        delete this->jsonValidator;
        this->jsonValidator = 0;
    }
}

void NetworkRequester::requestFilesDetails(QString url)
{
    this->request.setUrl(QUrl(url));
    this->reply = this->manager->get(request);

    connect(this->reply, &QIODevice::readyRead, this, &NetworkRequester::filesDetailsReadyRead);
    connect(this->reply, &QNetworkReply::finished, this, &NetworkRequester::filesDetailsFinished);

    QEventLoop loop;
    connect(this->reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

QJsonArray NetworkRequester::getJsonArray() const
{
    return this->jsonArray;
}

void NetworkRequester::filesDetailsReadyRead()
{
    verifyResult();
    this->jsonArray = this->jsonValidator->jsonFromString(this->currentContent, &this->validRequest);
}

void NetworkRequester::filesDetailsFinished()
{
    this->reply->deleteLater();
    this->reply = nullptr;
}

void NetworkRequester::validateRequest(QString url)
{
    this->request.setUrl(QUrl(url));
    this->reply = this->manager->get(request);

    connect(this->reply, &QNetworkReply::finished, this, &NetworkRequester::verificationFinished);

    QEventLoop loop;
    connect(this->reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void NetworkRequester::verificationFinished()
{
    verifyResult();

    this->reply->deleteLater();
    this->reply = nullptr;
}

void NetworkRequester::verifyResult()
{
    this->validRequest = false;

    if (this->reply->error() == QNetworkReply::NoError) {
        this->currentContent = this->reply->readAll();

        if (validContent(this->currentContent)) {
            this->validRequest = true;
            this->currentMessage = "The specified URL is valid.";
        }
        else {
            this->validRequest = false;
            this->currentMessage = "The JSON file content is invalid.";
        }
    }
    else {
        this->currentContent = "";

        this->validRequest = false;
        this->currentMessage = "Something went wrong. Please verify the specified URL and try again.";
    }
}

bool NetworkRequester::validContent(QString content)
{
    bool isValidJson = false;
    this->jsonValidator->jsonFromString(content, &isValidJson);

    return isValidJson;
}

bool NetworkRequester::isValidRequest()
{
    return this->validRequest;
}

QString NetworkRequester::getCurrentMessage()
{
    return this->currentMessage;
}
