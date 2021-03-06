#include "map.h"

baseTile::baseTile()
{
	tileID = TileType::BASE;
}

baseTile::~baseTile()
{

}

void baseTile::Update(float seconds)
{

}

void baseTile::Draw(float x, float y)
{
	sprite->Blit(x, y);
}

void TileMap::Draw()
{
	for (int y = 0; y < MAPHEIGHT; ++y)
	{
		for (int x = 0; x < MAPWIDTH; ++x)
		{
			theMap[x][y]->Draw(x * TILESIZE + TILESIZE / 2, 1080 - (y * TILESIZE + TILESIZE / 2 + 25));
			if (theMap[x][y]->onFire) flame.Draw(x * TILESIZE + TILESIZE / 2, 1080 - (y * TILESIZE + TILESIZE / 2 + 25));
		}
	}
}

void TileMap::Update(float seconds)
{
	flame.Update(seconds);

	for (int y = 0; y < MAPHEIGHT; ++y)
	{
		for (int x = 0; x < MAPWIDTH; ++x)
		{
			theMap[x][y]->Update(seconds);
		}
	}
}

bool TileMap::SpreadFire(std::vector<Vent> vent)
{

	bool touchedPlayer = false;
	int xOffSet[4] = { -1,1,0,0 };
	int yOffSet[4] = { 0,0,-1,1 };
	int x = 0;
	int y = 0;
	int nowX = 0;
	int nowY = 0;
	//get current vent's location
	for (int i = 0; i < vent.size(); i++)
	{
		x = vent[i].x;
		y = vent[i].y;
		//do checks on contiguous tiles 
		for (int dir = 0; dir < 4; ++dir)
		{
			nowX = x + xOffSet[dir];
			nowY = y + yOffSet[dir];

			if (nowX < MAPWIDTH && nowY < MAPHEIGHT)
			{
				// passable tile but Not on flame
				if (theMap[nowX][nowY]->passable == true && theMap[nowX][nowY]->onFire == false)
				{
					// set the tile on fire
					theMap[nowX][nowY]->onFire = true;

					// check if player is on fire
					touchedPlayer = isPlayerOnFlame(nowX, nowY);
					if (touchedPlayer) // if yes, then no need to keep expanding the fire
						break;
				}
				// passable tile but Not on flame
				else if (theMap[nowX][nowY]->passable == true && theMap[nowX][nowY]->onFire == true)
				{
					// check if player is on fire
					touchedPlayer = isPlayerOnFlame(nowX, nowY);
					if (touchedPlayer)
						break;
				}
			}
		}
	}
	return touchedPlayer;
}

bool TileMap::isPlayerOnFlame(int nowX, int nowY)
{
	bool isOnFlame = false;
	if (player.x == nowX && player.y == nowY)
	{
		isOnFlame = true;
	}
	return isOnFlame;
}

