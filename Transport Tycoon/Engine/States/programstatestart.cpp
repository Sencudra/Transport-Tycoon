#include "programstate.h"
#include "programstatestart.h"
#include "programstatemain.h"

#include <utility>

ProgramStateStart::ProgramStateStart(ng::ProgramEngine* game)
{
	this->m_game = game;

	bool showSettingsPanel = false;

	if (!m_music.openFromFile("media/sound/music/play1.ogg"))
		std::cout << "ERROR while opening music file" << std::endl;
	
	//music.setVolume(10);
	//music.play();

	this->m_background.setTexture(*m_game->m_texmng->getTextureRef("bg1"));
	float scale = setupScale();
	this->m_background.setScale(scale, scale);
	this->m_background.setPosition(sf::Vector2f(0, 0));

	// Gui setup 
	m_gui_menu = new gui::GuiMenu(game);

	



}

ProgramStateStart::~ProgramStateStart()
{
	std::cout << "ProgramStateStart::~ProgramStateStart" << std::endl;
};

float ProgramStateStart::setupScale()
{
	sf::Vector2f screenSize = sf::Vector2f(m_game->m_window.getSize());
	std::cout << m_background.getTexture()->getSize().x <<" : " << m_background.getTexture()->getSize().y << std::endl;
	std::cout << m_background.getScale().x << " : " << m_background.getScale().y << std::endl;
	std::cout << screenSize.x << " : " << screenSize.y << std::endl;
	sf::Vector2f imgSize = sf::Vector2f(m_background.getTexture()->getSize());
	float scale;

	// backgound scale setup
	if (screenSize.x > imgSize.x || screenSize.y > imgSize.y)
	{
		if (screenSize.x - imgSize.x > 0 && screenSize.y - imgSize.y > 0)
		{
			if (screenSize.x - imgSize.x > screenSize.y - imgSize.y)
			{
				scale = screenSize.x / imgSize.x;
			}
			else
			{
				scale = screenSize.y / imgSize.y;
			}
		}
		else if (screenSize.x - imgSize.x > 0)
		{
			scale = screenSize.x / imgSize.x;
		}
		else
		{
			scale = screenSize.y / imgSize.y;
		}
	}
	else
	{
		if (abs(screenSize.x - imgSize.x) < abs(screenSize.y - imgSize.y))
		{
			scale = screenSize.x / imgSize.x;
		}
		else
		{
			scale = screenSize.y / imgSize.y;
		}
	}
	return scale;
}

void ProgramStateStart::draw(const float dt)
{

    this->m_game->m_window.clear(sf::Color::Black);

    //this->m_game->m_window.setView(this->view);
    this->m_game->m_window.draw(this->m_background);

	this->showImGui();

    //for(auto gui : this->guiSystem) this->m_game->m_window.draw(gui.second);

    return;
}

void ProgramStateStart::showImGui()
{
	this->m_gui_menu->menu();
	//this->m_game->pushState(new ProgramStateMain(0, this->m_game));
	
}


void ProgramStateStart::update(const float dt)
{



}


void ProgramStateStart::handleInput()
{
    sf::Event event;

    sf::Vector2f mousePos = this->m_game->m_window.mapPixelToCoords(
                sf::Mouse::getPosition(this->m_game->m_window),this->m_view);

    while(this->m_game->m_window.pollEvent(event))
    {
		ImGui::SFML::ProcessEvent(event);
        /* Event handler */
        switch(event.type)
        {

            /* Close the window */
            case sf::Event::Closed:
            {
                m_game->m_window.close();
                break;
            }


            /* Resize the window */
            case sf::Event::Resized:
            {
				float scale = setupScale();
				this->m_background.setScale(scale, scale);
				this->m_background.setPosition(sf::Vector2f(0, 0));
                break;
            }


            /* KeyBoard key pressed */
            case sf::Event::KeyPressed:
            {

                if(event.key.code == sf::Keyboard::Escape) this->m_game->m_window.close();
                break;

            }


            /* Mouse handler */
            case sf::Event::MouseButtonPressed:
            {

                
                break;

            }
            default:
                break;

        }
    }

    return;
}
