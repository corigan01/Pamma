#pragma once

#include "Base.h"
#include "RandLi.h"

class Tile
{
public:
	Tile();
	~Tile();

	struct TileBase {
		Texture2D* texture;
		int Posx, Posy;
		float spawnrate;
	};
	struct Building {
		Texture2D* Textures;
		std::vector<int> Posx, Posy;
		int Sizex, Sizey;
		float spawnrate;
	};


	void AddNewTile(Texture2D* Texture, int Posx, int Posy, float spawnrate);
	void AddNewStructure(Texture2D *Textures, std::vector<int> Posx, std::vector<int> Posy, int SizeX, int SizeY, float spawnrate); 

	TileBase GetTexture(RandLi* rnd);
	Building GetBuilding(RandLi* rnd);



private:

	std::vector<Building> Buildings;
	std::vector<TileBase> Tiles;
};

