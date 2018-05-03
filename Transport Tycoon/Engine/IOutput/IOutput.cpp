#include "IOutput.h"
#include "map.h"

#include <string>
#include <iostream>
#include <boost\filesystem.hpp>

#ifdef WIN32
#define stat _stat
#endif


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


