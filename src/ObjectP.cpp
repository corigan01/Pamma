#include "ObjectP.h"

/*
class ObjectP
{
public:
	Vector2 Pos = { 0, 0 };
	char Name[20] = ""; // we are maxing out the player name to 20 chars
	

	std::vector<int> Tilesx;
	std::vector<int> Tilesy;

	ObjectP();
	~ObjectP();

	std::string ConvertOUT();
	bool ConvertIN(std::string input); // we can return if it is valid or not



};


*/

ObjectP::ObjectP() {

}

ObjectP::~ObjectP() {

	  
}

std::string ObjectP::ConvertOUT() {
	return std::string(std::to_string(Pos.x) + "," + std::to_string(Pos.y) + "," + Name + "," + std::to_string(PlayerType));
	

}

bool ObjectP::ConvertIN(std::string input) {
	auto Ver = split(input, ',');
	if (Ver.size() == 4) {
		if (is_digits(Ver[0]) && is_digits(Ver[1]) && is_digits(Ver[3])) {
			Pos.x = std::stod(Ver[0]);
			Pos.y = std::stod(Ver[1]);
			PlayerType = std::stoi(Ver[3]);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}


}