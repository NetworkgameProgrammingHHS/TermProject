#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
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

void CPlayer::Update(const float ElapsedTime)
{
	Animation(ElapsedTime);
	
	m_vec2fPrevPos = m_vec2fPos;

	m_vec2fPos.x += m_iDir * PLAYER_SPEED * ElapsedTime;

	if (m_bJump) {
		if (m_iJumpCnt < m_iJumpChange) {
			m_vec2fPos.y -= m_fJumpVelocity * ElapsedTime;
		}
		else {
			m_vec2fPos.y += m_fJumpVelocity * ElapsedTime;
		}
		++m_iJumpCnt;
	}
	else
		m_iJumpCnt = 0;

	m_sfSprite.setPosition(m_vec2fPos);
	UpdateAABB();
}

void CPlayer::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfSprite);
}

void CPlayer::UpdateAABB()
{
	m_rtAABB.height = TILE_SIZE;
	m_rtAABB.width = TILE_SIZE / 2;
	m_rtAABB.left = m_vec2fPos.x;
	m_rtAABB.top = m_vec2fPos.y;

	m_rtFallBB.left = m_vec2fPos.x + static_cast<float>(TILE_SIZE) / 4.f;
	m_rtFallBB.top = m_vec2fPos.y + static_cast<float>(TILE_SIZE);
}

void CPlayer::KeyBoardInput(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Left:
		if (m_iDir > -1)
			m_iDir -= 1;
		break;
	case sf::Keyboard::Right:
		if (m_iDir < 1)
			m_iDir += 1;
		break;
	case sf::Keyboard::Space:
		if (!m_bJump) {
			m_bJump = true;
			if (m_bSuperJump) {
				m_fJumpVelocity = SUPERJUMP_SPEED;
				m_iJumpChange = 30;
			}
			else {
				m_fJumpVelocity = JUMP_SPEED;
				m_iJumpChange = 20;
			}
		}
		break;
	case sf::Keyboard::Escape:
		exit(1);
		break;
	default:
		break;
	}	
}

void CPlayer::KeyBoardRelease(const sf::Keyboard::Key& key)
{
	switch (key) {
	case sf::Keyboard::Left:
		if (m_iDir < 1)
			m_iDir += 1;
		break;
	case sf::Keyboard::Right:
		if (m_iDir > -1)
			m_iDir -= 1;
		break;
	default:
		break;
	}
}
