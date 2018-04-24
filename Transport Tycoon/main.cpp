#include <iostream>

#include "programengine.h"
#include "programstatestart.h"


#include <SFML/Audio/Music.hpp>


/* Project Transport Tycoon */

int main(int argc, char* argv[])
{
	printf("Current compiler version is ...%d\n", _MSC_VER);
	printf("Current compiler full version is ...%d\n", _MSC_FULL_VER);

	

    std::cout << "Main::start" << std::endl;
    ng::ProgramEngine engine;
    engine.pushState(new ProgramStateStart(&engine));
    engine.loop();

    return 0;
}

//sf::RenderWindow window(sf::VideoMode(640, 480), "");
//window.setVerticalSyncEnabled(true);
//ImGui::SFML::Init(window);
//
//sf::Color bgColor;
//
//float color[3] = { 0.f, 0.f, 0.f };
//
//// let's use char array as buffer, see next part
//// for instructions on using std::string with ImGui
//char windowTitle[255] = "ImGui + SFML = <3";
//
//window.setTitle(windowTitle);
//window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
//sf::Clock deltaClock;
//while (window.isOpen()) {
//	sf::Event event;
//	while (window.pollEvent(event)) {
//		ImGui::SFML::ProcessEvent(event);
//
//		if (event.type == sf::Event::Closed) {
//			window.close();
//		}
//	}
//
//	ImGui::SFML::Update(window, deltaClock.restart());
//
//
//
//	
//
//	window.clear(bgColor); // fill background with color
//	ImGui::SFML::Render(window);
//	window.display();
//}

