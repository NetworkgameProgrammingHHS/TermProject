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
			m_Tiles[cnt / TILE_NUM_W][cnt % TILE_NUM_W] = tile;
			++cnt;
		}
	}

	for (int i = 0; i < TILE_NUM_H; ++i){
		for (int j = 0; j < TILE_NUM_W; ++j){
			m_mmObjPos.emplace(static_cast<int>(m_Tiles[i][j]), Vec2{ (float)j * TILE_SIZE, (float)i * TILE_SIZE });
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
	int indexX = iRand / TILE_NUM_W;
	int indexY = iRand % TILE_NUM_W;
	// testing
	//int indexX = 17;
	//int indexY = 11;
	m_GunPos.x = indexY * TILE_SIZE;
	m_GunPos.y = indexX * TILE_SIZE;
	//cout << iRand / TILE_NUM_W << endl;
	//cout << iRand % TILE_NUM_W << endl;
	//Is it need?

	m_Tiles[indexX][indexY] = 'A';
	m_mmObjPos.emplace(static_cast<int>(m_Tiles[indexX][indexY]), Vec2{ (float)indexY * TILE_SIZE, (float)indexX * TILE_SIZE });
}

void TileMap::Collide_Wall(Player* in)
{
	Vec2 inLT = { in->GetPos().x + TILE_SIZE / 4 + 3, in->GetPos().y + 3};
	Vec2 inRB = { in->GetPos().x + TILE_SIZE * 0.75f - 3, in->GetPos().y + TILE_SIZE - 3 };

	bool isCollide = false;

	auto range = m_mmObjPos.equal_range(static_cast<int>('D'));
	for (auto i = range.first; i != range.second; ++i){
		Vec2 wallLT = {i->second.x - 1, i->second.y};
		Vec2 wallRB = {i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE};

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else {
			Vec2 figure = {min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
					if (in->GetSuperJump())
					{
						in->SetSuperJump(false);
						in->SetJumpChange(10);
					}
				}
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
			break;
		}
	}
	
	//if ((!isCollide && !(in->GetJump() && in->IsJumpUp())) || in->GetFall()) {
	//	bool FallCollide = false;
	//	Vec2 FallLT = { in->GetPos().x + TILE_SIZE / 4, in->GetPos().y + TILE_SIZE };
	//	Vec2 FallRB = { in->GetPos().x + TILE_SIZE * 0.75f, in->GetPos().y + TILE_SIZE + 3 };
	//	for (auto i = range.first; i != range.second; ++i) {
	//		Vec2 wallLT = { i->second.x - 1, i->second.y };
	//		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

	//		if (FallLT.x > wallRB.x) continue;
	//		else if (FallRB.x < wallLT.x) continue;
	//		else if (FallLT.y > wallRB.y) continue;
	//		else if (FallRB.y < wallLT.y) continue;
	//		else if (FallLT.y < wallLT.y) continue;
	//		else {
	//			FallCollide = true;
	//			if (in->GetID() == 2) {
	//				cout << "Jump : " << in->GetJump();
	//				cout << "Fall : " << in->GetFall();
	//			}
	//			in->SetJump(false);
	//			in->SetFall(false);
	//			if (in->GetSuperJump())
	//			{
	//				in->SetSuperJump(false);
	//				in->SetJumpChange(10);
	//			}
	//			//in->SetPos({ in->GetPos().x, wallLT.y + TILE_SIZE });
	//			break;
	//		}
	//	}

	//	if (!FallCollide) {
	//		in->SetFall(true);
	//	}
	//}

	/*if (!isCollide)
	{
		in->SetJump(true);
	}*/
}

void TileMap::Collide_Gate(Player* in)
{
	Vec2 inLT = { in->GetPos().x + TILE_SIZE / 4 + 3, in->GetPos().y + 3 };
	Vec2 inRB = { in->GetPos().x + TILE_SIZE * 0.75f - 3, in->GetPos().y + TILE_SIZE - 3 };

	bool isCollide = false;

	// Red Gate
	auto range = m_mmObjPos.equal_range(static_cast<int>('!'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if(in->GetColor() != PLAYER_COLOR::RED) {
			Vec2 figure = { min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				/*if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
				}*/
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
		}
	}

	// Green Gate
	range = m_mmObjPos.equal_range(static_cast<int>('@'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() != PLAYER_COLOR::GREEN) {
			Vec2 figure = { min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				/*if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
				}*/
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
		}
	}

	// Blue Gate
	range = m_mmObjPos.equal_range(static_cast<int>('#'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() != PLAYER_COLOR::BLUE) {
			Vec2 figure = { min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				/*if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
				}*/
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
		}
	}

	// Yellow Gate
	range = m_mmObjPos.equal_range(static_cast<int>('$'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() != PLAYER_COLOR::YELLOW) {
			Vec2 figure = { min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				/*if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
				}*/
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
		}
	}

	// Magenta Gate
	range = m_mmObjPos.equal_range(static_cast<int>('%'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() != PLAYER_COLOR::PURPLE) {
			Vec2 figure = { min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				/*if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
				}*/
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
		}
	}

	// Cyan Gate
	range = m_mmObjPos.equal_range(static_cast<int>('^'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() != PLAYER_COLOR::GB) {
			Vec2 figure = { min(fabs(inRB.x - wallLT.x), fabs(wallRB.x - inLT.x)), min(fabs(inRB.y - wallLT.y), fabs(wallRB.y - inLT.y)) };
			Vec2 updatePos = in->GetPos();
			if (figure.x > figure.y) {
				/*if (inLT.y - 3 > wallLT.y) updatePos.y += figure.y + 3;
				else
				{
					updatePos.y -= figure.y + 3;
					in->SetJump(false);
				}*/
			}
			else {
				if (inLT.x > wallLT.x) updatePos.x += figure.x;
				else updatePos.x -= figure.x;
			}
			in->SetPos(updatePos);
			isCollide = true;
		}
	}
}

void TileMap::Collide_Jump(Player* in)
{
	Vec2 inLT = { in->GetPos().x + TILE_SIZE / 4 + 3, in->GetPos().y + 3 };
	Vec2 inRB = { in->GetPos().x + TILE_SIZE * 0.75f - 3, in->GetPos().y + TILE_SIZE - 3 };

	bool isCollide = false;

	// Red Jump
	auto range = m_mmObjPos.equal_range(static_cast<int>('Z'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() == PLAYER_COLOR::RED) {
			in->SetJumpChange(30);
			in->SetSuperJump(true);
			isCollide = true;
			break;
		}
	}

	// Green Jump
	range = m_mmObjPos.equal_range(static_cast<int>('Y'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() == PLAYER_COLOR::GREEN) {
			in->SetJumpChange(30);
			in->SetSuperJump(true);
			isCollide = true;
			break;
		}
	}

	// Blue Jump
	range = m_mmObjPos.equal_range(static_cast<int>('X'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() == PLAYER_COLOR::BLUE) {
			in->SetJumpChange(30);
			in->SetSuperJump(true);
			isCollide = true;
			break;
		}
	}

	// Yellow Jump
	range = m_mmObjPos.equal_range(static_cast<int>('W'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() == PLAYER_COLOR::YELLOW) {
			in->SetJumpChange(30);
			in->SetSuperJump(true);
			isCollide = true;
			break;
		}
	}

	// Magenta Jump
	range = m_mmObjPos.equal_range(static_cast<int>('V'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() == PLAYER_COLOR::PURPLE) {
			in->SetJumpChange(30);
			in->SetSuperJump(true);
			isCollide = true;
			break;
		}
	}

	// Cyan Jump
	range = m_mmObjPos.equal_range(static_cast<int>('U'));
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else if (in->GetColor() == PLAYER_COLOR::GB) {
			in->SetJumpChange(30);
			in->SetSuperJump(true);
			isCollide = true;
			break;
		}
	}

	if (!isCollide)
	{
		//in->SetJumpChange(10);
		//in->SetSuperJump(false);
	}
}

bool TileMap::Collide_Gun(Player* in)
{
	Vec2 inLT = { in->GetPos().x + TILE_SIZE / 4 + 3, in->GetPos().y + 3 };
	Vec2 inRB = { in->GetPos().x + TILE_SIZE * 0.75f - 3, in->GetPos().y + TILE_SIZE - 3 };

	bool bCollide = false;

	auto range = m_mmObjPos.equal_range((int)'A');
	for (auto i = range.first; i != range.second; ++i) {
		Vec2 wallLT = { i->second.x - 1, i->second.y };
		Vec2 wallRB = { i->second.x + TILE_SIZE + 1, i->second.y + TILE_SIZE };

		if (inLT.x > wallRB.x) continue;
		else if (inRB.x < wallLT.x) continue;
		else if (inLT.y > wallRB.y) continue;
		else if (inRB.y < wallLT.y) continue;
		else {
			bCollide = true;
			//for (auto it = range.first; it != range.second;)
			//{
			//	if (it->first == (int)'A')
			//	{
			//		it = m_mmObjPos.erase(it);
			//		break;
			//	}
			//	else
			//		++it;
			//}
			i = m_mmObjPos.erase(i);
			break;
		}
	}
	return bCollide;
}