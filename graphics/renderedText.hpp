/*
 * Rendered Text Class
 *
 * The rendered text class allows for text to be drawn at any location on the display
 * with transparent backgrounds. Each character is 8 pixels tall and 6 pixels wide.
 *
 * (c) 2021 Daniel Cooper
 */

#ifndef RENDERED_TEXT_INCLUDED
#define RENDERED_TEXT_INCLUDED

#include <string>
#include "graphicsController.hpp"
#include <list>

class RenderedText {

    private:

    std::string text;
    int x;
    int y;
    std::list<GraphicsElement*> elements;

    public:

    RenderedText(std::string content, int x, int y);

    ~RenderedText();

    inline std::string getText() { return this->text; }

    void setText(std::string content);
};

#endif //RENDERED_TEXT_INCLUDED