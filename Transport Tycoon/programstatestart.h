#ifndef PROGRAMSTATESTART_H
#define PROGRAMSTATESTART_H

#include "programstate.h"

class ProgramStateStart: public ProgramState
{
public:

    ProgramStateStart(ng::ProgramEngine* game);


    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();



private:
    void newGame();
    void loadGame();

private:

    std::map<std::string, gui::Gui> guiSystem;

    sf::View view;
};


#endif // PROGRAMSTATESTART_H
