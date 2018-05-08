

#include <iostream>
#include <array>

#include <imgui.h>
#include <imgui_internal.h>


#include "resources.h"
#include "programengine.h"
#include "programstate.h"
#include "programstatemain.h"
#include "IOutput.h"
#include "IconsKenney.h"



using namespace ng;

struct Rect
{
	float x;
	float y;
	float w;
	float h;
};


ProgramEngine::ProgramEngine()
{
    std::cout << "ENGINE::ProgramEngine:: game setup" << std::endl;

	m_active_state = nullptr;
    //  boostIni() must be first
    //  reading config data from ini file
    m_iniFile = this->boostIni();
    this->renderIni();

	ImGui::CreateContext();
    m_texmng = new TextureManager();
    this->loadTextures();
    this->loadFonts();
    this->loadStylesheets();
    //this->m_background.setTexture(*this->m_texmng->getTextureRef("bg1"));

	std::string defPath = "C:/Users/vladt/source/repos/Transport Tycoon/Transport Tycoon/saves/";
	m_ioutput = new IOutput(this, defPath);

    m_clock = new sf::Clock();

}

ProgramEngine::~ProgramEngine()
{
    std::cout << "ENGINE::destructor::game shutdown" << std::endl;

    while(!this->states.empty()) popState();

    delete m_texmng;
    //delete m_window;
    delete m_clock;
}

void ProgramEngine::pushState(ProgramState *state)
{
    this->states.push(state);
	m_active_state = this->peekState();

    return;
}

void ProgramEngine::popState()
{
    delete this->states.top();
    this->states.pop();
	m_active_state = this->peekState();

    return;
}

void ProgramEngine::changeState(ProgramState *state)
{
    if(!this->states.empty())
        popState();
    pushState(state);
    return;
}

ProgramState* ProgramEngine::peekState()
{
    if(this->states.empty()) return nullptr;
    return this->states.top();
}

boost::property_tree::ptree ProgramEngine::boostIni()
{

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("tycoon.ini", pt);

    return pt;
}

int ProgramEngine::renderIni()
{

    std::string windowName = "Transport Tycoon ";

    int screenWidth = std::stoi(m_iniFile.get<std::string>("video.screen_width"));
    int screenHeight = std::stoi(m_iniFile.get<std::string>("video.screen_height"));

    //m_window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight,8), windowName);
	
	m_window.create(sf::VideoMode(screenWidth, screenHeight, 8), windowName);


	m_window.setVerticalSyncEnabled(true);
    m_window.setFramerateLimit(60);

    // console output
    std::cout << "CONFIG::Width: "  << screenWidth << std::endl;
    std::cout << "CONFIG::Height: " << screenHeight << std::endl;

    return 0;
}

void ProgramEngine::loop()
{
	std::cout << "ProgramEngine::loop" << std::endl;
    //sf::RenderWindow* m_renderWindow = m_window;

    // Time variables
    sf::Clock clock;
    sf::Clock fps_clock;
    sf::Time fps_time;

	ImGui::SFML::Init(m_window);

    while( m_window.isOpen())
    {
        float time = m_clock->getElapsedTime().asMicroseconds();
        m_clock->restart();
        time = time/800;

        fps_time = fps_clock.getElapsedTime();
        //std::cout << 1.0f/fps_time.asSeconds() << std::endl;
        fps_clock.restart().asSeconds();

        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        if(m_active_state == nullptr) continue;
		
		m_active_state->handleInput();

		
		ImGui::SFML::Update(m_window, elapsed);
		m_window.clear(sf::Color::Black);
		m_active_state->update(dt);

		
		m_active_state->draw(dt);
		
		ImGui::SFML::Render(m_window);
		m_window.display();
		
    }
	ImGui::SFML::Shutdown();
}

void ng::ProgramEngine::io_setupIO(World * world) { m_ioutput->updateWorld(world); }

void ng::ProgramEngine::io_saveGame(std::string filename){ 

	sf::Clock clock;
	m_ioutput->saveGameToFile(filename);
	float time = clock.getElapsedTime().asSeconds();
	std::cout << "ng::ProgramEngine::io_saveGame: TIME SAVING: " << time << std::endl;

}

void ng::ProgramEngine::io_loadGame(std::string filename)
{

	sf::Clock clock;
	m_ioutput->loadGameFromFile(filename);
	float time = clock.getElapsedTime().asSeconds();
	std::cout << "ng::ProgramEngine::io_loadGame: TIME LOADING: " << time << std::endl;
}

void ng::ProgramEngine::setEditState(rs::EditState state)
{
	ProgramStateMain* stateMain = dynamic_cast<ProgramStateMain*> (this->peekState());
	stateMain->setEditState(state);
}

