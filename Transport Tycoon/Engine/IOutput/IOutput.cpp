#include <string>
#include <iostream>
#include <fstream>

#include <boost\filesystem.hpp>
#include "resources.h"

#include "world.h"
#include "programengine.h"
#include "programstatemain.h"
#include "IOutput.h"



IOutput::IOutput()
{

}

IOutput::IOutput(ng::ProgramEngine* engine, std::string defaultPath) : 
	m_defaultPath(defaultPath), m_engine(engine)
{
	
}



IOutput::~IOutput()
{
}

void IOutput::saveGameToFile(std::string filename)
{
	std::cout << "SAVED GOD" << std::endl;

	std::string finalPath = this->m_defaultPath + filename + ".svt";

	std::ofstream ofs(finalPath, std::fstream::out | std::fstream::binary);

	boost::archive::binary_oarchive oa(ofs);

	oa << m_world;


}

void IOutput::loadGameFromFile(std::string filename)
{
	std::cout << "Loading GOD" << std::endl;

	std::string finalPath = this->m_defaultPath + filename + ".svt";

	std::ifstream ifs(finalPath, std::fstream::binary | std::fstream::in);

	boost::archive::binary_iarchive ia(ifs);

	ia >> m_world;

	m_engine->pushState(new ProgramStateMain(m_world, m_engine));


	std::cout << "Stop";
}

void IOutput::getSaveList(std::vector<std::pair<std::string, std::string>>& m_file_list)
{	// Get inforamtion about all save files in a folder, as weel as their date and time creation

	m_file_list.clear(); // Clear variable to make new list

	std::cout << "IOutput::getSaveList" << std::endl;

	// Save folder adress
	std::string path = "C:\\Users\\vladt\\source\\repos\\Transport Tycoon\\Transport Tycoon\\saves";

	boost::filesystem::path targetDir(path);

	if (!path.empty())
	{
		boost::filesystem::path apk_path(path);
		boost::filesystem::recursive_directory_iterator end;

		for (boost::filesystem::recursive_directory_iterator i(apk_path); i != end; ++i)
		{
			const boost::filesystem::path cp = (*i);
			if (cp.extension().string() == ".svt")
			{
				struct tm time_info;
				char time[80]{};

				std::time_t t = boost::filesystem::last_write_time(cp);
				std::string filename = cp.stem().string();

				std::cout << filename << " : ";
				if (localtime_s(&time_info, &t) == 0)
				{
					strftime(time, sizeof(time), "%H:%M:%S %D", &time_info);

					std::pair<std::string, std::string> pair(filename, time);
					m_file_list.push_back(pair);
				}
				else
					std::cout << "error" << std::endl;
			}
		}
	}
	return;

}


