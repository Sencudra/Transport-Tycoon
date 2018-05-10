#pragma once

#ifndef GUI_H
#define GUI_H


#include "resources.h"


// Initialization
namespace ng {
	class ProgramEngine;
}
class World;
class ImVec2;
class ImFont;


namespace gui {
	// Full app's Gui module

	class GuiMenu
	{
	public:

		GuiMenu(ng::ProgramEngine* game);
		~GuiMenu();

		// Gui modules are shown if gShow == 'true'

		void menu(bool gShow = true);

	private:
		void settings();
		void loadGame();


		ng::ProgramEngine * m_game;
		
		bool m_isLoadPanelActive;
		bool m_isSettingsPanelAxtive;


		std::vector<std::pair<std::string, std::string>> m_file_list;
		char m_fileName[100];
		int m_fileSelected;

	};

	class GuiGame
	{
	public:

		GuiGame(ng::ProgramEngine* game, World* world);
		~GuiGame();

		// Gui modules are shown if gShow == 'true'

		void infoBar(bool gShow = true);
		void toolBar(bool gShow = true);

		void pauseButton(ImVec2 buttonRect, ImFont* fontIcon); // Play / Stop button
		void speedButton(ImVec2 buttonRect, ImFont* fontIcon); // Speed butt
		void saveButton(ImVec2 buttonRect, ImFont* fontIcon); // Save button
		void roadBuilderButton(ImVec2 buttonRect, ImFont* fontIcon); // Place road
		void vehicleBuilderButton(ImVec2 buttonRect, ImFont* fontIcon); // Place vechicle

	private:
		World * m_world;
		ng::ProgramEngine* m_game;

		bool m_isLoadMenuActive;

		
		bool m_isPauseBtnActive;
		bool m_isSpeedBtnActive;
		bool m_isRoadBuilderBtnActive;
		bool m_isVehicleBuilderBtnActive;
	
		bool m_showSaveWindow;
		std::vector<std::pair<std::string, std::string>> m_file_list;
		char m_fileName[50];
		int m_fileSelected;

	};

}

#endif // GUI_H
