#ifndef DOCUMENTOBSERVER_HPP
#define DOCUMENTOBSERVER_HPP

#include <QString>
#include <vector>
#include <memory>

// Observer interface
class DocumentObserver {
public:
    virtual ~DocumentObserver() = default;
    virtual void update(const QString& content) = 0;
};

// Subject
class DocumentSubject {
public:
    void attach(std::shared_ptr<DocumentObserver> observer);
    void notify(const QString& content);

private:
    std::vector<std::shared_ptr<DocumentObserver>> observers;
};

// Concrete Observer (UI component)
class TextEditObserver : public DocumentObserver {
public:
    TextEditObserver();
    void update(const QString& content) override;
    QString getLastUpdate() const;

private:
    QString lastUpdate;
};

#endif // DOCUMENTOBSERVER_HPP