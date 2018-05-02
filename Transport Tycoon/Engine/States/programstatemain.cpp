#include "resources.h"
#include "map.h"
#include "view.h"
#include "world.h"
#include "programstate.h"
#include "programstatemain.h"
#include "player.h"

#include "gui.h"

#include <imgui.h>
#include <imgui_internal.h>


ProgramStateMain::ProgramStateMain(int mode, ng::ProgramEngine* game)
{

    std::cout << "ProgramStateMain::ProgramStateMain" << std::endl;

    this->m_game = game;

    if(mode == 1)
    {
		// load from file
        m_world = new World(1, game, this);
    }
    else
    {
		// new game
        m_world = new World(0, game, this);
    }

	// Gui Setup

	this->m_gui = new gui::GuiGame(m_world);

    /* View setup */
    sf::Vector2f pos = sf::Vector2f(this->m_game->m_window.getSize());
    sf::Vector2f centre(0, this->m_world->getTileMapSize()*0.5*32);
    this->m_gameView = ScreenView(pos);
    m_gameView.setCenter(centre);           // Centre the camera on the map

	// for static background
    sf::FloatRect guiView = sf::FloatRect(sf::Vector2f(0,0),pos);
    m_guiView = View(guiView);

    this->m_zoomLevel = 1.0f;
    this->m_actionState = ActionState::NONE;
    this->m_editState = EditState::NONE;

    m_focusObject = nullptr;

}

ProgramStateMain::~ProgramStateMain()
{
    std::cout << "ProgramStateMain::~ProgramStateMain" << std::endl;
    delete m_world;
}

void ProgramStateMain::draw(const float dt)
{

    this->m_game->m_window.clear(sf::Color::Black);

    //this->m_game->m_window.setView(this->m_guiView);
    //this->m_game->m_window.draw(this->m_game->m_background);

    this->m_game->m_window.setView(this->m_gameView);
    this->m_world->draw(this->m_gameView);

	// DEMO Imgui
	//ImGui::ShowDemoWindow();

    return;
}

void ProgramStateMain::update(const float dt)
{
    m_gameView.viewMap(dt);
    m_gameView.changeView();

    m_world->update(dt);

	this->showImGui();

    return;
}

void ProgramStateMain::s_pause()
{
    m_world->switchPause();
}

void ProgramStateMain::s_save()
{
    m_world->saveToFile();
}

void ProgramStateMain::s_speed()
{
    m_world->x2Speed();
   
}

