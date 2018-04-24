#include "programstate.h"
#include "programstatestart.h"
#include "programstatemain.h"

#include <utility>

ProgramStateStart::ProgramStateStart(ng::ProgramEngine* game)
{

	if (!music.openFromFile("media/sound/music/play1.ogg"))
		std::cout << "ERROR while opening music file" << std::endl;
	
	//music.setVolume(10);
	//music.play();


    this->m_game = game;
    sf::Vector2f pos = sf::Vector2f(this->m_game->m_window.getSize());
    this->view.setSize(pos);
    pos *= 0.5f;
    this->view.setCenter(pos);

    this->guiSystem.emplace("menu", gui::Gui(sf::Vector2f(192, 32), 10, false, game->stylesheets.at("button"),
    {

        std::make_pair("New Game", "new_game"),
        std::make_pair("Load Game", "load_game"),
        std::make_pair("Exit Game", "exit_game")

    }));
    this->guiSystem.at("menu").setPosition(pos);
    this->guiSystem.at("menu").setOrigin(96, 32*1/2);
    this->guiSystem.at("menu").show();




}

void ProgramStateStart::draw(const float dt)
{

    this->m_game->m_window.clear(sf::Color::Black);

    this->m_game->m_window.setView(this->view);
    this->m_game->m_window.draw(this->m_game->m_background);

    for(auto gui : this->guiSystem) this->m_game->m_window.draw(gui.second);

    return;
}

void ProgramStateStart::update(const float dt)
{
	//std::cout << "check" << std::endl;

    this->guiSystem.at("menu").setEntryText(0, "New Game");
    this->guiSystem.at("menu").setEntryText(1, "Load Game");
    this->guiSystem.at("menu").setEntryText(2, "Quit");



	ImGui::Begin("Sample window"); // begin window

	float color[3] = { 0.f, 0.f, 0.f };
	char windowTitle[255] = "ImGui + SFML = <3";
	sf::Color bgColor;

	// Background color edit
	if (ImGui::ColorEdit3("Background color", color)) {
		// this code gets called if color value changes, so
		// the background color is upgraded automatically!
		bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
		bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
		bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
	}

	// Window title text edit
	ImGui::InputText("Window title", windowTitle, 255);

	//if (ImGui::Button("Update window title")) {
	//	// this code gets if user clicks on the button
	//	// yes, you could have written if(ImGui::InputText(...))
	//	// but I do this to show how buttons work :)
	//	m_window.setTitle(windowTitle);
	//}

	ImGui::End(); // end window

}

void ProgramStateStart::newGame()
{
    this->m_game->pushState(new ProgramStateMain(0, this->m_game));
}

void ProgramStateStart::loadGame()
{
    this->m_game->pushState(new ProgramStateMain(1, this->m_game));
}

void ProgramStateStart::handleInput()
{
    sf::Event event;

    sf::Vector2f mousePos = this->m_game->m_window.mapPixelToCoords(
                sf::Mouse::getPosition(this->m_game->m_window),this->view);

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
                this->view.setSize(event.size.width, event.size.height);
                this->m_game->m_background.setPosition(this->m_game->m_window.mapPixelToCoords(sf::Vector2i(0, 0)));
                this->m_game->m_background.setScale(
                  float(event.size.width) / float(this->m_game->m_background.getTexture()->getSize().x),
                  float(event.size.height) / float(this->m_game->m_background.getTexture()->getSize().y));
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

                if(event.mouseButton.button == sf::Mouse::Left)
                {

                    std::string msg = this->guiSystem.at("menu").activate(mousePos);

                    if(msg == "new_game")
                    {
                        this->newGame();
                    }
                    else if (msg == "load_game")
                    {
                        this->loadGame();
                    }
                    else if (msg == "exit_game")
                    {
                        this->m_game->m_window.close();
                    }

                }
                break;

            }
            default:
                break;

        }
    }

    return;
}
