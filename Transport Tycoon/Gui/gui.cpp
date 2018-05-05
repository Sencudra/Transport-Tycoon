#include "gui.h"

#include "programengine.h"
#include "programstate.h"
#include "programstatemain.h"
#include "world.h"
#include "IconsKenney.h"
#include "myImGui.h"
#include "IOutput.h"

using namespace gui;


// Menu Gui

GuiMenu::GuiMenu(ng::ProgramEngine* game)
{

	ImGuiStyle& style = ImGui::GetStyle();
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.WindowBorderSize = 0.0f;					// Invisible border size
	style.ItemSpacing = ImVec2(5, 10);				// Menu item spacing

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
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
									ImGuiWindowFlags_NoResize |
									ImGuiWindowFlags_NoMove |
									ImGuiWindowFlags_NoScrollbar;
	
	ImGui::Begin("Menu", NULL, window_flags); // begin window
	{

		ImVec2 size = ImVec2(menuSize.x, menuSize.y / 8.0f); // Size for buttons

		//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
		//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
		//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

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
			}
			ImGui::PopFont();

			ImGui::PushFont(fontMenu);
			{
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

GuiGame::GuiGame(ng::ProgramEngine* game, World* world)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.WindowBorderSize = 0.0f;					// Invisible border size
	style.ItemSpacing = ImVec2(5, 10);				// Menu item spacing

	this->m_world = world;
	this->m_game = game;

	this->m_fileSelected = -1;
	char m_fileName[50] = "default.svt";

	bool m_isPauseBtnActive = false;
	bool m_isSpeedBtnActive = false;

	bool m_showSaveWindow = false;

	this->m_game->m_ioutput->getSaveList(m_file_list);
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

	ImGuiIO& io = ImGui::GetIO();
	ImGuiContext& g = *GImGui;

	int pcc = 0; // push color counter
	int pvc = 0; // push variable counter
	float buttonSize = 40.0f;

	ImVec4 btHoveredColor = (ImVec4)ImColor(146, 146, 146, 50);
	ImVec4 btBgColor = (ImVec4)ImColor(0, 0, 0, 0);
	ImVec4 btAcColor = (ImVec4)ImColor(15, 135, 250, 255);

	ImVec2 buttonRect = ImVec2(buttonSize, buttonSize);

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btHoveredColor); ++pcc;
	ImGui::PushStyleColor(ImGuiCol_Button, btBgColor);  ++pcc;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10)); ++pvc;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 8)); ++pvc;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10)); ++pvc;


	ImGui::BeginMainMenuBar();
	{
		
		// Using special y offset for icons
		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		ImFont* fontIcon = atlas->Fonts[1];
		
		bool flagPush = false;

		{	// First section

			// Play/Stop
			this->psbtn(buttonRect, fontIcon);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Pause button");


			// Speed button
			this->spbtn(buttonRect, fontIcon);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Fast Forward button");

			fontIcon->DisplayOffset.y = 2;
			ImGui::Button(ICON_KI_COG, buttonRect);
			fontIcon->DisplayOffset.y = 0;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Settings");
			
			// Save button
			this->svbtn(buttonRect, fontIcon);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Save / Load button");
		}
		ImGui::VerticalSeparator();
		{
			fontIcon->DisplayOffset.y = 2;
			ImGui::Button("?", buttonRect);
			fontIcon->DisplayOffset.y = 0;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Map");

			fontIcon->DisplayOffset.y = 2;
			ImGui::Button(ICON_KI_USER, buttonRect);
			fontIcon->DisplayOffset.y = 0;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Company profile");

		}
		ImGui::VerticalSeparator();
		{
			fontIcon->DisplayOffset.y = 2;
			ImGui::Button(ICON_KI_WRENCH, buttonRect);
			fontIcon->DisplayOffset.y = 0;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Building tool");

			fontIcon->DisplayOffset.y = 2;
			ImGui::Button(ICON_KI_CAR, buttonRect);
			fontIcon->DisplayOffset.y = 0;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Vehicle");
		}

	}
	ImGui::EndMainMenuBar();
	ImGui::PopStyleColor(pcc);
	ImGui::PopStyleVar(pvc);

}

