#include "TextIterator.hpp"

ConcreteTextAggregate::ConcreteTextAggregate(const QString& text) : text(text) {
    iterator = std::make_shared<ConcreteTextIterator>(text);
}

std::shared_ptr<TextIterator> ConcreteTextAggregate::createIterator() {
    if (!iterator) {
        iterator = std::make_shared<ConcreteTextIterator>(text);
    }
    return iterator;
}

void ConcreteTextAggregate::updateText(const QString& newText) {
    text = newText;
    if (iterator) {
        iterator->setText(newText);
    }
}

ConcreteTextIterator::ConcreteTextIterator(const QString& text)
    : text(text), position(0), charCount(0), wordCount(0), inWord(false) {
    updateCounts();
}

void ConcreteTextIterator::setText(const QString& newText) {
    text = newText;
    position = 0;
    updateCounts();
}

void ConcreteTextIterator::updateCounts() {
    charCount = 0;
    wordCount = 0;
    inWord = false;

    for (const QChar& ch : text) {
        charCount++;
        if (ch.isSpace() || ch.isPunct()) {
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