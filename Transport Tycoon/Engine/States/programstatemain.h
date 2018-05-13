#pragma once 

#ifndef PROGRAMSTATEEDITOR_H
#define PROGRAMSTATEEDITOR_H

#include "resources.h"

#include "programState.h"
#include "view.h"


class World;
class Object;


class ProgramStateMain : public ProgramState
{
public:

	ProgramStateMain(World* world, ng::ProgramEngine* engine);
    ProgramStateMain(ng::ProgramEngine* game);
    ~ProgramStateMain();

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

	void setEditState(rs::EditState state) { m_editState = state; }
	void viewSetup();

	Object* getFocusedObject() { return m_focusObject; }

private:
    //void drawMap(int dt);

	void showImGui();

    /* Tool box signals maintenance */
    void s_speed();
    void s_pause();
    void s_save();


private:

    World* m_world;


    // View manipulation
    ScreenView m_gameView;
    sf::Vector2i m_panningAnchor;   // mouse anchor
    rs::ActionState m_actionState;      // mouse activity
    rs::EditState m_editState;
    float m_zoomLevel;

    sf::View m_guiView;
	gui::GuiGame* m_gui;

    //std::map<std::string, gui::Gui> guiSystem;

    Object* m_focusObject;

};

#endif // PROGRAMSTATEEDITOR_H
