#include "pch.h"
#include "TileMap.h"
#include "Player.h"
#include <random>

TileMap::TileMap(string filename)
{
	ifstream in(filename);

	int cnt = 0;
	char tile;
	while (!in.eof()) {
		in >> tile;
		if (cnt < TILE_NUM_W * TILE_NUM_H) {
			m_Tiles[cnt % TILE_NUM_W][cnt / TILE_NUM_W] = tile;
			++cnt;
		}
	}

	for (int i = 0; i < TILE_NUM_H; ++i){
		for (int j = 0; j < TILE_NUM_W; ++j){
			m_mmObjPos.emplace(static_cast<TILE_TYPE>(m_Tiles[i][j]), Vec2{ (float)j * TILE_SIZE, (float)i * TILE_SIZE });
		}
	}
}

TileMap::~TileMap()
{
}

void TileMap::CreateGun()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution<int> uid(0, TILE_NUM_W * TILE_NUM_H); //625
	int iRand = uid(dre);
	while (m_Tiles[iRand / TILE_NUM_W][iRand % TILE_NUM_W] != 'S')
	{
		iRand = uid(dre);
	}
	m_GunPos.x = iRand / TILE_NUM_W * TILE_SIZE;
	m_GunPos.y = iRand % TILE_NUM_W * TILE_SIZE;
	cout << iRand / TILE_NUM_W << endl;
	cout << iRand % TILE_NUM_W << endl;
	//Is it need?
	m_Tiles[iRand / TILE_NUM_W][iRand % TILE_NUM_W] = 'A';

}

void TileMap::Collide_Wall(Player* in)
{
	Vec2 inLT = { in->GetPos().x + TILE_SIZE / 4, in->GetPos().y};
	Vec2 inRB = { in->GetPos().x + TILE_SIZE * 0.75f, in->GetPos().y + TILE_SIZE };

	auto range = m_mmObjPos.equal_range(TILE_TYPE::WALL);
	for (auto i = range.first; i != range.second; ++i){
		Vec2 wallLT = {i->second.x, i->second.y};
		Vec2 wallRB = {i->second.x + TILE_SIZE, i->second.y + TILE_SIZE};

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y < wallRB.y) continue;
		else if (inRB.y > wallLT.y) continue;
		else{
			Vec2 figure = {min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (inLT.x > wallLT.x) updatePos.x += figure.x;
			else updatePos.x -= figure.x;
			if (inLT.y > wallLT.y) updatePos.y -= figure.y;
			else updatePos.y += figure.y;
			in->SetPos(updatePos);
		}
	}
}

