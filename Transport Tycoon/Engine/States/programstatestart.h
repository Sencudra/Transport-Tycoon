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
	void showSettings();

private:

	bool showSettingsPanel;
	//char windowTitle[255];

    std::map<std::string, gui::Gui> guiSystem;

    sf::View view;
	sf::Music music;
	sf::Sprite m_background;
};


#endif // PROGRAMSTATESTART_H
