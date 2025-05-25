#include "TextIterator.hpp"

ConcreteTextAggregate::ConcreteTextAggregate(const QString& text) : textData(text) {}

std::shared_ptr<TextIterator> ConcreteTextAggregate::createIterator() {
    return std::make_shared<ConcreteTextIterator>(textData);
}

ConcreteTextIterator::ConcreteTextIterator(const QString& text) : text(text), position(0) {}

bool ConcreteTextIterator::hasNext() {
    return position < text.length();
}

QChar ConcreteTextIterator::next() {
    return text[position++];
}