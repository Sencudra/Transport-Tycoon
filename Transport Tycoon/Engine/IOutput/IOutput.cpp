#include "IOutput.h"
#include "map.h"

IOutput::IOutput()
{

}

IOutput::IOutput(World* world)
{
	this->m_world = world;
}


IOutput::~IOutput()
{
}

void IOutput::saveGameToFile()
{

}

void IOutput::loadGameFromFile()
{

}
