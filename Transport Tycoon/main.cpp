#include <iostream>

#include "programengine.h"
#include "programstatestart.h"
#include "programstatemain.h" // FIX ME


#include <SFML/Audio/Music.hpp>


/* Project Transport Tycoon */

int main(int argc, char* argv[])
{
	printf("Current compiler version is ...%d\n", _MSC_VER);
	printf("Current compiler full version is ...%d\n", _MSC_FULL_VER);

    std::cout << "Main::start" << std::endl;
    ng::ProgramEngine engine;
    engine.pushState(new ProgramStateStart(&engine));
//	engine.pushState(new ProgramStateMain(0, &engine));
    engine.loop();

    return 0;
}

