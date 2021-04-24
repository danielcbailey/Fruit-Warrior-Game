/*
 * Rendered Text Class
 *
 * The rendered text class allows for text to be drawn at any location on the display
 * with transparent backgrounds. Each character is 8 pixels tall and 6 pixels wide.
 *
 * (c) 2021 Daniel Cooper
 */

#include "renderedText.hpp"
#include "assets/assets.hpp"

RenderedText::RenderedText(std::string content, int x, int y) {
    this->x = x;
    this->y = y;
    this->text = "";
    this->setText(content);
}

RenderedText::~RenderedText() {
    this->setText(""); //Will automatically remove everything
}

void RenderedText::setText(std::string content) {
    //Comparing current and new, on the first difference, every character will
    //have to be unrendered.

    int index = -1;
    auto it = this->elements.begin();
    for (int i = 0; min(content.length(), this->text.length()) > i; i++) {
        if (content[i] != this->text[i]) {
            //Mismatch, must erase everything after this point
            break;
        }
        ++it;
        index = i;
    }

    //Removing all remaining characters
    while (it != elements.end()) {
        _globalGraphics->removeGraphicsElement(*it);

        delete(*it);

        //Removing element's pointer from list
        auto pIt = it;
        ++it;

        if (it == elements.end()) {
            elements.remove(*pIt);
            break;
        }

        elements.remove(*pIt);
    }

    //Rendering new characters
    for (int i = index + 1; content.length() > i; i++) {
        BitmapImage* charImg = characters[content[i] - ' '];
        GraphicsElement* elem = new GraphicsElement(charImg, this->x + 6 * i, this->y);
        elem->getContext()->Image.layer = 102;
        _globalGraphics->registerGraphicsElement(elem);
        this->elements.push_back(elem);
    }

    this->text = content;
}