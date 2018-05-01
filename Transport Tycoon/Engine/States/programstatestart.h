#ifndef PROGRAMSTATESTART_H
#define PROGRAMSTATESTART_H

#include "programstate.h"

class ProgramStateStart: public ProgramState
{
public:

    ProgramStateStart(ng::ProgramEngine* game);
	~ProgramStateStart();

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();


private:

	float setupScale();
	void  showImGui();

private:
	

	gui::GuiMenu* m_gui_menu;

    sf::View m_view;
	sf::Music m_music;
	sf::Sprite m_background;
};


#endif // PROGRAMSTATESTART_H
