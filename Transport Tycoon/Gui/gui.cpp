#include "gui.h"

#include "programengine.h"
#include "programstate.h"
#include "programstatemain.h"
#include "world.h"
#include "IconsKenney.h"
//#include "imgui.h"
//#include "imgui_internal.h"

#include "myImGui.h"

using namespace gui;


// Menu Gui

GuiMenu::GuiMenu(ng::ProgramEngine* game)
{
	this->m_game = game;
	
	// variables
	
	bool m_show_settings = false;
}


GuiMenu::~GuiMenu()
{

}

void GuiMenu::menu(bool gShow)
{
	// Setup Menu Block Size
	sf::Vector2f windowSize = sf::Vector2f(this->m_game->m_window.getSize());
	sf::Vector2f menuSize = sf::Vector2f(windowSize.x*0.4f, windowSize.y*0.4f);
	sf::Vector2f menuPos;

	if (windowSize.x - menuSize.x >= 0)
		menuPos = sf::Vector2f((windowSize.x - menuSize.x) / 2.0f, (windowSize.y - menuSize.y) / 2.0f);
	else
		menuPos = sf::Vector2f(0, 0);

	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Menu", NULL, window_flags); // begin window
	{


		ImVec2 size = ImVec2(menuSize.x, menuSize.y / 8.0f); // Size for buttons

		ImGuiStyle& style = ImGui::GetStyle();
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.WindowBorderSize = 0.0f;					// Invisible border size
		style.ItemSpacing = ImVec2(5, 10);				// Menu item spacing


		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImFont* fontName = atlas->Fonts[3];
		ImFont* fontMenu = atlas->Fonts[2];


		ImGui::BeginGroup();
		{

			ImGui::PushFont(fontName);
			{
				ImVec2 textSize = ImGui::CalcTextSize(rs::G_PROGRAM_NAME);
				ImVec2 textPos = ImVec2((menuSize.x - textSize.x) / 2.0f, 0);
				ImGui::SetCursorPos(textPos);
				ImGui::Text(rs::G_PROGRAM_NAME);
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
					this->m_show_settings = !(this->m_show_settings);

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

	if (m_show_settings) this->settings();
}

void GuiMenu::settings()
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

// Game Gui

GuiGame::GuiGame(World* world)
{
	this->m_world = world;
}


GuiGame::~GuiGame()
{

}


void GuiGame::infoBar(bool gShow)
{
	ImGuiContext* g = ImGui::GetCurrentContext();
	ImGuiStyle& s = ImGui::GetStyle();
	ImGuiIO& IO = ImGui::GetIO();

	float xSizeCoef = 0.98f;

	ImGui::SetNextWindowSize(ImVec2(g->IO.DisplaySize.x*xSizeCoef, 10 + g->FontBaseSize + g->Style.FramePadding.y));
	ImGui::SetNextWindowPos(ImVec2((g->IO.DisplaySize.x - g->NextWindowData.SizeVal.x) / 2.0f, g->IO.DisplaySize.y - g->NextWindowData.SizeVal.y));

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs;

	ImVec4 bgColor = (ImVec4)ImColor(15, 15, 15, 255);
	ImVec4 sepColor = (ImVec4)ImColor::HSV(0, 0, 0, 0);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 7));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
	ImGui::PushStyleColor(ImGuiCol_Separator, sepColor);

	ImGui::Begin("BottomBar", NULL, window_flags);
	{
		float money = this->m_world->m_player.getBalance();

		ImGui::Columns(3, "word-wrapping");
		ImGui::Separator();
		ImGui::TextWrapped("Day %d", this->m_world->getDayCount());

		ImGui::NextColumn();
		ImGui::TextWrapped(" -- Current state -- ");
		ImGui::NextColumn();
		ImGui::TextWrapped(" Money: %d $", this->m_world->m_player.getBalance());
		ImGui::Columns(1);
		ImGui::Separator();

	}

	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(2);

	ImGui::End();
}

void GuiGame::toolBar(bool gShow)
{
	//ImGui::ImageButton
	ImGuiIO& io = ImGui::GetIO();
	ImGuiContext& g = *GImGui;


	ImTextureID my_tex_id = io.Fonts->TexID;
	float my_tex_w = (float)io.Fonts->TexWidth;
	float my_tex_h = (float)io.Fonts->TexHeight;

	//g.FontBaseSize = 20;
	ImGui::ShowDemoWindow();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10,10));

	ImGui::BeginMainMenuBar();
	{
		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImFont* font = atlas->Fonts[0];
		font->DisplayOffset.y = 5; // magic
		ImGui::PushFont(font);

		ImGui::Button(ICON_KI_PAUSE);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Pause button");

		ImGui::Button(ICON_KI_FORWARD);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Fast Forward button");

		ImGui::Button(ICON_KI_COG);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Settings");

		ImGui::Button(ICON_KI_SAVE);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Save / Load button");


		font->DisplayOffset.y = 0; // magic
		ImGui::PopFont();

		//int frame_padding = -1; 
		//ImGui::ImageButton(my_tex_id, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f / my_tex_w, 32 / my_tex_h), -1, ImColor(0, 0, 0, 255));


	}
	ImGui::EndMainMenuBar();
	ImGui::PopStyleVar(1);

}