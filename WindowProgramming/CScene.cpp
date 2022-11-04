#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CTileMap.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::KeyBoardInput(const sf::Keyboard::Key& key)
{
}

void CScene::Update(const float ElapsedTime)
{
	m_pPlayer->Update(ElapsedTime);
}

void CScene::Render(sf::RenderWindow& RW)
{
	m_pPlayer->Render(RW);
}

void CScene::Collide_Wall()
{
	if (m_pTileMap) {
		for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
			if (m_pPlayer->GetAABB().intersects(wall->GetAABB())) {
				if (m_pPlayer->GetJump()) {
					m_pPlayer->SetPosition(sf::Vector2f(m_pPlayer->GetPrevPos().x, m_pPlayer->GetPos().y));					
					if (m_pPlayer->GetJumpDir()) { // 머리 충돌 시 플레이어 점프 방향 변경
						m_pPlayer->SetJumpCnt(m_pPlayer->GetJumpChange());
					}
					else {
						m_pPlayer->SetJump(false);
						// 문제가 되는 부분1 : 통과하여 충돌하면 좌표가 블록위로 순간이동
						m_pPlayer->SetPosition(sf::Vector2f(m_pPlayer->GetSprite().getPosition().x, wall->GetSprite().getPosition().y - m_pPlayer->GetSprite().getGlobalBounds().height));
					}
				}
				else {
					m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
				}
				break;				
			}
		}

		if (!m_pPlayer->GetJump()) {
			for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
				if (m_pPlayer->GetFallBB().intersects(wall->GetAABB())) {
					m_pPlayer->SetFall(false);	// 문제가 되는 부분2 : Fall값이 false가 되면서 공중에서 멈춘다
					m_pPlayer->SetPosition(sf::Vector2f(m_pPlayer->GetSprite().getPosition().x, wall->GetSprite().getPosition().y - m_pPlayer->GetSprite().getGlobalBounds().height));
					return;
				}
			}
			m_pPlayer->SetFall(true);
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
	Collide_Spoid();
}

void CScene::Collide_Potion()
{
	// Red Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_P)->second) {
		if (potion->GetEnable() && m_pPlayer->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::PURPLE);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::RED);
			break;
		}
	}

	// Green Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_P)->second) {
		if (potion->GetEnable() && m_pPlayer->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
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
		if (potion->GetEnable() && m_pPlayer->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::RED == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::PURPLE);
			else if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor())
				m_pPlayer->SetColor(PLAYER_COLOR::GB);
			else
				m_pPlayer->SetColor(PLAYER_COLOR::BLUE);
			break;
		}
	}

	// Black Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLACK_P)->second) {
		if (potion->GetEnable() && m_pPlayer->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			m_pPlayer->SetColor(PLAYER_COLOR::NORMAL);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			break;
		}
	}
}

void CScene::Collide_Gate()
{
	// Red Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_G)->second) {
		if (PLAYER_COLOR::RED != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Green Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_G)->second) {
		if (PLAYER_COLOR::GREEN != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Blue Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_G)->second) {
		if (PLAYER_COLOR::BLUE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Yellow Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_G)->second) {
		if (PLAYER_COLOR::YELLOW != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// Purple Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_G)->second) {
		if (PLAYER_COLOR::PURPLE != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}

	// GB Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_G)->second) {
		if (PLAYER_COLOR::GB != m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetPosition(m_pPlayer->GetPrevPos());
			break;
		}
	}
}

void CScene::Collide_Turret()
{
	// Red Turret
	if (PLAYER_COLOR::RED == m_pPlayer->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_pPlayer->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
				//No Collide
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(false);
			}
		}
	}

	// Green Turret
	if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_pPlayer->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
				//No Collide
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(false);
			}
		}
	}

	// Blue Turret
	if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_pPlayer->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
				//No Collide
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(false);
			}
		}
	}

	// Yellow Turret
	if (PLAYER_COLOR::YELLOW == m_pPlayer->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_pPlayer->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
				//No Collide
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(false);
			}
		}
	}

	// Purple Turret
	if (PLAYER_COLOR::PURPLE == m_pPlayer->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_pPlayer->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
				//No Collide
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(false);
			}
		}
	}

	// GB Turret
	if (PLAYER_COLOR::GB == m_pPlayer->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_pPlayer->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_pPlayer->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
				//No Collide
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(false);
			}
		}
	}
}

void CScene::Collide_Jump()
{
	//Red Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_J)->second) {
		if (PLAYER_COLOR::RED == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}
	
	//Green Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_J)->second) {
		if (PLAYER_COLOR::GREEN == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Blue Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_J)->second) {
		if (PLAYER_COLOR::BLUE == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Yellow Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_J)->second) {
		if (PLAYER_COLOR::YELLOW == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//Purple Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_J)->second) {
		if (PLAYER_COLOR::PURPLE == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	//GB Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_J)->second) {
		if (PLAYER_COLOR::GB == m_pPlayer->GetColor() && m_pPlayer->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_pPlayer->SetSuperJump(true);
			return;
		}
	}

	m_pPlayer->SetSuperJump(false);
}

void CScene::Collide_Spoid() {
	for (auto& spoid : m_pTileMap->m_umTiles.find(TILE_TYPE::SPOID)->second) {
		if (spoid->GetEnable() && m_pPlayer->GetAABB().intersects(spoid->GetAABB())) {
			m_pPlayer->SetSpoid(true);
			m_pPlayer->SetSavedColor(m_pPlayer->GetColor());
			m_pPlayer->SetColor(PLAYER_COLOR::NORMAL);
			spoid->SetEnable(false);
			break;
		}
	}
}

void CScene::Next_Stage()
{

}

void CScene::Reset() 
{
}