void ProgramStateMain::handleInput()
{
    sf::Event event;

    while(this->m_game->m_window.pollEvent(event))      
    {
        switch(event.type)
        {
        case sf::Event::MouseMoved:
        {

            /* Pan the camera */
            if(this->m_actionState == ActionState::PANNING)
            {
                rs::Rectangle mapEdges = m_world->getTileMapEdges();

                sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->m_game->m_window) - this->m_panningAnchor);

                sf::Vector2f newPos = m_gameView.getCenter() + (-1.0f * pos * this->m_zoomLevel);

                // Camera moving limit according to map size/edges and screen size
                //          TL
                //        *    *
                //      BL      TR
                //        *    *
                //          BR


                if(newPos.y - (m_gameView.getSize().y/2) > mapEdges.topLeft.y &&
                   newPos.y + (m_gameView.getSize().y/2) < mapEdges.bottomRight.y &&
                   newPos.x - (m_gameView.getSize().x/2) > mapEdges.bottomLeft.x &&
                   newPos.x + (m_gameView.getSize().x/2) < mapEdges.topRight.x)
                {
                    m_gameView.move(-1.0f * pos * this->m_zoomLevel);

                }
                m_panningAnchor = sf::Mouse::getPosition(this->m_game->m_window);

            }
            break;
        }

        /* Press mouse button */
        case sf::Event::MouseButtonPressed:
        {


            /* Start panning */
            if(event.mouseButton.button == sf::Mouse::Middle)
            {
                if(this->m_actionState != ActionState::PANNING)
                {
                    this->m_actionState = ActionState::PANNING;
                    this->m_panningAnchor = sf::Mouse::getPosition(this->m_game->m_window);
                }
            }

			/* Right click: */
            if(event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2f mousePosWorld = this->m_game->m_window.mapPixelToCoords(sf::Mouse::getPosition(this->m_game->m_window),this->m_gameView);

            }

			/* Left mouse click */
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = this->m_game->m_window.mapPixelToCoords(sf::Mouse::getPosition(this->m_game->m_window),this->m_guiView);
                sf::Vector2f mousePosWorld = this->m_game->m_window.mapPixelToCoords(sf::Mouse::getPosition(this->m_game->m_window),this->m_gameView);

                if(m_focusObject != nullptr && m_editState == EditState::NONE)
                {					
                    m_focusObject->m_isSelected = false;
                    m_focusObject->m_sprite.setColor(Color::White);
					std::cout << "ProgramStateMain::handleInput: Object unselected" << std::endl;
                }

				// Selecting object
				if (m_focusObject != nullptr && m_editState == EditState::NONE)
				{
					m_focusObject = m_world->selectObject(mousePosWorld);
					std::cout << "ProgramStateMain::handleInput: Object selected" << std::endl;
				}

				// Adding task for vechicle
				if (m_focusObject != nullptr && m_editState == EditState::ROUTING)
				{	
					rs::Point task;
					task.setValues(mousePosWorld.x, mousePosWorld.y);
					rs::isoToTwoD(task.x, task.y, 64, 32);
					dynamic_cast<DynamicObject*> (m_focusObject)->addTask(task);
					std::cout << "ProgramStateMain::handleInput: Task added for vechicle" << std::endl;

				}

                /* Tool Box panel handling */
                //std::string msg = this->guiSystem.at("toolBar").activate(mousePos);

                //if(msg == "time")
                //{
                //    this->s_pause();
                //}
                //else if (msg == "speed")
                //{
                //    if(m_world->isPause())
                //        this->s_pause();
                //    this->s_speed();
                //}
                //else if (msg == "exit_game")
                //{
                //    this->m_game->m_window.close();
                //}
                //else if (msg == "save")
                //{
                //    this->s_pause();
                //    this->s_save();
                //    this->s_pause();
                //}
                //else if (msg == "setup_road")
                //{
                //    if(this->m_editState != EditState::ROADING)
                //    {
                //        this->m_editState = EditState::ROADING;
                //        this->guiSystem.at("toolBar").highlight(3);
                //    }
                //    else if(this->m_editState == EditState::ROADING)
                //    {
                //        this->m_editState = EditState::NONE;
                //        this->guiSystem.at("toolBar").highlight(3);     //unhighlight
                //    }
                //}
                //else if (msg == "setup_car")
                //{
                //    if(this->m_editState != EditState::CARSETUP)
                //    {
                //        this->m_editState = EditState::CARSETUP;
                //        this->guiSystem.at("toolBar").highlight(4);
                //    }
                //    else if (this->m_editState == EditState::CARSETUP)
                //    {
                //        this->m_editState = EditState::NONE;
                //        this->guiSystem.at("toolBar").highlight(4);
                //    }
                //}
                //else if (msg == "exit")
                //{
                //    //this->s_exit();
                //    this->m_game->m_window.close();
                //}
                //else
                //{
                //    // actions
                //    if(this->m_editState == EditState::ROADING)
                //    {
                //        m_world->addRoad(mousePosWorld.x, mousePosWorld.y);
                //    }

                //    if(this->m_editState == EditState::CARSETUP)
                //    {
                //        m_focusObject = m_world->addVehicle(mousePosWorld.x, mousePosWorld.y);
                //        if(m_focusObject == nullptr)
                //            std::cout << "wrong place" << std::endl;
                //    }
                //}
            }
            break;
        }


        /* Release mouse button */
        case sf::Event::MouseButtonReleased:
        {
            /* Stop panning */
            if(event.mouseButton.button == sf::Mouse::Middle)
            {
                this->m_actionState = ActionState::NONE;
            }
            break;
        }


        /* Zoom the view */
        case sf::Event::MouseWheelMoved:
        {
            if(event.mouseWheel.delta < 0)
            {
                m_gameView.zoom(2.0f);
                m_zoomLevel *= 2.0f;
            }
            else
            {
                m_gameView.zoom(0.5f);
                m_zoomLevel *= 0.5f;
            }
            break;
        }


        /* Close the window */
        case sf::Event::Closed:
        {
            this->m_game->m_window.close();
            break;
        }


        /* Resize the window */
        case sf::Event::Resized:
        {

            m_gameView.setSize(event.size.width, event.size.height);
            m_gameView.zoom(m_zoomLevel);

			m_guiView.setSize(event.size.width, event.size.height);


            break;
        }


        case sf::Event::KeyPressed:
        {
            if(event.key.code == sf::Keyboard::Escape)
            {
                if(m_focusObject != nullptr)
                {
                    m_editState = EditState::NONE;
                    m_focusObject->m_isSelected = false;
                    m_focusObject->m_sprite.setColor(Color::White);
                }
            }
			if (event.key.code == sf::Keyboard::T)
			{
				if (m_focusObject != nullptr && m_focusObject->m_objectType == rs::ObjectType::VECHICALE)
					m_editState = EditState::ROUTING;
			}
            if(event.key.code == sf::Keyboard::C)
            {

                if(m_focusObject != nullptr && m_focusObject->m_objectType == rs::ObjectType::VECHICALE)
                {

                    m_world->deleteVec(m_focusObject);
                    m_focusObject = nullptr;

                }

                if(m_editState == EditState::ROADING)
                {
                    sf::Vector2f mousePosWorld = this->m_game->m_window.mapPixelToCoords(sf::Mouse::getPosition(this->m_game->m_window),this->m_gameView);
                    m_world->deleteObject(mousePosWorld);
                }
            }
            break;
        }


        default: break;
        }
    }

    return;
}

void ProgramStateMain::showImGui()
{
	// Enable default font
	ImFontAtlas* atlas = ImGui::GetIO().Fonts;
	ImFont* font = atlas->Fonts[1];
	ImGui::PushFont(font);

	this->m_gui->infoBar(true);
	this->m_gui->toolBar(true);
	ImGui::ShowDemoWindow();

	ImGui::PopFont();
}