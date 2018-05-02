#ifndef GUI_H
#define GUI_H


#include "resources.h"


// Initialization
namespace ng {
	class ProgramEngine;
}
class World;


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

		GuiGame(World* world);
		~GuiGame();

		// Gui modules are shown if gShow == 'true'

		void infoBar(bool gShow = true);
		void toolBar(bool gShow = true);

	private:
		World * m_world;
	
		bool m_isPauseBtnActive;

	};

}

#endif // GUI_H
