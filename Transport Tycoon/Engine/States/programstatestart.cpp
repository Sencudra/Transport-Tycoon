#include "programstate.h"
#include "programstatestart.h"
#include "programstatemain.h"

#include <utility>

ProgramStateStart::ProgramStateStart(ng::ProgramEngine* game)
{
	this->m_game = game;

	bool showSettingsPanel = false;
	//windowTitle[255] = char("Settings");

	if (!music.openFromFile("media/sound/music/play1.ogg"))
		std::cout << "ERROR while opening music file" << std::endl;
	
	//music.setVolume(10);
	//music.play();


	this->m_background.setTexture(*m_game->m_texmng->getTextureRef("bg1"));
	float scale = setupScale();
	this->m_background.setScale(scale, scale);
	this->m_background.setPosition(sf::Vector2f(0, 0));





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

    //for(auto gui : this->guiSystem) this->m_game->m_window.draw(gui.second);

    return;
}

void ProgramStateStart::showSettings()
{
	char windowTitle[255] = "Settings";
	sf::Vector2f windowSize = sf::Vector2f(this->m_game->m_window.getSize());
	sf::Vector2f menuSize = sf::Vector2f(windowSize.x*0.4f, windowSize.y*0.4f);
	sf::Vector2f menuPos;

	if (windowSize.x - menuSize.x >= 0)
		menuPos = sf::Vector2f((windowSize.x - menuSize.x) / 2.0f, (windowSize.y - menuSize.y) / 2.0f);
	else
		menuPos = sf::Vector2f(0, 0);

	//ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	ImGui::Begin("Settings", NULL, window_flags); // begin window
	{

		// Window title text edit
		ImGui::InputText("Window title", windowTitle, 255);

		if (ImGui::Button("Update window title")) {
			// this code gets if user clicks on the button
			// yes, you could have written if(ImGui::InputText(...))
			// but I do this to show how buttons work :)
			this->m_game->m_window.setTitle(windowTitle);
		}
	}
	ImGui::End();

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

		
		ImVec2 size = ImVec2(menuSize.x, menuSize.y/8.0f); // Size for buttons

		ImGuiStyle& style = ImGui::GetStyle();
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.WindowBorderSize = 0.0f;					// Invisible border size
		style.ItemSpacing = ImVec2(5,10);				// Menu item spacing
	

		// Font setup
		//ImGuiIO& io = ImGui::GetIO();
		//ImFont* fontCurrent = ImGui::GetFont();

		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImFont* fontName = atlas->Fonts[3];
		ImFont* fontMenu = atlas->Fonts[2];

		
		ImGui::BeginGroup();
		{
			const char name[20] = "Transport Paradise";

			ImGui::PushFont(fontName);
			{
				ImVec2 textSize = ImGui::CalcTextSize(name);
				ImVec2 textPos = ImVec2((menuSize.x - textSize.x) / 2.0f, 0);
				ImGui::SetCursorPos(textPos);
				ImGui::Text(name);
				//ImGui::InvisibleButton("Transport Paradise", size);
			}
			ImGui::PopFont();

			ImGui::PushFont(fontMenu);
			{
				//ImGui::SetCursorPos(ImVec2(textPos / 2.0f, 0););
				if (ImGui::Button("New Game", size)) {
					this->m_game->pushState(new ProgramStateMain(0, this->m_game));

				}
				if (ImGui::Button("Load Game", size)) {
					this->m_game->pushState(new ProgramStateMain(1, this->m_game));

				}
				if (ImGui::Button("Settings", size)) {
					this->showSettingsPanel = !(this->showSettingsPanel);

				}
				if (ImGui::Button("Exit", size)) {
					this->m_game->m_window.close();

				}
				ImGui::EndGroup();
			}
			ImGui::PopFont();
		}
	}
	ImGui::End(); // end window

	// Other windows

	if (showSettingsPanel) this->showSettings();

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
