#pragma once

#include <QString>
#include <QObject>
#include <memory>

// Iterator interface
class TextIterator {
public:
    virtual ~TextIterator() = default;
    virtual bool hasNext() = 0;
    virtual QChar next() = 0;
    virtual int getCharCount() const = 0;
    virtual int getWordCount() const = 0;
};

// Aggregate interface
class TextAggregate {
public:
    virtual ~TextAggregate() = default;
    virtual std::shared_ptr<TextIterator> createIterator() = 0;
};

// Concrete Iterator
class ConcreteTextIterator : public TextIterator {
public:
    explicit ConcreteTextIterator(const QString& text);
    bool hasNext() override;
    QChar next() override;
    int getCharCount() const override;
    int getWordCount() const override;

private:
    QString text;
    int position;
    int charCount;
    int wordCount;
    bool inWord;

    void updateCounts();
};

// Concrete Aggregate
class ConcreteTextAggregate : public TextAggregate {
public:
    explicit ConcreteTextAggregate(const QString& text);
    std::shared_ptr<TextIterator> createIterator() override;

private:
    QString text;
};