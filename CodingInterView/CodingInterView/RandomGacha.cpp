#include "RandomGacha.h"
#include <iostream>
#include <sstream>
#include <fstream>

void RandomGacha::ReadDoubleCSV(const std::string& _Path)
{
	std::ifstream _file(_Path);
	if (_file.fail()) { _file.close(); return; }

	std::string _cell;
	std::string _line;

	bool _Is_Once = false;

	while (std::getline(_file, _line))
	{
		std::stringstream _lineStream(_line);

		// 첫줄인 경우
		if (!_Is_Once)
		{
			// utf8(bom) 형식에서 처음 3단어를 제외하고 읽음
			char utf8bom[3] = {};
			std::streamsize _Size = _lineStream.readsome(utf8bom, 3);

			if (3 == _Size)
			{
				if ((char)0xEF == utf8bom[0] && (char)0xBB == utf8bom[1] && (char)0xBF == utf8bom[2])
				{
					_lineStream.seekg(3);
				}
			}

			_Is_Once = true;
		}

		while (std::getline(_lineStream, _cell, ','))
		{
			std::cout << _cell << std::endl;
		}
	}
}

void RandomGacha::SetStarForce(const std::string& _Path)
{
	ReadDoubleCSV(_Path);
}

void RandomGacha::SetRebirthFireOption(const std::string& _Path)
{

}

void RandomGacha::SetAdditionalOption(const std::string& _Path)
{

}

void RandomGacha::AddFixed_AdditionalOption(const std::string& _Option_Name)
{

}
