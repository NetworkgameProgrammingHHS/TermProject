#include "pch.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CNetworkMgr.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CTileMap.h"

CScene::CScene()
{
	m_pGun = make_unique<CGun>();

	m_sfFont.loadFromFile("Resource\\Font\\cour.ttf");
	m_sfPlayerColor[0].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 0, 32, 32)); // Normal
	m_sfPlayerColor[1].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(32, 0, 32, 32)); // Red
	m_sfPlayerColor[2].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 32, 32, 32)); // Green
	m_sfPlayerColor[3].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(32, 32, 32, 32)); // Blue
	m_sfPlayerColor[4].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 64, 32, 32)); // GB
	m_sfPlayerColor[5].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(32, 64, 32, 32)); // Yellow
	m_sfPlayerColor[6].loadFromFile("Resource\\Character\\Player_Info.png", sf::IntRect(0, 96, 32, 32)); // Cyan


	m_sfPlayerInfoText[0][0].setString("Player 1");
	m_sfPlayerInfoText[1][0].setString("Player 2");
	m_sfPlayerInfoText[2][0].setString("Player 3");
	m_sfPlayerInfoText[0][1].setString("Stage 1");
	m_sfPlayerInfoText[1][1].setString("Stage 1");
	m_sfPlayerInfoText[2][1].setString("Stage 1");
	for (int i = 0; i < 3; ++i) {
		m_sfPlayerInfo[i].setTexture(m_sfPlayerColor[0]);
		m_sfPlayerInfo[i].setScale(2, 2);
		m_sfPlayerInfo[i].setPosition(WINDOW_WIDTH - (m_sfPlayerInfo[0].getScale().x * TILE_SIZE * (3 - i)), 0);

		for (int j = 0; j < 2; ++j) {
			m_sfPlayerInfoText[i][j].setOutlineThickness(1.0f);
			m_sfPlayerInfoText[i][j].setCharacterSize(12);
			m_sfPlayerInfoText[i][j].setFont(m_sfFont);
			m_sfPlayerInfoText[i][j].setFillColor(sf::Color::White);
			auto size = m_sfPlayerInfoText[i][j].getGlobalBounds();
			m_sfPlayerInfoText[i][j].setPosition(WINDOW_WIDTH - (m_sfPlayerInfo[0].getScale().x * TILE_SIZE * (3 - i)) + 5, TILE_SIZE * 2 + j * 14 + 5 * (j + 1));
		}
	}
}

CScene::~CScene()
{
}

void CScene::KeyBoardInput(const sf::Keyboard::Key& key)
{
}

void CScene::Update(const float ElapsedTime)
{
	
}

void CScene::Render(sf::RenderWindow& RW)
{
	for (int i = 0; i < 3; ++i) {
		if (!m_sfPlayerInfo.empty()) {
			RW.draw(m_sfPlayerInfo[i]);
		}

		for (int j = 0; j < 2; ++j) {
			if (!m_sfPlayerInfoText.empty()) {
				if (!m_sfPlayerInfoText[i].empty()) {
					RW.draw(m_sfPlayerInfoText[i][j]);
				}
			}
		}
	}
}

void CScene::SetGunState(int enable, int bulletx, int bullety)
{
	if (m_pGun) 
	{
		m_pGun->SetEnable(enable);
		m_pGun->SetBulletPos(bulletx, bullety);
	}
}

void CScene::Logout(int index)
{
	m_ppPlayers[index]->Reset();
	m_ppPlayers[index]->SetPosition(sf::Vector2f(-1000.0f, -1000.0f));
	

}

void CScene::SetPlayerInfo(const PLAYER_COLOR pc, const int index)
{
	switch (m_ppPlayers[index]->GetColor()) {
	case PLAYER_COLOR::NORMAL:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[0]);
		break;
	case PLAYER_COLOR::RED:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[1]);
		break;
	case PLAYER_COLOR::GREEN:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[2]);
		break;
	case PLAYER_COLOR::BLUE:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[3]);
		break;
	case PLAYER_COLOR::GB:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[4]);
		break;
	case PLAYER_COLOR::YELLOW:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[5]);
		break;
	case PLAYER_COLOR::PURPLE:
		m_sfPlayerInfo[index].setTexture(m_sfPlayerColor[6]);
		break;
	default:
		break;
	}

}

