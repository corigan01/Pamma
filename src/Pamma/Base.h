#pragma once


#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ostream>
#include <time.h>  
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdio>
#include <sstream>


#define autoX float((float)GetScreenWidth()/1920.0f) *
#define autoY float((float)GetScreenHeight()/1080.0f) *
#define autoB float((((float)GetScreenWidth()/1920.0f) + ((float)GetScreenHeight() / 1080.0f)) / 2.0f) *


// Read a file
bool ReadFileVector(std::vector<std::string>& FileData, std::string FileName) {

	std::string line;
	std::ifstream myfile(FileName);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			FileData.push_back(line);
		}
		myfile.close();
		return true;
	}
	
	return false;
}

// Draws text
void DrawTextC(std::string text, int textsize, Vector2 pos, Color color) {
	DrawText(text.c_str(), autoX pos.x, autoY pos.y, autoB textsize, color);
}

bool buttonState = true;
bool DrawButtonS(Rectangle Pos, int thickness, Color Forground1, Color Forground2, Color AccColor1) {
	bool turning = false;

	Pos = { autoX Pos.x, autoY Pos.y, autoX Pos.width, autoY Pos.height };

	Color color = Forground1;

	Vector2 MousePos = GetMousePosition();


	if (CheckCollisionPointRec(MousePos, Pos)) {
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			color = Forground2;
			if (buttonState) {
				turning = true;
				buttonState = false;
			}
		}
		thickness += autoB 3;
	}

	if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
		buttonState = true;
	}


	DrawRectangleRounded(Pos, 0.6, 1, color);

	Pos = { Pos.x + 2, Pos.y + 2, Pos.width - 4, Pos.height - 4 };
	DrawRectangleRoundedLines(Pos, 0.6, 1, autoB thickness * 0.5, AccColor1);

	return turning;
}


// Draws scalled text on a scalled button
bool DrawButtonText(Rectangle Pos, int thickness, std::string text, int textsize, int textcolornumber, Color TextColor, Color Forground1, Color Forground2, Color AccColor1) {
	bool turning = DrawButtonS(Pos, thickness, Forground1, Forground2, AccColor1);


	DrawTextC(text.c_str(), textsize, { (Pos.x + (Pos.width / 2) - MeasureText(text.c_str(), textsize) / 2),  ((Pos.y + (Pos.height / 2)) - textsize * 0.5f) }, TextColor);


	return turning;
}

int LastClockTime = 0;
int LastClockTimeM = 0;
// turns true 10 times every second
bool IsSecond() {
	if (int(GetTime()) != LastClockTimeM) {
		LastClockTimeM = GetTime();
		return true;
	}
	return false;
}