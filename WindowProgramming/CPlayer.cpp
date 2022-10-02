#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	
	if (!m_sfTexture.loadFromFile("Resource\\Character\\Scientist.png")) {
		exit(1);
	}
	
	m_sfSprite.setTexture(m_sfTexture);
	m_sfSprite.setTextureRect(sf::IntRect(m_fSpriteLeft, m_iSpriteTop, TILE_SIZE, TILE_SIZE));

	m_vec2fPos = { 0.0f, WINDOW_HEIGHT - TILE_SIZE };

}

CPlayer::~CPlayer()
{
}

void CPlayer::Animation(const float ElapsedTime)
{
	switch (m_iDir) {
	case 0:
		m_eState = PLAYER_STATE::IDLE;
		m_iSpriteTop = 32;
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

	m_sfSprite.setPosition(m_vec2fPos);

}

void CPlayer::Render(sf::RenderWindow& RW)
{
	RW.draw(m_sfSprite);
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
		cout << "space" << endl;
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
