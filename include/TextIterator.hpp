#ifndef TEXTITERATOR_HPP
#define TEXTITERATOR_HPP

#include <QString>
#include <memory>

// Iterator interface
class TextIterator {
public:
    virtual ~TextIterator() = default;
    virtual bool hasNext() = 0;
    virtual QChar next() = 0;
};

// Aggregate
class TextAggregate {
public:
    virtual ~TextAggregate() = default;
    virtual std::shared_ptr<TextIterator> createIterator() = 0;
};

// Concrete Aggregate
class ConcreteTextAggregate : public TextAggregate {
public:
    ConcreteTextAggregate(const QString& text);
    std::shared_ptr<TextIterator> createIterator() override;

private:
    QString textData;
};

// Concrete Iterator
class ConcreteTextIterator : public TextIterator {
public:
    ConcreteTextIterator(const QString& text);
    bool hasNext() override;
    QChar next() override;

private:
    QString text;
    int position;
};

#endif // TEXTITERATOR_HPP