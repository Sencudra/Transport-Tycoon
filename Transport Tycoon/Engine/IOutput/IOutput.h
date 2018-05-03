#pragma once

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


class World;

class IOutput
{
public:

	IOutput(World* world);
	IOutput();

	void saveGameToFile();
	void loadGameFromFile();

	void getSaveList(std::vector<std::pair<std::string, std::string>>& m_file_list);

	~IOutput();


private:
	World * m_world;


};

