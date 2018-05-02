#include "gui.h"

#include "programengine.h"
#include "programstate.h"
#include "programstatemain.h"
#include "world.h"
#include "IconsKenney.h"
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
	bool m_isPauseBtnActive = false;
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
	ImGui::ShowDemoWindow();

	//ImGui::ImageButton
	ImGuiIO& io = ImGui::GetIO();
	ImGuiContext& g = *GImGui;

	int pcc = 0; // push color counter
	int pvc = 0; // push variable counter

	ImVec4 btHoveredColor = (ImVec4)ImColor(146, 146, 146, 50);
	ImVec4 btBgColor = (ImVec4)ImColor(0, 0, 0, 0);
	ImVec4 btAcColor = (ImVec4)ImColor(15, 135, 250, 255);

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btHoveredColor); ++pcc;
	ImGui::PushStyleColor(ImGuiCol_Button, btBgColor);  ++pcc;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); ++pvc;

	ImGui::BeginMainMenuBar();
	{

		// Using special y offset for icons
		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImFont* fontIcon = atlas->Fonts[1];
		
		int offset = 2;
		int zeroOffset = 0;

		{	// First section

			// Play//Stop
			fontIcon->DisplayOffset.y = offset;
			bool flagPush = false;
			if (m_isPauseBtnActive) // Color manipulations
			{
				ImGui::PushStyleColor(ImGuiCol_Button, btAcColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btAcColor);
				flagPush = true;
			}
			if (!m_isPauseBtnActive)
			{
				if (ImGui::Button(ICON_KI_PAUSE))
				{
					m_isPauseBtnActive = !m_isPauseBtnActive;
					this->m_world->switchPause();
				}
			}
			else
			{
				if (ImGui::Button(ICON_KI_STOP))
				{
					m_isPauseBtnActive = !m_isPauseBtnActive;
					this->m_world->switchPause();
				}
			}
			if(flagPush) ImGui::PopStyleColor(2);
			fontIcon->DisplayOffset.y = zeroOffset;

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Pause button");



			fontIcon->DisplayOffset.y = offset;
			ImGui::Button(ICON_KI_FORWARD);
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Fast Forward button");

			fontIcon->DisplayOffset.y = offset;
			ImGui::Button(ICON_KI_COG);
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Settings");

			fontIcon->DisplayOffset.y = offset;
			ImGui::Button(ICON_KI_SAVE);
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Save / Load button");
		}
		ImGui::VerticalSeparator();
		{
			fontIcon->DisplayOffset.y = offset;
			ImGui::Button("?");
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Map");

			fontIcon->DisplayOffset.y = offset;
			ImGui::Button(ICON_KI_USER);
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Company profile");

		}
		ImGui::VerticalSeparator();
		{
			fontIcon->DisplayOffset.y = offset;
			ImGui::Button(ICON_KI_WRENCH);
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Building tool");

			fontIcon->DisplayOffset.y = offset;
			ImGui::Button(ICON_KI_CAR);
			fontIcon->DisplayOffset.y = zeroOffset;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Vehicle");
		}

	}
	ImGui::EndMainMenuBar();
	ImGui::PopStyleColor(pcc);
	ImGui::PopStyleVar(pvc);

}