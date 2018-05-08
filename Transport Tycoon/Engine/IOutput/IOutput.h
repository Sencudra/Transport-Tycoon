#pragma once

class World;
namespace ng { class ProgramEngine; }

class IOutput
{
public:

	IOutput();
	IOutput(ng::ProgramEngine* engine, std::string defaultPath);
	

	void saveGameToFile(std::string defaultPath);
	void loadGameFromFile(std::string defaultPath);

	void getSaveList(std::vector<std::pair<std::string, std::string>>& m_file_list);

	~IOutput();


	void updateWorld(World* world) { m_world = world; }


private:
	ng::ProgramEngine* m_engine;
	World* m_world;
	std::string m_defaultPath; // default path to save folder


};

