#pragma once

class World;

class IOutput
{
public:

	IOutput(std::string defaultPath);
	IOutput();

	void saveGameToFile(std::string defaultPath);
	void loadGameFromFile(std::string defaultPath);

	void getSaveList(std::vector<std::pair<std::string, std::string>>& m_file_list);

	~IOutput();


	void updateWorld(World* world) { m_world = world; }


private:
	World * m_world;
	std::string m_defaultPath; // default path to save folder


};

