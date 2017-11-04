/*
	Simple example of loading/rotating/displaying sprites in Blit3D
*/
//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"

#include "map.h"
#include "Floor.h"

Blit3D *blit3D = NULL;

//GLOBAL DATA
//mouse coordinates
float mx = 0, my = 0;

TileMap* tileMap;

int editTileNum = 1;

Sprite* hdoorSprite;
Sprite* spaceSprite;
Sprite* floorSprite;

void Init()
{
	tileMap = new TileMap();

	hdoorSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\H_Door_Close.png");
	spaceSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\V_Door_Close.png");
	floorSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\V_Door_Close.png");

	//tileMap.LoadLevel("level1.txt");

	for (int y = 0; y < MAPHEIGHT; ++y)
		for (int x = 0; x < MAPWIDTH; ++x)
		{
			tileMap->theMap[x][y] = new FloorTile;
			tileMap->theMap[x][y]->tileID = TileType::FLOOR;
		}

	for(int y = 0; y < MAPHEIGHT; ++y)
		for (int x = 0; x < MAPWIDTH; ++x)
		{
			switch (tileMap->theMap[x][y]->tileID)
			{
			case TileType::DOORH:
				tileMap->theMap[x][y]->sprite = hdoorSprite;
				break;

			case TileType::SPACE:
				tileMap->theMap[x][y]->sprite = spaceSprite;
				break;

			case TileType::FLOOR:
				tileMap->theMap[x][y]->sprite = floorSprite;
				break;

			default:
				assert(false && "Unknown tile id!");
				break;
			}
		}
}

void DeInit(void)
{
	delete tileMap;
	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}

void Update(double seconds)
{
	tileMap->Update((float)seconds);
}

void Draw(void)
{
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);	//clear colour: r,g,b,a 	
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw stuff here
	tileMap->Draw();

	switch (editTileNum)
	{
	case (int)TileType::SPACE:
		spaceSprite->Blit(mx, my);
		break;

	case (int)TileType::DOORH:
		hdoorSprite->Blit(mx, my);
		break;

	case (int)TileType::FLOOR:
		floorSprite->Blit(mx, my);
		break;
	}
	
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence
}

void DoCursor(double x, double y)
{
	mx = (float)x;
	my = blit3D->trueScreenHeight - (float)y; //invert y for Blit3D screen coords

											  //scale, in case screen resolution does not match our mode
	mx = mx * (blit3D->screenWidth / blit3D->trueScreenWidth);
	my = my * (blit3D->screenHeight / blit3D->trueScreenHeight);
}

void DoMouseButton(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		int x = mx / TILESIZE;
		int y = (1080 - 25 - my) / TILESIZE;
		editTileNum = (int)tileMap->theMap[x][y]->tileID;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		int x = mx / TILESIZE;
		int y = (1080 - 25 - my) / TILESIZE;

		tileMap->theMap[x][y]->tileID = (TileType)editTileNum;

		switch (tileMap->theMap[x][y]->tileID)
		{
		case TileType::DOORH:
			tileMap->theMap[x][y]->sprite = hdoorSprite;
			break;

		case TileType::SPACE:
			tileMap->theMap[x][y]->sprite = spaceSprite;
			break;

		case TileType::FLOOR:
			tileMap->theMap[x][y]->sprite = floorSprite;
			break;

		default:
			assert(false && "Unknown tile id!");
			break;
		}
	}
	
}

void DoScrollwheel(double xoffset, double yoffset)
{
	//A simple mouse wheel, being vertical, provides offsets along the Y-axis.
	if (yoffset > 0)
	{
		//scrolled up
		editTileNum++;
		if (editTileNum >= (int)TileType::ENDOFLIST)
			editTileNum = (int)TileType::BASE + 1;
		
	}
	else if (yoffset < 0)
	{
		//scrolled down
		editTileNum--;	
		if (editTileNum <= (int)TileType::BASE)
			editTileNum = (int)TileType::BASE + 1;
	}
}


int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 640, 400);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);

	blit3D->SetDoCursor(DoCursor);
	blit3D->SetDoMouseButton(DoMouseButton);
	blit3D->SetDoScrollwheel(DoScrollwheel);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}