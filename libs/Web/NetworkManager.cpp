#include "NetworkManager.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);
    this->reply = nullptr;

    this->validRequest = false;
    this->currentMessage = "";
}

NetworkManager::~NetworkManager()
{
    if (this->manager != nullptr) {
        delete this->manager;
        this->manager = nullptr;
    }

    if (this->reply != nullptr) {
        delete this->reply;
        this->reply = nullptr;
    }
}

bool NetworkManager::isValidRequest()
{
    return this->validRequest;
}

QString NetworkManager::getCurrentMessage()
{
    return this->currentMessage;
}
