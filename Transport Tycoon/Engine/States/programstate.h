#pragma once

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>

#include "programengine.h"
#include "gui\gui.h"


class ProgramState
{
    public:

    ng::ProgramEngine* m_game;

    virtual void draw(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void handleInput() = 0;

    virtual ~ProgramState()
    {
        std::cout << "PROGRAMSTATE::destructor..." << std::endl;
    }

};



#endif // GAMESTATE_H
