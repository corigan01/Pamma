#include "Tile.h"


Tile::Tile() {
	// nothing - defalt consturctor

}

Tile::~Tile() {
	// nothing - defalt deconsturctor

}

// Add a new Tile
void Tile::AddNewTile(Texture2D* Texture, int Posx, int Posy, float spawnrate) {
	Tiles.push_back({ Texture, Posx, Posy, spawnrate });
}

// Add a new building
void Tile::AddNewStructure(Texture2D* Textures, std::vector<int> Posx, std::vector<int> Posy, int SizeX, int SizeY, float spawnrate) {
	//std::cout << Posx[0] << std::endl;
	Buildings.push_back({ Textures, Posx, Posy, SizeX, SizeY, spawnrate });
}

// Get a random Tile
Tile::TileBase Tile::GetTexture(RandLi* rnd) {
	for (auto i : Tiles) {
		if (rnd->rndDouble(0, 1) > 1.0 - i.spawnrate) {
			return i;
		}
	}	
}

// Get a random bulding
Tile::Building Tile::GetBuilding(RandLi* rnd) {
	for (auto i : Buildings) {
		if (rnd->rndDouble(0, 1) > 1.0 - i.spawnrate) {
			return i;
		}
	}
}