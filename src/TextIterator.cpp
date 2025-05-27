#include "TextIterator.hpp"

ConcreteTextAggregate::ConcreteTextAggregate(const QString& text) : text(text) {}

std::shared_ptr<TextIterator> ConcreteTextAggregate::createIterator() {
    return std::make_shared<ConcreteTextIterator>(text);
}

ConcreteTextIterator::ConcreteTextIterator(const QString& text)
    : text(text), position(0), charCount(0), wordCount(0), inWord(false) {
    // Начальный подсчет
    for (const QChar& ch : text) {
        charCount++;
        if (ch.isSpace()) {
            if (inWord) {
                wordCount++;
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }
    if (inWord) {
        wordCount++;
    }
    position = 0;
    inWord = false;
}

bool ConcreteTextIterator::hasNext() {
    return position < text.length();
}

QChar ConcreteTextIterator::next() {
    if (hasNext()) {
        return text[position++];
    }
    return QChar();
}

int ConcreteTextIterator::getCharCount() const {
    return charCount;
}

int ConcreteTextIterator::getWordCount() const {
    return wordCount;
}