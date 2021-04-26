

#ifndef TITLE_SCREEN_INCLUDED
#define TITLE_SCREEN_INCLUDED

#include "core/gameScreen.hpp"
#include "graphics/graphicsElement.hpp"
#include "game/fruitManager.hpp"
#include "game/guiButton.hpp"
#include "graphics/renderedText.hpp"
#include <list>

class TitleScreen : public GameScreen {
    private:

    GraphicsElement* background;
    GraphicsElement* samuraiLeft;
    GraphicsElement* samuraiRight;
    GraphicsElement* highScoreTitle;
    std::list<RenderedText*> text;
    GUIButton* startButton;
    GUIButton* highScoresButton;
    GUIButton* backButton;
    GraphicsElement* fruitLogo;
    FruitManager* fruitManager;
    bool initialized;
    bool mode;
    int joyEvtHandle;

    public:

    void onCreation();

    void onDestruction();

    void onGameLoop(float dt);

    void setMode(bool isHighScores);

    bool play(void* evtDetails, float dt);

    bool back(void* evtDetails, float dt);

    bool showHighScores(void* evtDetails, float dt);

    bool handleJoy(void* evtDetails, float dt);
};

extern TitleScreen _titleScreen;

#endif //TITLE_SCREEN_INCLUDED