void ProgramEngine::loadTextures()
{

    //Background
    m_texmng->loadTexture("flat1", "media/images/flat1.jpg");
	m_texmng->loadTexture("bg1", "media/images/bg1.jpg");

    //Textures
    m_texmng->loadTexture("bg_deepwater",       "media/textures/land/deepwater.png");
    m_texmng->loadTexture("bg_factory",         "media/textures/land/factory.png");
    m_texmng->loadTexture("bg_water",           "media/textures/land/water.png");
    m_texmng->loadTexture("bg_sand",            "media/textures/land/sand.png");
    m_texmng->loadTexture("bg_plain",           "media/textures/land/plain.png");
    m_texmng->loadTexture("bg_forest",          "media/textures/land/forest.png");
    m_texmng->loadTexture("bg_stone1",          "media/textures/land/stone1.png");
    m_texmng->loadTexture("bg_stone2",          "media/textures/land/stone2.png");
    m_texmng->loadTexture("bg_snow",            "media/textures/land/snow.png");
    m_texmng->loadTexture("bg_red",             "media/textures/land/red.png");

    m_texmng->loadTexture("auto",               "media/object.png");

    //Indutry
    m_texmng->loadTexture(rs::IndustryType::COALMINE,       "media/textures/industry/coalmine.png");
    //m_texmng->loadTexture("ind_bank",           "media/textures/industry/bank.png");
    //m_texmng->loadTexture("ind_farm",           "media/textures/industry/farm.png");
    //m_texmng->loadTexture("ind_forest",         "media/textures/industry/forest(industry).png");
    //m_texmng->loadTexture("ind_ironoremine",    "media/textures/industry/ironoremine.png");
    //m_texmng->loadTexture("ind_oilrefinery",    "media/textures/industry/oilrefinary.png");
    //m_texmng->loadTexture("ind_oilrig",         "media/textures/industry/oilrig.png");
    //m_texmng->loadTexture("ind_oilwells",       "media/textures/industry/oilwells.png");
    m_texmng->loadTexture(rs::IndustryType::POWERSTATION,   "media/textures/industry/powerstation.png");
    //m_texmng->loadTexture("ind_sawmill",        "media/textures/industry/sawmill.png");
    //m_texmng->loadTexture("ind_steelmill",      "media/textures/industry/steelmill.png");
    //m_texmng->loadTexture("ind_factory",        "media/textures/industry/factory.png");


    /* Road loading */
    m_texmng->loadTexture(rs::RoadType::ROAD_0_PATH1,   "media/textures/roads/road0path1.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_1_PATH1,   "media/textures/roads/road1path1.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_1_PATH2,   "media/textures/roads/road1path2.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_1_PATH3,   "media/textures/roads/road1path3.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_1_PATH4,   "media/textures/roads/road1path4.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_2_PATH1,   "media/textures/roads/road2path1.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_2_PATH2,   "media/textures/roads/road2path2.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_2_PATH3,   "media/textures/roads/road2path3.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_2_PATH4,   "media/textures/roads/road2path4.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_2_PATH5,   "media/textures/roads/road2path5.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_2_PATH6,   "media/textures/roads/road2path6.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_3_PATH1,   "media/textures/roads/road3path1.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_3_PATH2,   "media/textures/roads/road3path2.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_3_PATH3,   "media/textures/roads/road3path3.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_3_PATH4,   "media/textures/roads/road3path4.png");
    m_texmng->loadTexture(rs::RoadType::ROAD_4_PATH1,   "media/textures/roads/road4path1.png");


}
// Text Sizes available in px
enum TextSize
{
	PxSize_12 = 12, PxSize_14 = 14, PxSize_16 = 16,
	PxSize_18 = 18, PxSize_20 = 20, PxSize_22 = 22, 
	PxSize_24 = 24, PxSize_26 = 26, PxSize_28 = 28,
	PxSize_30 = 30, PxSize_32 = 32, PxSize_34 = 34,
	PxSize_36 = 36, PxSize_38 = 38, PxSize_40 = 40,
};

void ProgramEngine::loadFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig icons_config;

	io.Fonts->AddFontDefault();

	io.Fonts->AddFontFromFileTTF("media/fonts/RobotoSlab-Regular.ttf", TextSize::PxSize_18);

	// Merging previous font with icons
	static const ImWchar icons_ranges[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
	
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_KI, TextSize::PxSize_18, &icons_config, icons_ranges);

	io.Fonts->AddFontFromFileTTF("media/fonts/RobotoSlab-Bold.ttf", TextSize::PxSize_20);
	io.Fonts->AddFontFromFileTTF("media/fonts/ARCADECLASSIC.TTF", TextSize::PxSize_32);
	io.Fonts->AddFontFromFileTTF("media/fonts/ARCADECLASSIC.TTF", TextSize::PxSize_20);

	unsigned char * pixels;
	int width, height, bytes_per_pixels;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);
    return;
}

void ProgramEngine::loadStylesheets()
{
    //this->stylesheets["button"] = gui::GuiStyle(&this->fonts.at("second_font"), 1,
    //    sf::Color(0xc6,0xc6,0xc6), sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00),
	//     sf::Color(0x61,0x61,0x61), sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00));
    //this->stylesheets["text"] = gui::GuiStyle(&this->fonts.at("main_font"), 0,
    //    sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00), sf::Color(0xff,0xff,0xff),
    //    sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00), sf::Color(0xff,0x00,0x00));

    return;
}




