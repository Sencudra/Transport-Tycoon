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

    public:
        std::stack<ProgramState*> states;

        TextureManager* m_texmng;
        sf::Sprite m_background;
        sf::RenderWindow m_window;
        boost::property_tree::ptree m_iniFile;

        std::map<std::string, gui::GuiStyle> stylesheets;
        std::map<std::string, sf::Font> fonts;

    private:
        void loadTextures();
        void loadStylesheets();
        void loadFonts();

    private:
        sf::Clock* m_clock;
		IOutput* m_ioutput;
    };
}

#endif // PROGRAMENGINE_H
