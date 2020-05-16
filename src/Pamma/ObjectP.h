#pragma once

#include "Base.h"




class ObjectP
{
public:
	Vector2 Pos = { 0, 0 };
	Vector2 Offset = { 0, 0 };
	std::string Name; // 10 chars MAX
	Color Tint = WHITE;
	
	int PlayerType = 0;

	ObjectP();
	~ObjectP();

	std::string ConvertOUT();
	bool ConvertIN(std::string input); // we can return if it is valid or not

	// spits a string
	std::vector<std::string> split(std::string strToSplit, char delimeter)
	{
		std::stringstream ss(strToSplit);
		std::string item;
		std::vector<std::string> splittedStrings;
		while (std::getline(ss, item, delimeter))
		{
			splittedStrings.push_back(item);
		}
		return splittedStrings;
	}

	bool is_digits(const std::string& str)
	{
		if (str.find_first_not_of("-0123456789.") != std::string::npos) {
			return false;
		}
		return true;
	}

private:


	

	

};

