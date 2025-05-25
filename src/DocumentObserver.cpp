#include "DocumentObserver.hpp"

void DocumentSubject::attach(std::shared_ptr<DocumentObserver> observer) {
    observers.push_back(observer);
}

void DocumentSubject::notify(const QString& content) {
    for (auto& observer : observers) {
        observer->update(content);
    }
}

TextEditObserver::TextEditObserver() : lastUpdate("") {}

void TextEditObserver::update(const QString& content) {
    lastUpdate = content;
}

QString TextEditObserver::getLastUpdate() const {
    return lastUpdate;
}