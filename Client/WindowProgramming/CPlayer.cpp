#include "pch.h"
#include "CPlayer.h"
#include "CNetworkMgr.h"

CPlayer::CPlayer(shared_ptr<CNetworkMgr> networkmgr)
{
	m_pNetworkMgr = networkmgr;

	m_umTextures.reserve(static_cast<size_t>(PLAYER_COLOR::END));

	sf::Texture temp; 
	temp.loadFromFile("Resource\\Character\\Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::NORMAL, temp);

	temp.loadFromFile("Resource\\Character\\Red_Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::RED, temp);

	temp.loadFromFile("Resource\\Character\\Green_Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::GREEN, temp);

	temp.loadFromFile("Resource\\Character\\Blue_Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::BLUE, temp);

	temp.loadFromFile("Resource\\Character\\Red+Green_Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::YELLOW, temp);

	temp.loadFromFile("Resource\\Character\\Red+Blue_Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::PURPLE, temp);

	temp.loadFromFile("Resource\\Character\\Green+Blue_Scientist.png");
	m_umTextures.try_emplace(PLAYER_COLOR::GB, temp);
	
	m_sfSprite.setTexture(m_umTextures.find(PLAYER_COLOR::NORMAL)->second);
	m_sfSprite.setTextureRect(sf::IntRect(static_cast<int>(m_fSpriteLeft), m_iSpriteTop, TILE_SIZE, TILE_SIZE));

	m_vec2fPos = { 0.0f, WINDOW_HEIGHT - TILE_SIZE };
	m_vec2fPrevPos = sf::Vector2f{ static_cast<float>(TILE_SIZE), static_cast<float>(WINDOW_HEIGHT - 2 * TILE_SIZE) };

	m_rtFallBB.left = m_sfSprite.getPosition().x + static_cast<float>(TILE_SIZE) / 4.f;
	m_rtFallBB.top = m_sfSprite.getPosition().y + static_cast<float>(TILE_SIZE);
	m_rtFallBB.width = static_cast<float>(TILE_SIZE) / 2.f;
	m_rtFallBB.height = static_cast<float>(TILE_SIZE) / 4.0f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Animation(const float ElapsedTime)
{
	switch (m_iDir) {
	case 0:
		m_eState = PLAYER_STATE::IDLE;
		break;
	case -1:
		m_eState = PLAYER_STATE::MOVE_L;
		m_iSpriteTop = 64;
		break;
	case 1:
		m_eState = PLAYER_STATE::MOVE_R;
		m_iSpriteTop = 96;
		break;
	default:
		break;
	}

	if (PLAYER_STATE::IDLE != m_eState)
		m_fSpriteLeft = (m_fSpriteLeft + FRAMES_PER_ACTION * ACTION_PER_TIME * ElapsedTime);
	else
		m_fSpriteLeft = 0.f;
	int FrameStart = static_cast<int>(m_fSpriteLeft) % FRAMES_PER_ACTION;

	m_sfSprite.setTextureRect(sf::IntRect(FrameStart * 32, m_iSpriteTop, TILE_SIZE, TILE_SIZE));
}

void CPlayer::Reset()
{
	m_vec2fPrevPos = {};
	m_iDir = 0;
	m_bFall = false;
	m_bJump = false;
	m_bSuperJump = false;
	m_iJumpCnt = 0;
	m_iJumpChange = 0;
	m_fJumpVelocity = 0.0f;

	m_fSpriteLeft = 0;
	m_iSpriteTop = 32;
	m_eState = PLAYER_STATE::IDLE;
	SetColor(PLAYER_COLOR::NORMAL);
}

void CPlayer::Update(const float ElapsedTime)
{
	Animation(ElapsedTime);
	
	//m_vec2fPrevPos = m_vec2fPos;

	m_sfSprite.setPosition(m_vec2fPos);
	UpdateAABB();
}

void CPlayer::Render(sf::RenderWindow& RW)
{
	if (m_bDebug) {
		sf::RectangleShape rt;
		rt.setPosition(m_rtAABB.left, m_rtAABB.top);
		rt.setSize(sf::Vector2f(m_rtAABB.width, m_rtAABB.height));
		rt.setOutlineColor(sf::Color::Red);
		rt.setOutlineThickness(1.f);

		sf::RectangleShape rt2;
		rt2.setPosition(m_rtFallBB.left, m_rtFallBB.top);
		rt2.setSize(sf::Vector2f(m_rtFallBB.width, m_rtFallBB.height));
		rt2.setOutlineColor(sf::Color::Blue);
		rt2.setOutlineThickness(1.f);

		RW.draw(rt);
		RW.draw(rt2);
	}

	// Render Spoid Color Rect
	if (m_bSpoid) {
		sf::RectangleShape rt;
		switch (m_eSavedColor) {
		case PLAYER_COLOR::RED:
			rt.setFillColor(sf::Color::Red);
			break;
		case PLAYER_COLOR::GREEN:
			rt.setFillColor(sf::Color::Green);
			break;
		case PLAYER_COLOR::BLUE:
			rt.setFillColor(sf::Color::Blue);
			break;
		case PLAYER_COLOR::YELLOW:
			rt.setFillColor(sf::Color::Yellow);
			break;
		case PLAYER_COLOR::PURPLE:
			rt.setFillColor(sf::Color::Magenta);
			break;
		case PLAYER_COLOR::GB:
			rt.setFillColor(sf::Color::Cyan);
			break;
		default:
			break;
		}
		
		rt.setPosition(m_vec2fPos.x - 1.0f, m_vec2fPos.y + 1.0f);
		rt.setSize(sf::Vector2f(5.0f, 5.0f));
		RW.draw(rt);
	}

	if(m_bOnline)
		RW.draw(m_sfSprite);
}

void CPlayer::UpdateAABB()
{
	m_rtAABB.height = TILE_SIZE;
	m_rtAABB.width = TILE_SIZE / 2;
	m_rtAABB.left = m_vec2fPos.x + TILE_SIZE / 4;
	m_rtAABB.top = m_vec2fPos.y;

	m_rtFallBB.left = m_vec2fPos.x + static_cast<float>(TILE_SIZE) / 4.f;
	m_rtFallBB.top = m_vec2fPos.y + static_cast<float>(TILE_SIZE);
}

void CPlayer::KeyBoardInput(const sf::Keyboard::Key& key)
{
	bool input = false;
	char dir = 0;

	switch (key) {
	case sf::Keyboard::Left:
		if (m_nKeyDir > -1) {
			input = true;
			dir = KEY_DIR_LEFT;
			m_nKeyDir -= 1;
		}
		break;
	case sf::Keyboard::Right:
		if (m_nKeyDir < 1) {
			input = true;
			dir = KEY_DIR_RIGHT;
			m_nKeyDir += 1;
		}
		break;
	case sf::Keyboard::Space:
		if (!m_bJump) {
			//m_bJump = true;
			input = true;
			dir = KEY_DIR_UP;
		}
		break;
	case sf::Keyboard::Escape:
		exit(1);
		break;
	case sf::Keyboard::F5:
		m_bDebug = !m_bDebug;
		break;
	case sf::Keyboard::F:
		if (m_bSpoid) {
			m_bSpoid = false;
			SetColor(m_eSavedColor);
		}
		break;
	case sf::Keyboard::C:
		input = true;
		dir = KEY_FIREGUN;
		break;
	default:
		break;
	}

	if (input && dir != NULL) {
		CS_INPUT_PACKET* packet = new CS_INPUT_PACKET;
		packet->type = CS_INPUT;
		packet->key = dir;
		packet->state = KEY_PRESS;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_INPUT_PACKET));
		cout << "Å° ´©¸§" << endl;
		cout << "Dir : " << m_nKeyDir << endl;
	}
	else if (input && dir == NULL) {
		CS_PLAYER_COLOR_PACKET* packet = new CS_PLAYER_COLOR_PACKET;
		packet->type = CS_COLOR;
		packet->color = static_cast<short>(m_eSavedColor);
		//packet->collide = POTION_COLLIDE_OFF;
	}
}

void CPlayer::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	bool input = false;
	char dir = 0;

	switch (key) {
	case sf::Keyboard::Left:
		if (m_nKeyDir == -1) {
			input = true;
			dir = KEY_DIR_LEFT;
			m_nKeyDir = 0;
		}
		break;
	case sf::Keyboard::Right:
		if (m_nKeyDir == 1) {
			input = true;
			dir = KEY_DIR_RIGHT;
			m_nKeyDir = 0;
		}
		break;
	default:
		break;
	}
	if (input && dir != NULL) {
		CS_INPUT_PACKET* packet = new CS_INPUT_PACKET;
		packet->type = CS_INPUT;
		packet->key = dir;
		packet->state = KEY_RELEASE;
		m_pNetworkMgr->SendPacket(reinterpret_cast<char*>(packet), sizeof(CS_INPUT_PACKET));
		cout << "Å° ¶À" << endl;
		cout << "Dir : " << m_nKeyDir << endl;
	}
}
