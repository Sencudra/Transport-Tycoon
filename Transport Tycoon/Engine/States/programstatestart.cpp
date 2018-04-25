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
    //sf::Vector2f pos = sf::Vector2f(this->m_game->m_window.getSize());
    //this->view.setSize(pos);
    //pos *= 0.5f;
    //this->view.setCenter(pos);

    /*this->guiSystem.emplace("menu", gui::Gui(sf::Vector2f(192, 32), 10, false, game->stylesheets.at("button"),
    {

        std::make_pair("New Game", "new_game"),
        std::make_pair("Load Game", "load_game"),
        std::make_pair("Exit Game", "exit_game")

    }));
    this->guiSystem.at("menu").setPosition(pos);
    this->guiSystem.at("menu").setOrigin(96, 32*1/2);
    this->guiSystem.at("menu").show();*/




}

void ProgramStateStart::draw(const float dt)
{

    this->m_game->m_window.clear(sf::Color::Black);

    this->m_game->m_window.setView(this->view);
    this->m_game->m_window.draw(this->m_game->m_background);

    //for(auto gui : this->guiSystem) this->m_game->m_window.draw(gui.second);

    return;
}

void ProgramStateStart::update(const float dt)
{

	// DEMO
	ImGui::ShowDemoWindow();

	// Setup GUi Appearance

	// Setup Menu Block Size
	sf::Vector2f windowSize = sf::Vector2f(this->m_game->m_window.getSize());
	sf::Vector2f menuSize = sf::Vector2f(windowSize.x*0.4f, windowSize.y*0.4f);
	sf::Vector2f menuPos;

	if (windowSize.x - menuSize.x >= 0)
		menuPos = sf::Vector2f((windowSize.x - menuSize.x) / 2.0f, (windowSize.y - menuSize.y) / 2.0f);
	else
		menuPos = sf::Vector2f(0,0);

	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
									ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
	
	ImGui::Begin("Menu",NULL, window_flags); // begin window
	{

		
		ImVec2 size = ImVec2(menuSize.x, menuSize.y/8.0f);
		ImGuiStyle& style = ImGui::GetStyle();
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.WindowBorderSize = 0.0f;
	

		ImGuiIO& io = ImGui::GetIO();
		ImFont* font_current = ImGui::GetFont();

		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImFont* font = atlas->Fonts[3];



		{
			ImGui::BeginGroup();
			
			ImGui::PushFont(font);
			ImGui::Text("Transport Paradise");
			ImGui::InvisibleButton("Transport Paradise", size);
			ImGui::PopFont();

			if (ImGui::Button("New Game", size)) {
				this->m_game->pushState(new ProgramStateMain(0, this->m_game));

			}
			if (ImGui::Button("Load Game", size)) {


			}
			if (ImGui::Button("Settings", size)) {


			}
			if (ImGui::Button("Exit", size)) {


			}
			ImGui::EndGroup();
		}
	}
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

					std::string msg;// = this->guiSystem.at("menu").activate(mousePos);

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
