#pragma once

#include <QString>
#include <vector>
#include <memory>

// Observer interface
class IDocumentObserver {
public:
    virtual ~IDocumentObserver() = default;
    virtual void update(const QString& content) = 0;
};

// Subject interface
class IDocumentSubject {
public:
    virtual ~IDocumentSubject() = default;
    virtual void attach(std::shared_ptr<IDocumentObserver> observer) = 0;
    virtual void detach(std::shared_ptr<IDocumentObserver> observer) = 0;
    virtual void notify(const QString& content) = 0;
};

// Concrete Subject
class DocumentSubject : public IDocumentSubject {
public:
    void attach(std::shared_ptr<IDocumentObserver> observer) override;
    void detach(std::shared_ptr<IDocumentObserver> observer) override;
    void notify(const QString& content) override;

private:
    std::vector<std::shared_ptr<IDocumentObserver>> observers;
};

// Concrete Observer
class TextEditObserver : public IDocumentObserver {
public:
    TextEditObserver();
    void update(const QString& content) override;
    QString getLastUpdate() const;

private:
    QString lastUpdate;
};