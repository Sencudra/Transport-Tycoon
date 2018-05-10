#pragma once

#ifndef PROGRAMENGINE_H
#define PROGRAMENGINE_H

/////////////////////////////////////////////////////////////////////////////////////////
/// NAMESPACE NG
///
///

#include <stack>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "texturemanager.h"
#include "gui/gui.h"


class ProgramState;
class IOutput;

namespace ng {
    class ProgramEngine
    {
    public:
        ProgramEngine();
        ~ProgramEngine();

        void pushState(ProgramState* state);
        void popState();
        void changeState(ProgramState* state);
        ProgramState* peekState();

        boost::property_tree::ptree boostIni();
        int renderIni();
		

        void loop();

		// IOutput interface

		void io_setupIO(World* world);
		void io_saveGame(std::string filename);
		void io_loadGame(std::string filename);

		// StateMain connections
		void setEditState(rs::EditState state);


    public:
		ProgramState * m_active_state;
        std::stack<ProgramState*> states;

        DataManager* m_texmng;
        //sf::Sprite m_background;
        sf::RenderWindow m_window;
        boost::property_tree::ptree m_iniFile;
		IOutput* m_ioutput; // IO file working

       // std::map<std::string, gui::GuiStyle> stylesheets;
        std::map<std::string, sf::Font> fonts;

    private:
        void loadTextures();
        void loadStylesheets();
        void loadFonts();

    private:
        sf::Clock* m_clock;
		
    };
}

#endif // PROGRAMENGINE_H