void CScene::SetPlayerInfo(const SCENE_NUM sn, const int index)
{
	switch (m_ppPlayers[index]->GetStageNum()) {
	case SCENE_NUM::STAGE1:
		m_sfPlayerInfoText[index][1].setString("Stage 1");
		break;
	case SCENE_NUM::STAGE2:
		m_sfPlayerInfoText[index][1].setString("Stage 2");
		break;
	case SCENE_NUM::STAGE3:
		m_sfPlayerInfoText[index][1].setString("Stage 3");
		break;
	case SCENE_NUM::STAGE4:
		m_sfPlayerInfoText[index][1].setString("Stage 4");
		break;
	case SCENE_NUM::STAGE5:
		m_sfPlayerInfoText[index][1].setString("Stage 5");
		break;
	default:
		break;
	}
}

void CScene::SetPlayerName(const char* name, const int index)
{
	m_sfPlayerInfoText[index][0].setString(name);
}

void CScene::TileRest()
{
	m_pTileMap->TileReset();

}

/*void CScene::Collide_Wall()
{
	if (m_pTileMap) {
		for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(wall->GetAABB())) {
				if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetJump()) {
					m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos().x, m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPos().y));
					if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetJumpDir()) { // �Ӹ� �浹 �� �÷��̾� ���� ���� ����
						m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetJumpCnt(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetJumpChange());
					}
					else {
						m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetJump(false);
						// ������ �Ǵ� �κ�1 : ����Ͽ� �浹�ϸ� ��ǥ�� ������� �����̵�
						m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getPosition().x, wall->GetSprite().getPosition().y - m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getGlobalBounds().height));
					}
				}
				else {
					m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
				}
				break;				
			}
		}

		if (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetJump()) {
			for (const auto& wall : m_pTileMap->m_umTiles.find(TILE_TYPE::WALL)->second) {
				if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetFallBB().intersects(wall->GetAABB())) {
					m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetFall(false);	// ������ �Ǵ� �κ�2 : Fall���� false�� �Ǹ鼭 ���߿��� �����
					m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(sf::Vector2f(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getPosition().x, wall->GetSprite().getPosition().y - m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetSprite().getGlobalBounds().height));
					return;
				}
			}
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetFall(true);
		}
	}
}*/

void CScene::ShowPlayerInfo()
{
}

void CScene::Collide_OBJ()
{
	//Collide_Wall();
	Collide_Potion();
	//Collide_Gate();
	Collide_Turret();
	//Collide_Jump();
	Collide_Spoid();
}

void CScene::Collide_Potion()
{
	bool collide_potion = false;

	// Red Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_P)->second) {
		if (potion->GetEnable() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			collide_potion = true;
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::GREEN == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor())
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor())
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::PURPLE);
			else
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::RED);
			break;
		}
	}

	// Green Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_P)->second) {
		if (potion->GetEnable() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			collide_potion = true;
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::RED == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor())
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::YELLOW);
			else if (PLAYER_COLOR::BLUE == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor())
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::GB);
			else
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::GREEN);
			break;
		}
	}

	// Blue Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_P)->second) {
		if (potion->GetEnable() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			collide_potion = true;
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			if (PLAYER_COLOR::RED == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor())
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::PURPLE);
			else if (PLAYER_COLOR::GREEN == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor())
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::GB);
			else
				m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::BLUE);
			break;
		}
	}

	// Black Potion
	for (auto& potion : m_pTileMap->m_umTiles.find(TILE_TYPE::BLACK_P)->second) {
		if (potion->GetEnable() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(potion->GetAABB())) {
			potion->SetEnable(false);
			collide_potion = true;
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::NORMAL);
			m_pTileMap->SetPotionNum(m_pTileMap->GetPotionNum() - 1);
			break;
		}
	}

	if (collide_potion) {
		CS_PLAYER_COLOR_PACKET* packet = new CS_PLAYER_COLOR_PACKET;
		//packet->collide = false;
		packet->color = static_cast<short>(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor());
		packet->type = CS_COLOR;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_PLAYER_COLOR_PACKET));
		SetPlayerInfo(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor(), m_pNetworkMgr->GetPlayerIndex());
	}
}

