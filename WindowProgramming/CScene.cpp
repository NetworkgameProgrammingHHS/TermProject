#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CStage1.h"
#include "CTileMap.h"

CScene::CScene()
{
	
}

CScene::~CScene()
{
}

void CScene::Initialize()
{
	m_pPlayer = make_shared<CPlayer>();

	m_pStage = dynamic_pointer_cast<CScene>(make_shared<CStage1>(m_pPlayer));
}

void CScene::Update(const float ElapsedTime)
{
	m_pStage->Update(ElapsedTime);

	m_pPlayer->Update(ElapsedTime);

}

void CScene::Render(sf::RenderWindow& RW)
{
	m_pStage->Render(RW);
	m_pPlayer->Render(RW);
}

void CScene::KeyBoardInput(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardInput(key);
}

void CScene::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	m_pPlayer->KeyBoardRelease(key);
}

void CScene::Collide_Wall()
{
	if (m_pTileMap && !m_pPlayer->GetJump()) {
		for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
			if (m_pPlayer->GetAABB().intersects(wall.GetSprite().getGlobalBounds())) {
				m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
				break;
			}
		}
	}
}

void CScene::Collide_OBJ()
{
	Collide_Wall();
	Collide_Potion();
	Collide_Gate();
	Collide_Turret();
	Collide_Jump();
}

void CScene::Collide_Potion()
{
	// Red Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::PURPLE);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::RED);
		}
	}

	// Green Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			if (PLAYER_COLOR::RED == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::GB);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::GREEN);
			break;
		}
	}

	// Blue Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			if (PLAYER_COLOR::RED == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::PURPLE);
			else if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::GB);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::BLUE);
		}
	}

	// Black Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLACK_P)->second) {
		if (potion.GetEnable() && m_pPlayer->GetAABB().intersects(potion.GetAABB())) {
			potion.SetEnable(false);
			m_pPlayer->SetColor(PLAYER_COLOR::NORMAL);
			break;
		}
	}
}

void CScene::Collide_Gate()
{
	// Red Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_G)->second) {
		if (PLAYER_COLOR::RED != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Green Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_G)->second) {
		if (PLAYER_COLOR::GREEN != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Blue Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_G)->second) {
		if (PLAYER_COLOR::BLUE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Yellow Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_G)->second) {
		if (PLAYER_COLOR::YELLOW != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Purple Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_G)->second) {
		if (PLAYER_COLOR::PURPLE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// GB Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_G)->second) {
		if (PLAYER_COLOR::GB != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}
}

void CScene::Collide_Turret()
{
}

void CScene::Collide_Jump()
{
	//Red Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_J)->second) {
		if (PLAYER_COLOR::RED == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}
	
	//Green Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_J)->second) {
		if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Blue Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_J)->second) {
		if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Yellow Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_J)->second) {
		if (PLAYER_COLOR::YELLOW == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Purple Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_J)->second) {
		if (PLAYER_COLOR::PURPLE == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//GB Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_J)->second) {
		if (PLAYER_COLOR::GB == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	m_pPlayer->SetSuperJump(false);
}

void CScene::JumpProcess()
{
	// 스크린 화면 y좌표는 위에가 0, 아래로 갈수록 증가
	for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
		if (wall.GetSprite().getPosition().y - m_pPlayer->GetSprite().getPosition().y < TILE_SIZE / 2)
			continue;
		if (abs(m_pPlayer->GetSprite().getPosition().x - wall.GetSprite().getPosition().x) > TILE_SIZE * 2)
			continue;
		if (m_pPlayer->GetAABB().intersects(wall.GetSprite().getGlobalBounds())) {
			m_pPlayer->SetJump(false);
			m_pPlayer->SetPosition(sf::Vector2f(m_pPlayer->GetSprite().getPosition().x, wall.GetSprite().getPosition().y - m_pPlayer->GetSprite().getGlobalBounds().height));
			cout << "Jump False" << endl;
			break;
		}
	}
}
