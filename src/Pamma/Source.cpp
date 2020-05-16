#include "Base.h"
#include "RandLi.h"
#include "Tile.h"
#include <CrossSockets.h>
#include "ObjectP.h"
#include "Item.h"

#define TileMapSize 256

Rectangle PosToRec(float x, float y) {
	return { (16.0f * x), (16.0f * y), 16, 16 };
}

displayout display;


enum PlayerDir {
	D_UP = 0,
	D_DOWN,
	D_RIGHT,
	D_LEFT
};

#define GameVersion 1



// FIX THIS MESS
int main() {
	
	CS_Client cl(1233, "127.0.0.1", true);
	
	display.out(D_DEBUG, R"(
__________                                 
\______   \_____    _____   _____ _____   
 |     ___/\__  \  /     \ /     \\__  \    
 |    |     / __ \|  Y Y  \  Y Y  \/ __ \_  
 |____|    (____  /__|_|  /__|_|  (____  / 
                \/      \/      \/     \/ 
Version 00.00.01  --  All rights reserved (c)

)");

	//SetConfigFlags(FLAG_FULLSCREEN_MODE);


	InitWindow(1366, 748, "Pamma v0.01");
	
	InitAudioDevice();


	RandLi Random;
	Tile TilesM;
	Texture2D TileMap = LoadTexture("Tiles.png");

	TilesM.AddNewTile(&TileMap, 12, 7, 0.50);
	TilesM.AddNewTile(&TileMap, 1, 10, 0.35);
	TilesM.AddNewTile(&TileMap, 9, 11, 0.35);
	TilesM.AddNewTile(&TileMap, 2, 10, 0.001);
	


	Rectangle FrameSize = { 0., 0., 16, 16};
	Vector2 FramePos = { 16, 16 };



	float xSect = GetScreenWidth() / 20;
	float ySect = GetScreenHeight() / 20;

	bool NotSupported = false;

	ObjectP Player;
	Player.Name = "WOW";

	SetTargetFPS(60);
	PlayerDir LastDir = D_DOWN;

	srand(time(0));
	uint32_t seed = 0;

	long long FrameCounter = 0;

	std::vector<ObjectP> OtherPlayers;
	std::string ErrorString = "";

	Player.Pos.x = 0;
	Player.Pos.y = 0;

	std::vector<Item> InvContents = {};


	std::vector<Item> Items;

	std::vector<std::string> FileContents;
	
	ReadFileVector(FileContents, "Items.txt");

	for (auto i : FileContents) {
		
		Item item;
		item.Name = i;
		item.StackSize = 100;
		Items.push_back(item);
	}


	bool IsInvBeingDisplayed = false;

	bool buttonholding = true;

	bool isLocked = false;
	Item WhatItemIsNeeded;

	SetExitKey(NULL);

	bool IsPlay = false;
	bool IsMulti = false;
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(DARKGREEN);

		if (IsSecond()) {
			seed = rand();
		}

		for (float y = 0; y < 40; y++) {
			for (float x = 0; x < 40; x++) {
				Vector2 GPos = { x + Player.Pos.x - 7, y + Player.Pos.y - 11 };
				Random.nProcGen = (int(Player.Pos.x + GPos.x + x) & 0xFFFF) << 16 | (int(Player.Pos.y + GPos.y + y) & 0xFFFF) + seed;



				Tile::TileBase tile = TilesM.GetTexture(&Random);


				DrawTexturePro(*tile.texture, PosToRec(tile.Posx, tile.Posy), { autoX(x * xSect + (-Player.Offset.x * 40) + 20), autoY(y * ySect + (-Player.Offset.y * 40) + 20), autoX xSect, autoY ySect }, { autoX 160, autoY 160 }, 0, WHITE);

				if (Random.rndDouble(0, 1) < 0.001) {
					DrawTexturePro(*tile.texture, PosToRec(10, 0), { autoX(x * xSect + (-Player.Offset.x * 40) + 20), autoY(y * ySect + (-Player.Offset.y * 40) + 20), autoX xSect, autoY ySect }, { autoX 160, autoY 160 }, 0, WHITE);

				}


			}
		}

		DrawText("Chest Quest", (GetScreenWidth() / 2) - (75*12)/2, autoY 50, autoB 75, DARKGREEN);
		DrawText("**How to play**\nUse UASD to move\nClick on Chests to collect them\nWatch for other people", autoX 1350, autoY 200, autoB 35, DARKGREEN);

		if (DrawButtonText({ 10, 300, 300, 100 }, 4, "Play", 35, 0, WHITE, GREEN, LIGHTGRAY, DARKGREEN)) {
			IsPlay = true;
			break;
		}
		if (DrawButtonText({ 10, 500, 300, 100 }, 4, "Quit", 35, 0, WHITE, GREEN, LIGHTGRAY, DARKGREEN)) {
			break;
		}
		

		EndDrawing();
	}

	if (!IsPlay && !IsMulti) {
		exit(0);
	}

	while (!WindowShouldClose()) {
		FrameCounter++;
		float movementMod = 0.253;


		if (cl.IsConnected()) {
			auto ver = cl.GetRec();
			for (auto i : ver) {
				try
				{

					if (ver.size() == 2) {
						if (Player.is_digits(ver[0]) && Player.is_digits(ver[1])) {
							if (std::stoi(ver[0]) != GameVersion) {
								NotSupported = true;
								cl.StopClient();
							}
							seed = std::stoi(ver[1]);
						}
					}
					else {
						OtherPlayers.clear();

						auto V = Player.split(i, '/');
						
						for (auto e : V) {
							ErrorString = e;
							ObjectP obj;
							if (obj.ConvertIN(e)) {
								
								OtherPlayers.push_back(obj);
							}
							else {
								display.out(D_ERROR, "Can not convert data from other player!");
							}
						}
					}
				}
				catch (const std::exception & e)
				{
					display.out(D_ERROR, e.what());
					display.out(D_ERROR, i);
					display.out(D_ERROR, ver[0]);
					display.out(D_ERROR, ver[1]);

					OtherPlayers.clear();
				}
				
			}
		}

		


		if (FrameCounter % 5 == 0 && cl.IsConnected() && !NotSupported) {
			cl.Send(Player.ConvertOUT());
		}

		if (IsKeyDown(KEY_W)) {
			Player.Offset.y -= movementMod;
			LastDir = D_UP;
		}
		if (IsKeyDown(KEY_S)) {
			Player.Offset.y += movementMod;
			LastDir = D_DOWN;
		}

		if (IsKeyDown(KEY_D)) {
			Player.Offset.x += movementMod;
			LastDir = D_RIGHT;
		}
		if (IsKeyDown(KEY_A)) {
			Player.Offset.x -= movementMod;
			LastDir = D_LEFT;
		}
		
		
		if (Player.Offset.x > autoX 2.4) {
			Player.Pos.x++;
			Player.Offset.x = 0;
		}
		if (Player.Offset.y > autoY 1.4) {
			Player.Pos.y++;
			Player.Offset.y = 0;
		}
		if (Player.Offset.x < autoX -2.4) {
			Player.Pos.x--;
			Player.Offset.x = 0;
		}
		if (Player.Offset.y < autoY -1.4) {
			Player.Pos.y--;
			Player.Offset.y = 0;
		}

		if (IsKeyPressed(KEY_E)) {
			IsInvBeingDisplayed = IsInvBeingDisplayed ? 0 : 1;
			display.out(D_DEBUG, "pressed");
		}
		

		BeginDrawing();
		ClearBackground(WHITE);
		
		int Hx = 0, Hy = 0;
		int Sx = 2, Sy = 2;

		bool Active = false;
		bool Drawing = false;

		
		Vector2 MousePos = GetMousePosition();



		for (float y = 0; y < 40; y++) {
			for (float x = 0; x < 40; x++) {
				Vector2 GPos = { x + Player.Pos.x - 7, y + Player.Pos.y - 11 };
				Random.nProcGen = (int(Player.Pos.x + GPos.x + x) & 0xFFFF) << 16 | (int(Player.Pos.y + GPos.y + y) & 0xFFFF) + seed;

				
		
				Tile::TileBase tile = TilesM.GetTexture(&Random);
				//Tile::Building building = TilesM.GetBuilding(&Random); // this is causing us issues

				if (tile.Posx == 2 && tile.Posy == 10) {
					Active = true;
					Hx = x;
					Hy = y;
					Drawing = true;
				}
				

				DrawTexturePro(*tile.texture, PosToRec(tile.Posx, tile.Posy), { autoX (x * xSect + (-Player.Offset.x * 40) + 20), autoY (y * ySect + (-Player.Offset.y * 40) + 20), autoX xSect, autoY ySect }, { autoX 160, autoY 160 }, 0, WHITE);
				
				try
				{
					for (auto i : OtherPlayers) {
						if (i.Pos.x == GPos.x && i.Pos.y == GPos.y) {
							DrawTexturePro(*tile.texture, PosToRec(12, 1), { autoX(x * xSect + (-Player.Offset.x * 40) + i.Offset.x * 40), autoY(y * ySect + (-Player.Offset.y * 40) + i.Offset.y * 40), autoX xSect, autoY ySect }, { autoX 160, autoY 160 }, 0, WHITE);

						}
					}
				}
				catch (const std::exception & e)
				{
					display.out(D_ERROR, e.what());
				}


				if (Random.rndDouble(0, 1) < 0.001) {
					DrawTexturePro(*tile.texture, PosToRec(10, 0), { autoX(x * xSect + (-Player.Offset.x * 40) + 20), autoY(y * ySect + (-Player.Offset.y * 40) + 20), autoX xSect, autoY ySect }, { autoX 160, autoY 160 }, 0, WHITE);
					
					if (CheckCollisionPointRec(MousePos, { autoX(x * xSect + (-Player.Offset.x * 40) + 20 - 160), autoY(y * ySect + (-Player.Offset.y * 40) + 20 - 160), autoX xSect, autoY ySect }) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && buttonholding) {
						IsInvBeingDisplayed = true;
						buttonholding = false;

						for (int i = 0; i < Random.rndInt(1, 3); i++) {
							auto Cont = Items[Random.rndInt(0, Items.size() - 1)];
							bool usingthis = true;
							for (auto e : InvContents) {
								if (Cont.Name == e.Name) {
									usingthis = false;
								}
							}
							if (usingthis)
								InvContents.push_back(Cont);
						}

					}
					else if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
						buttonholding = true;
					}
					
				}
				
				
				

				
				

				//DrawText(std::string(ErrorString).c_str(), 0, autoY 80, autoB 25, ORANGE);
				//DrawText(std::string(std::to_string(seed)).c_str(), 0, autoY 120, autoB 30, RED);
				//DrawText(std::string(std::to_string(int(GPos.x)) + ":" + std::to_string(int(GPos.y))).c_str(),  autoX (x * xSect + (-Player.Offset.x * 40)),  autoY (y * ySect + (-Player.Offset.y * 40)) - autoY 200, autoX 10, RED);


				// player movement
				switch (LastDir)
				{
				case D_UP:
					DrawTexturePro(TileMap, PosToRec(12.f, 1.f), { 0, 0, xSect, ySect }, { -GetScreenWidth() / 2.f + 40, -GetScreenHeight() / 2.f }, 0, WHITE);
					break;
				case D_DOWN:
					DrawTexturePro(TileMap, PosToRec(12.f, 5.f), { 0, 0, xSect, ySect }, { -GetScreenWidth() / 2.f + 40, -GetScreenHeight() / 2.f }, 0, WHITE);
					break;
				case D_RIGHT:
					DrawTexturePro(TileMap, PosToRec(2.f, 0.f), { 0, 0, xSect, ySect }, { -GetScreenWidth() / 2.f + 40, -GetScreenHeight() / 2.f }, 0, WHITE);
					break;
				case D_LEFT:
					DrawTexturePro(TileMap, PosToRec(4.f, 4.f), { 0, 0, xSect, ySect }, { -GetScreenWidth() / 2.f + 40, -GetScreenHeight() / 2.f }, 0, WHITE);
					break;

				default:
					break;
				}
				

			}
		}
		//DrawTexturePro(TileMap, PosToRec(4, 0), { 0+40, 0+40, 240+40, 240+40 }, { 0, 0 }, 0, WHITE);

		
		DrawFPS(0, 0);
		//DrawText(std::string("XY: " + std::to_string(Player.Pos.x) + " : " + std::to_string(Player.Pos.y)).c_str(), 0, 40, 25, GREEN);
		DrawText(std::string(Player.ConvertOUT()).c_str(), autoX 0, autoY 40, autoB 25, GREEN);

		if (!cl.IsConnected() && !cl.IsFailedToConnect() && !NotSupported)
			DrawText("Connecting...", autoX(1920 - 250), autoX 10, autoY 25, ORANGE);
		else if (cl.IsFailedToConnect())
			DrawText("Failed to Connect to Server!", autoX(1920 - 370), autoX 10, autoY 25, RED);
		else if (NotSupported)
			DrawText("Game Not Supported!", autoX(1920 - 370), autoX 10, autoY 25, RED);
		else
			DrawText("Connected to Server!", autoX(1920 - 380), autoX 10, autoY 25, GREEN);

		

		if (IsInvBeingDisplayed) {
			DrawRectangleGradientEx({ autoX 500, autoY 200, autoX 1000, autoY 700 }, GRAY, DARKGRAY, DARKGRAY, GRAY);

			for (int e = 0; e < InvContents.size(); e++) {
				DrawText(InvContents[e].Name.c_str(), autoX 550, autoY(300 + e * 30), autoB 15, WHITE);
			}

			DrawRectangleLinesEx({ autoX 500, autoY 200, autoX 1000, autoY 700 }, autoB 4, BLACK);
			DrawText("Inventory", autoX 550, autoY 220, autoB 35, WHITE);
			DrawLine(autoX 550, autoY 280, autoX 1000, autoY 280, WHITE);
			
			
		}

		

		EndDrawing();
	}

	exit(0);
	//cl.~CS_Client();

}


