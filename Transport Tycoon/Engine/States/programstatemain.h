#ifndef PROGRAMSTATEEDITOR_H
#define PROGRAMSTATEEDITOR_H

#include "resources.h"

#include "programState.h"
#include "view.h"


class World;
class Object;

enum class ActionState{NONE, PANNING};
enum class EditState{NONE, ROADING, CARSETUP, ROUTING, DELETING};


class ProgramStateMain : public ProgramState
{
public:

    ProgramStateMain(int mode,ng::ProgramEngine* game);
    ~ProgramStateMain();

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

private:
    void drawMap(int dt);

    /* Tool box signals maintenance */
    void s_speed();
    void s_pause();
    void s_save();


private:

    World* m_world;


    // View manipulation
    ScreenView m_gameView;
    sf::Vector2i m_panningAnchor;   // mouse anchor
    ActionState m_actionState;      // mouse activity
    EditState m_editState;
    float m_zoomLevel;

    sf::View m_guiView;

    std::map<std::string, gui::Gui> guiSystem;

    Object* m_focusObject;

};

#endif // PROGRAMSTATEEDITOR_H
