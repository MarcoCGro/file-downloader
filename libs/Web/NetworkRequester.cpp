#include "NetworkRequester.h"

NetworkRequester::NetworkRequester(QObject *parent)
    : QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);
    this->reply = nullptr;

    this->jsonValidator = new JsonValidator(JSON_FIELDS);

    this->validEndpoint = false;
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

void NetworkRequester::getFilesDetails(QString url)
{
    this->request.setUrl(QUrl(url));
    this->reply = this->manager->get(request);

    connect(this->reply, &QIODevice::readyRead, this, &NetworkRequester::filesDetailsReadyRead);
    connect(this->reply, &QNetworkReply::finished, this, &NetworkRequester::filesDetailsFinished);
}

void NetworkRequester::filesDetailsReadyRead()
{
}

void NetworkRequester::filesDetailsFinished()
{
    this->reply->deleteLater();
    this->reply = nullptr;
}

void NetworkRequester::validateEndpoint(QString url)
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
    if (this->reply->error() == QNetworkReply::NoError) {
        if (validContent(this->reply->readAll())) {
            this->validEndpoint = true;
            this->currentMessage = "The specified URL is valid.";
        }
        else {
            this->validEndpoint = false;
            this->currentMessage = "The JSON file content is invalid.";
        }
    }
    else {
        this->validEndpoint = false;
        this->currentMessage = "Something went wrong. Please verify the specified URL and try again.";
    }

    this->reply->deleteLater();
    this->reply = nullptr;
}

bool NetworkRequester::validContent(QString content)
{
    bool isValidJson = false;
    this->jsonValidator->jsonFromString(content, &isValidJson);

    return isValidJson;
}

bool NetworkRequester::isValidEndpoint()
{
    return this->validEndpoint;
}

QString NetworkRequester::getCurrentMessage()
{
    return this->currentMessage;
}
