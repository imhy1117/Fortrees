#include "Missile_redBall.h"
#include "Scene_game.h"


Missile_redBall::Missile_redBall()
{
}
void Missile_redBall::init(Vec2 pt, Tank* pTank, Scene_game* pScene)
{
	m_sprMissile = Sprite::create("redball.png");
	m_sprMissile->setPosition(pt);
	m_pt = pt;
	m_sprMissile->retain();
	pScene->getGameLayer()->addChild(m_sprMissile, 3);
	m_fWind = 0;
	m_fPower = 0;
	m_fTime = 0;
	m_iDegree = 0;
	m_bExist = true;
	m_bPowerup = false;
	m_pTank = pTank;
	m_pScene = pScene;
	m_iRad = 30;
	m_iDamage = 150;
}

Missile_redBall::~Missile_redBall()
{
}
