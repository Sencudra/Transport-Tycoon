#include <iostream>

#include "programengine.h"
#include "programstatestart.h"


#include <SFML/Audio/Music.hpp>


/* Project Transport Tycoon */

int main(int argc, char* argv[])
{

    std::cout << "Main::start" << std::endl;
    ng::ProgramEngine engine;
    engine.pushState(new ProgramStateStart(&engine));
    engine.loop();

    return 0;
}
