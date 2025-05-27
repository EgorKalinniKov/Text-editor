#include "DocumentObserver.hpp"
#include <algorithm>

void DocumentSubject::attach(std::shared_ptr<IDocumentObserver> observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void DocumentSubject::detach(std::shared_ptr<IDocumentObserver> observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void DocumentSubject::notify(const QString& content) {
    for (const auto& observer : observers) {
        if (observer) {
            observer->update(content);
        }
    }
}

TextEditObserver::TextEditObserver() : lastUpdate() {}

void TextEditObserver::update(const QString& content) {
    lastUpdate = content;
}

QString TextEditObserver::getLastUpdate() const {
    return lastUpdate;
}