bool TileMap::LoadLevel(std::string filename, int &playerX, int &playerY)
{
	std::ifstream mapFile(filename);
	if (mapFile.is_open())
	{
		for (int y = 0; y < MAPHEIGHT; ++y)
		{
			for (int x = 0; x < MAPWIDTH; ++x)
			{
				int tileNum = 0;
				mapFile >> tileNum;
				//TileType{ BASE, SPACE, FLOOR, WALL, DOORH, DOORV, VENT, EXIT };
				switch (tileNum)
				{
				case (int)TileType::BASE:
					assert(0 && "BASE tile? Really?");
					break;
				case (int)TileType::FLOOR:
					theMap[x][y] = new FloorTile();
					break;
				case (int)TileType::WALL:
					theMap[x][y] = new Wall();
					break;
				case (int)DoorType::R_V_CLOSED:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::RED;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::VERTICAL;
					((Door*)theMap[x][y])->passable = false;
					break;
				case (int)DoorType::R_V_OPEN:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::RED;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::VERTICAL;
					((Door*)theMap[x][y])->passable = true;
					break;
				case (int)DoorType::R_H_CLOSED:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::RED;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::HOREZONTAL;
					((Door*)theMap[x][y])->passable = false;
					break;
				case (int)DoorType::R_H_OPEN:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::RED;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::HOREZONTAL;
					((Door*)theMap[x][y])->passable = true;
					break;
				case (int)DoorType::B_V_CLOSED:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::BLUE;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::VERTICAL;
					((Door*)theMap[x][y])->passable = false;
					break;
				case (int)DoorType::B_V_OPEN:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::BLUE;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::VERTICAL;
					((Door*)theMap[x][y])->passable = true;
					break;
				case (int)DoorType::B_H_CLOSED:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::BLUE;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::HOREZONTAL;
					((Door*)theMap[x][y])->passable = false;
					break;
				case (int)DoorType::B_H_OPEN:
					theMap[x][y] = new Door();
					((Door*)theMap[x][y])->color = (int)DoorColor::BLUE;
					((Door*)theMap[x][y])->orientation = (int)DoorOrientation::HOREZONTAL;
					((Door*)theMap[x][y])->passable = true;
					break;
				case (int)TileType::VENT:
					theMap[x][y] = new Vent();
					break;
				case (int)TileType::EXIT:
					theMap[x][y] = new Exit();
					break;
				case (int)TileType::SPACE:
					theMap[x][y] = new Space();
					break;
				default:
					assert(0 && "Unknow tile? Really?");
					break;
				}
			}

		}
		mapFile >> playerX;
		mapFile >> playerY;
		mapFile.close();
	}

	else std::cout << "Unable to open file";

	return true; //successfully loaded map
}

void TileMap::SaveLevel(std::string filename, int playerX, int playerY)
{
	std::ofstream mapFile(filename);
	if (mapFile.is_open())
	{
		//mapFile << (int)theMap[x][y]->tileID;
		for (int y = 0; y < MAPHEIGHT; ++y)
		{
			for (int x = 0; x < MAPWIDTH; ++x)
			{
				//TileType { BASE, SPACE, FLOOR, WALL, VENT, EXIT, DOOR, END_ENUM};
				switch ((int)theMap[x][y]->tileID) {
				case  (int)TileType::DOOR:
				{
					Door* door = (Door*)theMap[x][y];
					if (door->passable)
					{
						if (door->orientation == (int)DoorOrientation::VERTICAL)
						{
							if (door->color == (int)DoorColor::RED)
							{
								mapFile << (int)DoorType::R_V_OPEN << " ";
							}
							else
							{
								mapFile << (int)DoorType::B_V_OPEN << " ";
							}
						}
						else
						{
							if (door->color == (int)DoorColor::RED)
							{
								mapFile << (int)DoorType::R_H_OPEN << " ";
							}
							else
							{
								mapFile << (int)DoorType::B_H_OPEN << " ";
							}
						}
					}
					else
					{
						if (door->orientation == (int)DoorOrientation::VERTICAL)
						{
							if (door->color == (int)DoorColor::RED)
							{
								mapFile << (int)DoorType::R_V_CLOSED << " ";
							}
							else
							{
								mapFile << (int)DoorType::B_V_CLOSED << " ";
							}
						}
						else
						{
							if (door->color == (int)DoorColor::RED)
							{
								mapFile << (int)DoorType::R_H_CLOSED << " ";
							}
							else
							{
								mapFile << (int)DoorType::B_H_CLOSED << " ";
							}
						}
					}
					break;
				}
				default:
					mapFile << (int)theMap[x][y]->tileID << " ";
					break;
				}
			}
			mapFile << '\n';
		}
		mapFile << playerX << " " << playerY;
		mapFile.close();
	}
	else std::cout << "Unable to open file";

}

TileMap::~TileMap()
{
	for (int y = 0; y < MAPHEIGHT; ++y)
	{
		for (int x = 0; x < MAPWIDTH; ++x)
		{
			delete theMap[x][y];
		}
	}
}
