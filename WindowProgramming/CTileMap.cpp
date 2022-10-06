#include "pch.h"
#include "CTileMap.h"

CTile::CTile(const string& Filename, const TILE_TYPE& type)
{
	if (!m_sfTexture.loadFromFile(Filename))
		exit(1);

	SetType(type);
}

CTile::CTile(const sf::Texture& tex, const sf::Vector2f& pos)
{
	m_sfTexture = tex;
	SetPosition(pos);
}

CTile::CTile(const sf::Vector2f& pos)
{
	SetPosition(pos);
}

CTile::~CTile()
{
}

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

	m_umTileInfo.reserve(TILE_CNT);
	m_umTileInfo.try_emplace('D', CTile("Resource\\TileSet\\Lab_Tile.png", TILE_TYPE::WALL));

	m_umTileInfo.try_emplace('R', CTile("Resource\\Object\\Potion_Red.png", TILE_TYPE::RED_P));
	m_umTileInfo.try_emplace('G', CTile("Resource\\Object\\Potion_Green.png", TILE_TYPE::GREEN_P));
	m_umTileInfo.try_emplace('B', CTile("Resource\\Object\\Potion_Blue.png", TILE_TYPE::BLUE_P));
	m_umTileInfo.try_emplace('N', CTile("Resource\\Object\\Potion_Black.png", TILE_TYPE::BLACK_P));

	m_umTileInfo.try_emplace('1', CTile("Resource\\Object\\Turret_Red.png", TILE_TYPE::RED_T));
	m_umTileInfo.try_emplace('2', CTile("Resource\\Object\\Turret_Green.png", TILE_TYPE::GREEN_T));
	m_umTileInfo.try_emplace('3', CTile("Resource\\Object\\Turret_Blue.png", TILE_TYPE::BLUE_T));
	m_umTileInfo.try_emplace('4', CTile("Resource\\Object\\Turret_Red+Green.png", TILE_TYPE::YELLOW_T));
	m_umTileInfo.try_emplace('5', CTile("Resource\\Object\\Turret_Red+Blue.png", TILE_TYPE::PURPLE_T));
	m_umTileInfo.try_emplace('6', CTile("Resource\\Object\\Turret_Green+Blue.png", TILE_TYPE::GB_T));
	m_umTileInfo.try_emplace('7', CTile("Resource\\Object\\Turret_Wait.png", TILE_TYPE::IDLE_T));

	m_umTileInfo.try_emplace('Z', CTile("Resource\\Object\\Jump_Red.png", TILE_TYPE::RED_J));
	m_umTileInfo.try_emplace('Y', CTile("Resource\\Object\\Jump_Green.png", TILE_TYPE::GREEN_J));
	m_umTileInfo.try_emplace('X', CTile("Resource\\Object\\Jump_Blue.png", TILE_TYPE::BLUE_J));
	m_umTileInfo.try_emplace('W', CTile("Resource\\Object\\Jump_Red+Green.png", TILE_TYPE::YELLOW_J));
	m_umTileInfo.try_emplace('V', CTile("Resource\\Object\\Jump_Red+Blue.png", TILE_TYPE::PURPLE_J));
	m_umTileInfo.try_emplace('U', CTile("Resource\\Object\\Jump_Green+Blue.png", TILE_TYPE::GB_J));

	m_umTileInfo.try_emplace('!', CTile("Resource\\Object\\Gate_Red.png", TILE_TYPE::RED_G));
	m_umTileInfo.try_emplace('@', CTile("Resource\\Object\\Gate_Green.png", TILE_TYPE::GREEN_G));
	m_umTileInfo.try_emplace('#', CTile("Resource\\Object\\Gate_Blue.png", TILE_TYPE::BLUE_G));
	m_umTileInfo.try_emplace('$', CTile("Resource\\Object\\Gate_Red+Green.png", TILE_TYPE::YELLOW_G));
	m_umTileInfo.try_emplace('%', CTile("Resource\\Object\\Gate_Red+Blue.png", TILE_TYPE::PURPLE_G));
	m_umTileInfo.try_emplace('^', CTile("Resource\\Object\\Gate_Green+Blue.png", TILE_TYPE::GB_G));

	m_umTiles.reserve(25);
	 
	for (int i = static_cast<int>(TILE_TYPE::NONE); i < static_cast<int>(TILE_TYPE::END); ++i) {
		m_umTiles.try_emplace(static_cast<TILE_TYPE>(i), vector<CTile>());
	}
	for (int i = 0; i < TILE_NUM_H; ++i) {
		for (int j = 0; j < TILE_NUM_W; ++j) {
			if (m_vMap[i * TILE_NUM_W + j] == '0')
				m_umTiles.find(TILE_TYPE::NONE)->second.emplace_back(CTile(sf::Vector2f(static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE))));
			else {
				m_umTiles.find(m_umTileInfo.find(m_vMap[i * TILE_NUM_W + j])->second.GetType())->second.emplace_back(CTile(m_umTileInfo.find(m_vMap[i * TILE_NUM_W + j])->second.GetTexture(), sf::Vector2f(static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE))));
			}
		}
	}
}

CTileMap::~CTileMap()
{
}

void CTileMap::Initialize()
{
	for (int i = static_cast<int>(TILE_TYPE::WALL); i < static_cast<int>(TILE_TYPE::END); ++i) {
		for (auto& sprite : m_umTiles.find(static_cast<TILE_TYPE>(i))->second) {
			sprite.SetSpriteTex();
			if (i == static_cast<int>(TILE_TYPE::WALL)) {
				sprite.SetAABB(sprite.GetSprite().getPosition(), sf::Vector2f(static_cast<float>(TILE_SIZE) / 1.2f, static_cast<float>(TILE_SIZE) / 1.2f));
			}
			else {
				sprite.SetAABB(sprite.GetSprite().getPosition(), sf::Vector2f(static_cast<float>(TILE_SIZE) / 2.f, static_cast<float>(TILE_SIZE) / 1.0f));
			}
		}
	}
}

void CTileMap::Render(sf::RenderWindow& RW)
{
	for (int i = static_cast<int>(TILE_TYPE::WALL); i < static_cast<int>(TILE_TYPE::END); ++i) {
		for (const auto& sprite : m_umTiles.find(static_cast<TILE_TYPE>(i))->second) {
			if(sprite.GetEnable())
				RW.draw(sprite.GetSprite());
		}
	}
}
