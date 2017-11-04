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

Sprite* spaceSprite;
Sprite* floorSprite;
Sprite* wallSprite;
Sprite* ventSprite;
Sprite* exitSprite;

Sprite* door_Red_V_Open_Sprite;
Sprite* door_Red_V_Closed_Sprite;
Sprite* door_Red_H_Open_Sprite;
Sprite* door_Red_H_Closed_Sprite;

Sprite* door_Blue_V_Open_Sprite;
Sprite* door_Blue_V_Closed_Sprite;
Sprite* door_Blue_H_Open_Sprite;
Sprite* door_Blue_H_Closed_Sprite;

Sprite* playerSprite;

int playerX = 4;
int playerY = 4;

void Init()
{
	tileMap = new TileMap();
	playerSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\Character_Idle_Front.png");

	door_Blue_H_Closed_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\H_Door_Blue_Close-1.png");
	door_Blue_H_Open_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\H_Door_Blue_Open.png");
	door_Blue_V_Closed_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\V_Door_Blue_Close-1.png");
	door_Blue_V_Open_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\V_Door_Blue_Open.png");

	door_Red_H_Closed_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\H_Door_Red_Close-1.png");
	door_Red_H_Open_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\H_Door_Red_Open.png");
	door_Red_V_Closed_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\V_Door_Red_Close-1.png");
	door_Red_V_Open_Sprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\V_Door_Red_Open.png");
	
	spaceSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\Space_Tile_A.png");
	floorSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\Floor_Tile_Base.png");
	wallSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\Wall_Tile_A.png");
	ventSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\Vent_Tile_Glow.png");
	exitSprite = blit3D->MakeSprite(0, 0, 64, 64, "Media\\Exit_Tile.png");

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
			case TileType::DOOR:
			{
				Door* door = (Door*)tileMap->theMap[x][y];

				if (door->color == (int)DoorColor::RED)
				{
					if (door->orientation == (int)DoorOrientation::HOREZONTAL)
					{
						door->sprite = door_Red_H_Closed_Sprite;
						door->open = door_Red_H_Open_Sprite;
					}
					else
					{
						door->sprite = door_Red_V_Closed_Sprite;
						door->open = door_Red_V_Open_Sprite;
					}
				}
				else
				{
					if (door->orientation == (int)DoorOrientation::HOREZONTAL)
					{
						door->sprite = door_Blue_H_Closed_Sprite;
						door->open = door_Blue_H_Open_Sprite;
					}
					else
					{
						door->sprite = door_Blue_V_Closed_Sprite;
						door->open = door_Blue_V_Open_Sprite;
					}
				}				
			}
				break;

			case TileType::SPACE:
				tileMap->theMap[x][y]->sprite = spaceSprite;
				break;

			case TileType::FLOOR:
				tileMap->theMap[x][y]->sprite = floorSprite;
				break;

			case TileType::WALL:
				tileMap->theMap[x][y]->sprite = wallSprite;
				break;

			case TileType::VENT:
				tileMap->theMap[x][y]->sprite = ventSprite;
				break;

			case TileType::EXIT:
				tileMap->theMap[x][y]->sprite = exitSprite;
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

	int cx = (int)mx / TILESIZE * TILESIZE + TILESIZE/2;
	int cy = 1080 - my;
	cy -= 25;
	cy = 1080 - ((int)cy / TILESIZE * TILESIZE + TILESIZE/2 + 25);

	switch (editTileNum)
	{
	case (int)TileType::EXIT:
		exitSprite->Blit(cx, cy);
		break;

	case (int)TileType::VENT:
		ventSprite->Blit(cx, cy);
		break;

	case (int)TileType::WALL:
		wallSprite->Blit(cx, cy);
		break;

	case (int)TileType::SPACE:
		spaceSprite->Blit(cx, cy);
		break;

	case (int)TileType::FLOOR:
		floorSprite->Blit(cx, cy);
		break;

	case (int)DoorType::B_H_CLOSED:
		door_Blue_H_Closed_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::B_H_OPEN:
		door_Blue_H_Open_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::B_V_CLOSED:
		door_Blue_V_Closed_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::B_V_OPEN:
		door_Blue_V_Open_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::R_H_CLOSED:
		door_Red_H_Closed_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::R_H_OPEN:
		door_Red_H_Open_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::R_V_CLOSED:
		door_Red_V_Closed_Sprite->Blit(cx, cy);
		break;

	case (int)DoorType::R_V_OPEN:
		door_Red_V_Open_Sprite->Blit(cx, cy);
		break;
	}

	//draw player start location
	playerSprite->Blit(playerX * TILESIZE + TILESIZE / 2, 1080 - (playerY * TILESIZE + TILESIZE / 2 + 25));
	
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		tileMap->SaveLevel("level1.txt", playerX, playerY);

	if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		//delete old map
		delete tileMap;
		tileMap = new TileMap();
		tileMap->LoadLevel("level1.txt", playerX, playerY);
		for (int y = 0; y < MAPHEIGHT; ++y)
			for (int x = 0; x < MAPWIDTH; ++x)
			{
				switch (tileMap->theMap[x][y]->tileID)
				{
				case TileType::DOOR:
				{
					Door* door = (Door*)tileMap->theMap[x][y];

					if (door->color == (int)DoorColor::RED)
					{
						if (door->orientation == (int)DoorOrientation::HOREZONTAL)
						{
							door->sprite = door_Red_H_Closed_Sprite;
							door->open = door_Red_H_Open_Sprite;
						}
						else
						{
							door->sprite = door_Red_V_Closed_Sprite;
							door->open = door_Red_V_Open_Sprite;
						}
					}
					else
					{
						if (door->orientation == (int)DoorOrientation::HOREZONTAL)
						{
							door->sprite = door_Blue_H_Closed_Sprite;
							door->open = door_Blue_H_Open_Sprite;
						}
						else
						{
							door->sprite = door_Blue_V_Closed_Sprite;
							door->open = door_Blue_V_Open_Sprite;
						}
					}
				}
				break;

				case TileType::SPACE:
					tileMap->theMap[x][y]->sprite = spaceSprite;
					break;

				case TileType::FLOOR:
					tileMap->theMap[x][y]->sprite = floorSprite;
					break;

				case TileType::WALL:
					tileMap->theMap[x][y]->sprite = wallSprite;
					break;

				case TileType::VENT:
					tileMap->theMap[x][y]->sprite = ventSprite;
					break;

				case TileType::EXIT:
					tileMap->theMap[x][y]->sprite = exitSprite;
					break;

				default:
					assert(false && "Unknown tile id!");
					break;
				}
			}
	}
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
		playerX = x;
		playerY = y;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		int x = mx / TILESIZE;
		int y = (1080 - 25 - my) / TILESIZE;

		delete tileMap->theMap[x][y];
		if (editTileNum < 100)
		{
			switch (editTileNum)
			{
			case (int)TileType::DOOR:
				assert(0 && "ERROR...doors should be handled elswhere");
				break;

			case (int)TileType::SPACE:
				tileMap->theMap[x][y] = new Space();
				tileMap->theMap[x][y]->sprite = spaceSprite;
				break;

			case (int)TileType::FLOOR:
				tileMap->theMap[x][y] = new FloorTile();
				tileMap->theMap[x][y]->sprite = floorSprite;
				break;

			case (int)TileType::VENT:
				tileMap->theMap[x][y] = new Vent();
				tileMap->theMap[x][y]->sprite = ventSprite;
				break;

			case (int)TileType::WALL:
				tileMap->theMap[x][y] = new Wall();
				tileMap->theMap[x][y]->sprite = wallSprite;
				break;

			case (int)TileType::EXIT:
				tileMap->theMap[x][y] = new Exit();
				tileMap->theMap[x][y]->sprite = exitSprite;
				break;

			default:
				assert(false && "Unknown tile id!");
				break;
			}
		}
		else
		{
			//make a door
			Door* door = new Door();
			tileMap->theMap[x][y] = door;
			switch (editTileNum)
			{
			case (int)DoorType::B_H_CLOSED:
				door->sprite = door_Blue_H_Closed_Sprite;
				door->open = door_Blue_H_Open_Sprite;
				door->orientation = (int)DoorOrientation::HOREZONTAL;
				door->color = (int)DoorColor::BLUE;
				door->passable = false;
				break;

			case (int)DoorType::B_H_OPEN:
				door->sprite = door_Blue_H_Closed_Sprite;
				door->open = door_Blue_H_Open_Sprite;
				door->orientation = (int)DoorOrientation::HOREZONTAL;
				door->color = (int)DoorColor::BLUE;
				door->passable = true;
				break;

			case (int)DoorType::B_V_CLOSED:
				door->sprite = door_Blue_V_Closed_Sprite;
				door->open = door_Blue_V_Open_Sprite;
				door->orientation = (int)DoorOrientation::VERTICAL;
				door->color = (int)DoorColor::BLUE;
				door->passable = false;
				break;

			case (int)DoorType::B_V_OPEN:
				door->sprite = door_Blue_V_Closed_Sprite;
				door->open = door_Blue_V_Open_Sprite;
				door->orientation = (int)DoorOrientation::VERTICAL;
				door->color = (int)DoorColor::BLUE;
				door->passable = true;
				break;

				//red
			case (int)DoorType::R_H_CLOSED:
				door->sprite = door_Red_H_Closed_Sprite;
				door->open = door_Red_H_Open_Sprite;
				door->orientation = (int)DoorOrientation::HOREZONTAL;
				door->color = (int)DoorColor::RED;
				door->passable = false;
				break;

			case (int)DoorType::R_H_OPEN:
				door->sprite = door_Red_H_Closed_Sprite;
				door->open = door_Red_H_Open_Sprite;
				door->orientation = (int)DoorOrientation::HOREZONTAL;
				door->color = (int)DoorColor::RED;
				door->passable = true;
				break;

			case (int)DoorType::R_V_CLOSED:
				door->sprite = door_Red_V_Closed_Sprite;
				door->open = door_Red_V_Open_Sprite;
				door->orientation = (int)DoorOrientation::VERTICAL;
				door->color = (int)DoorColor::RED;
				door->passable = false;
				break;

			case (int)DoorType::R_V_OPEN:
				door->sprite = door_Red_V_Closed_Sprite;
				door->open = door_Red_V_Open_Sprite;
				door->orientation = (int)DoorOrientation::VERTICAL;
				door->color = (int)DoorColor::RED;
				door->passable = true;
				break;
			}
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
		if (editTileNum == (int)TileType::DOOR)
			editTileNum = (int)DoorType::R_V_CLOSED;
		else if (editTileNum > (int)DoorType::B_H_OPEN)
			editTileNum = (int)TileType::BASE + 1;
		
	}
	else if (yoffset < 0)
	{
		//scrolled down
		editTileNum--;	
		if (editTileNum <= (int)TileType::BASE)
			editTileNum = (int)DoorType::B_H_OPEN;
		else if(editTileNum == 99 )
			editTileNum = (int)TileType::DOOR - 1;
	}
}


int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 1920, 1080);

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