void gui::GuiGame::psbtn(ImVec2 buttonRect, ImFont* fontIcon)
{	// Play/Stop button manipulator

	ImVec4 btAcColor = (ImVec4)ImColor(15, 135, 250, 255);
	bool flagPush = false;

	if (m_isPauseBtnActive) // Color manipulations
	{
		ImGui::PushStyleColor(ImGuiCol_Button, btAcColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btAcColor);
		flagPush = true;
	}
	fontIcon->DisplayOffset.y = 2;
	if (ImGui::Button(ICON_KI_PAUSE, buttonRect))
	{
		m_isPauseBtnActive = !m_isPauseBtnActive;
		this->m_world->switchPause();

		// Checks for other buttons enabled and disable
		if (m_isSpeedBtnActive)
		{
			m_isSpeedBtnActive = !m_isSpeedBtnActive;
			this->m_world->x2Speed();
		}
	}
	fontIcon->DisplayOffset.y = 0;
	if (flagPush) ImGui::PopStyleColor(2);
}

void gui::GuiGame::spbtn(ImVec2 buttonRect, ImFont* fontIcon)
{	// Speed button manipulator

	ImVec4 btAcColor = (ImVec4)ImColor(15, 135, 250, 255);
	bool flagPush = false;

	if (m_isSpeedBtnActive) // Color manipulations
	{
		ImGui::PushStyleColor(ImGuiCol_Button, btAcColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btAcColor);
		flagPush = true;
	}

	fontIcon->DisplayOffset.y = 2;
	if (ImGui::Button(ICON_KI_FORWARD, buttonRect))
	{
		m_isSpeedBtnActive = !m_isSpeedBtnActive;
		this->m_world->x2Speed();

		// Checks for other buttons enabled and disable
		if (m_isPauseBtnActive)
		{
			m_isPauseBtnActive = !m_isPauseBtnActive;
			this->m_world->switchPause();
		}
	}
	fontIcon->DisplayOffset.y = 0;
	if (flagPush) ImGui::PopStyleColor(2);

}

void gui::GuiGame::svbtn(ImVec2 buttonRect, ImFont* fontIcon)
{
	
	ImGuiStyle& style = ImGui::GetStyle();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;

	ImVec4 darkColor = (ImVec4)ImColor(0, 0, 0, 200);
	ImVec4 btBgColor = (ImVec4)ImColor(0, 0, 0, 0);
	// Current window style settings
	int pcc = 0; // push color counter
	int pvc = 0; // push variable counter

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10)); ++pvc;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10)); ++pvc;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 5)); ++pvc;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3); ++pvc;
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3); ++pvc;

	style.Colors[ImGuiCol_ModalWindowDarkening] = darkColor;
		
	fontIcon->DisplayOffset.y = 2;
	if (ImGui::Button(ICON_KI_SAVE, buttonRect))
	{
		fontIcon->DisplayOffset.y = 0;

		this->m_fileSelected = 0; // reset save position
		this->m_game->m_ioutput->getSaveList(m_file_list); // get list of save files available

		m_showSaveWindow = true; // window flag

		ImGui::OpenPopup("Save Game");
		//this->m_world->switchPause(); // in the future we can pause the game while saving
	}
	fontIcon->DisplayOffset.y = 0;
		
	if (ImGui::BeginPopupModal("Save Game", &m_showSaveWindow, window_flags))
	{
		ImGui::PopStyleColor(1); // back to previous color style
		
		ImGui::Text("Input save name or choose old.");		

		ImGui::PushItemWidth(350);	
		if (ImGui::InputText("##Filename", m_fileName, IM_ARRAYSIZE(m_fileName)))
		{
			m_fileSelected = -1;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if(ImGui::Button("Save"))
		{
			if (m_fileSelected != -1)
			{
				ImGui::OpenPopup("Continue"); // Ask for a confirm
			}
			else if(std::string(m_fileName) != "" )
			{
				this->m_game->io_saveGame(std::string(m_fileName)); // Passing filename to IO module
			}
		}

		ImGui::PushItemWidth(-1);
		if (ImGui::ListBox("##ListBox", &m_fileSelected, m_file_list))
		{
			strcpy_s(m_fileName, m_file_list[m_fileSelected].first.data());
		}
		ImGui::PopItemWidth();		
		ImGui::Separator();

		if (ImGui::Button("Return to Menu")) { ; }
		ImGui::SameLine();
		if (ImGui::Button("Return to Desctop")) { ; }

		// Modals
		if (ImGui::BeginPopupModal("Continue", NULL, window_flags))
		{
			ImGui::Text("This file will be overwritten.\nOperation cannot be undone!\n\n");
			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				this->m_game->io_saveGame(std::string(m_fileName)); // Passing filename to IO module
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, btBgColor);  ++pcc;
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(pvc);

	
}

