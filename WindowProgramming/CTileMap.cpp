#include "pch.h"
#include "CTileMap.h"

CTileMap::CTileMap(const string& Filename)
{
	m_vMap.reserve(TILE_NUM_H * TILE_NUM_W);
	ifstream in(Filename);
	while (!in.eof()) {
		char tile;
		in >> tile;
		if(m_vMap.size() < TILE_NUM_H * TILE_NUM_W)
			m_vMap.emplace_back(tile);	
	}
	m_umTiles.reserve(TILE_CNT);
	m_umTiles.try_emplace('D', CTile("Resource\\TileSet\\Lab_Tile.png"));

	m_umTiles.try_emplace('R', CTile("Resource\\Object\\Potion_Red.png"));
	m_umTiles.try_emplace('G', CTile("Resource\\Object\\Potion_Green.png"));
	m_umTiles.try_emplace('B', CTile("Resource\\Object\\Potion_Blue.png"));
	m_umTiles.try_emplace('N', CTile("Resource\\Object\\Potion_Black.png"));

	m_umTiles.try_emplace('1', CTile("Resource\\Object\\Turret_Red.png"));
	m_umTiles.try_emplace('2', CTile("Resource\\Object\\Turret_Green.png"));
	m_umTiles.try_emplace('3', CTile("Resource\\Object\\Turret_Blue.png"));
	m_umTiles.try_emplace('4', CTile("Resource\\Object\\Turret_Red+Green.png"));
	m_umTiles.try_emplace('5', CTile("Resource\\Object\\Turret_Red+Blue.png"));
	m_umTiles.try_emplace('6', CTile("Resource\\Object\\Turret_Green+Blue.png"));
	m_umTiles.try_emplace('7', CTile("Resource\\Object\\Turret_Wait.png"));

	m_umTiles.try_emplace('Z', CTile("Resource\\Object\\Jump_Red.png"));
	m_umTiles.try_emplace('Y', CTile("Resource\\Object\\Jump_Green.png"));
	m_umTiles.try_emplace('X', CTile("Resource\\Object\\Jump_Blue.png"));
	m_umTiles.try_emplace('W', CTile("Resource\\Object\\Jump_Red+Green.png"));
	m_umTiles.try_emplace('V', CTile("Resource\\Object\\Jump_Red+Blue.png"));
	m_umTiles.try_emplace('U', CTile("Resource\\Object\\Jump_Green+Blue.png"));

	m_umTiles.try_emplace('!', CTile("Resource\\Object\\Gate_Red.png"));
	m_umTiles.try_emplace('@', CTile("Resource\\Object\\Gate_Green.png"));
	m_umTiles.try_emplace('#', CTile("Resource\\Object\\Gate_Blue.png"));
	m_umTiles.try_emplace('$', CTile("Resource\\Object\\Gate_Red+Green.png"));
	m_umTiles.try_emplace('%', CTile("Resource\\Object\\Gate_Red+Blue.png"));
	m_umTiles.try_emplace('^', CTile("Resource\\Object\\Gate_Green+Blue.png"));

	for (int i = 0; i < TILE_NUM_H; ++i) {
		for (int j = 0; j < TILE_NUM_W; ++j) {
			if (m_vMap[i * TILE_NUM_W + j] != '0') {
				m_arrTiles[i * TILE_NUM_W + j].SetTexture(m_umTiles.find(m_vMap[i * TILE_NUM_W + j])->second.GetTexture());
				m_arrTiles[i * TILE_NUM_W + j].SetSpriteTex();
				m_arrTiles[i * TILE_NUM_W + j].SetPosition(sf::Vector2f(static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE)));
			}
		}
	}
}

CTileMap::~CTileMap()
{
}

CTile::CTile(const string& Filename)
{	
	if (!m_sfTexture.loadFromFile(Filename))
		exit(1);
}

CTile::~CTile()
{
}
