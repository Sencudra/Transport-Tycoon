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


		ng::ProgramEngine * m_game;
		
		bool m_show_settings;

	};

	class GuiGame
	{
	public:

		GuiGame(ng::ProgramEngine* game, World* world);
		~GuiGame();

		// Gui modules are shown if gShow == 'true'

		void infoBar(bool gShow = true);
		void toolBar(bool gShow = true);

		void psbtn(ImVec2 buttonRect, ImFont* fontIcon); // Play / Stop button
		void spbtn(ImVec2 buttonRect, ImFont* fontIcon); // Speed butt
		void svbtn(ImVec2 buttonRect, ImFont* fontIcon); // Save button

	private:
		World * m_world;
		ng::ProgramEngine* m_game;

		std::vector<std::pair<std::string, std::string>> m_file_list;
		
	
		bool m_isPauseBtnActive;
		bool m_isSpeedBtnActive;

	};

}

#endif // GUI_H