/*void CScene::Collide_Gate()
{
	// Red Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_G)->second) {
		if (PLAYER_COLOR::RED != m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
			break;
		}
	}

	// Green Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_G)->second) {
		if (PLAYER_COLOR::GREEN != m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
			break;
		}
	}

	// Blue Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_G)->second) {
		if (PLAYER_COLOR::BLUE != m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
			break;
		}
	}

	// Yellow Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_G)->second) {
		if (PLAYER_COLOR::YELLOW != m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
			break;
		}
	}

	// Purple Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_G)->second) {
		if (PLAYER_COLOR::PURPLE != m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
			break;
		}
	}

	// GB Gate
	for (auto& gate : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_G)->second) {
		if (PLAYER_COLOR::GB != m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(gate->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetPosition(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetPrevPos());
			break;
		}
	}
}*/

void CScene::Collide_Turret()
{
	// Red Turret
	if (PLAYER_COLOR::RED == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
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
	if (PLAYER_COLOR::GREEN == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
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
	if (PLAYER_COLOR::BLUE == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
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
	if (PLAYER_COLOR::YELLOW == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
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
	if (PLAYER_COLOR::PURPLE == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
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
	if (PLAYER_COLOR::GB == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor()) {
		for (auto& turret : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_T)->second) {
			if (!dynamic_cast<CTurret*>(turret)->GetActivate() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) || m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1))) {
				sf::Texture temp = turret->GetTexture();
				turret->SetTexture(dynamic_cast<CTurret*>(turret)->GetTurretTex());
				turret->SetSpriteTex();
				dynamic_cast<CTurret*>(turret)->SetTurretTex(temp);
				dynamic_cast<CTurret*>(turret)->SetActivate(true);

			}
			if (m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(turret->GetSprite().getGlobalBounds())) {
				//Collide Turret
				Reset();
				break;
			}
			else if (dynamic_cast<CTurret*>(turret)->GetActivate() && (!m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(0)) && !m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(dynamic_cast<CTurret*>(turret)->GetTurretAABB(1)))) {
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

/*void CScene::Collide_Jump()
{
	//Red Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::RED_J)->second) {
		if (PLAYER_COLOR::RED == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(true);
			return;
		}
	}
	
	//Green Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GREEN_J)->second) {
		if (PLAYER_COLOR::GREEN == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(true);
			return;
		}
	}

	//Blue Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::BLUE_J)->second) {
		if (PLAYER_COLOR::BLUE == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(true);
			return;
		}
	}

	//Yellow Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::YELLOW_J)->second) {
		if (PLAYER_COLOR::YELLOW == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(true);
			return;
		}
	}

	//Purple Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::PURPLE_J)->second) {
		if (PLAYER_COLOR::PURPLE == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(true);
			return;
		}
	}

	//GB Jump
	for (const auto& jump : m_pTileMap->m_umTiles.find(TILE_TYPE::GB_J)->second) {
		if (PLAYER_COLOR::GB == m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(jump->GetSprite().getGlobalBounds())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(true);
			return;
		}
	}

	m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSuperJump(false);
}*/

void CScene::Collide_Spoid() {
	for (auto& spoid : m_pTileMap->m_umTiles.find(TILE_TYPE::SPOID)->second) {
		if (spoid->GetEnable() && m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetAABB().intersects(spoid->GetAABB())) {
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSpoid(true);
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetSavedColor(m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->GetColor());
			m_ppPlayers[m_pNetworkMgr->GetPlayerIndex()]->SetColor(PLAYER_COLOR::NORMAL);
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
