#include "EndpointRequester.h"

EndpointRequester::EndpointRequester(QObject *parent) :
    NetworkManager(parent)
{
    this->jsonValidator = new JsonValidator(JSON_FIELDS);
    this->currentContent = "";
}

EndpointRequester::~EndpointRequester()
{
    if (this->jsonValidator != nullptr) {
        delete this->jsonValidator;
        this->jsonValidator = 0;
    }
}

void EndpointRequester::requestFilesDetails(QString url)
{
    this->request.setUrl(QUrl(url));
    this->reply = this->manager->get(request);

    connect(this->reply, &QIODevice::readyRead, this, &EndpointRequester::filesDetailsReadyRead);
    connect(this->reply, &QNetworkReply::finished, this, &EndpointRequester::filesDetailsFinished);

    QEventLoop loop;
    connect(this->reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

QJsonArray EndpointRequester::getJsonArray() const
{
    return this->jsonArray;
}

void EndpointRequester::filesDetailsReadyRead()
{
    verifyResult();
    this->jsonArray = this->jsonValidator->jsonFromString(this->currentContent, &this->validRequest);
}

void EndpointRequester::filesDetailsFinished()
{
    this->reply->deleteLater();
    this->reply = nullptr;
}

void EndpointRequester::validateRequest(QString url)
{
    this->request.setUrl(QUrl(url));
    this->reply = this->manager->get(request);

    connect(this->reply, &QNetworkReply::finished, this, &EndpointRequester::verificationFinished);

    QEventLoop loop;
    connect(this->reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void EndpointRequester::verificationFinished()
{
    verifyResult();

    this->reply->deleteLater();
    this->reply = nullptr;
}

void EndpointRequester::verifyResult()
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

bool EndpointRequester::validContent(QString content)
{
    bool isValidJson = false;
    this->jsonValidator->jsonFromString(content, &isValidJson);

    return isValidJson;
}
