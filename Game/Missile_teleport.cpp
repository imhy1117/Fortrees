#include "Missile_teleport.h"
#include "Scene_game.h"
#include "Tank.h"

Missile_teleport::Missile_teleport()
{
}

void Missile_teleport::init(Vec2 pt, Tank* pTank, Scene_game* pScene)
{
	m_sprMissile = Sprite::create("ball.png");
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
	m_iRad = 1;
	m_iDamage = 0;
}
void  Missile_teleport::explosion()
{
	Vec2 pt = m_sprMissile->getPosition();
	m_pTank->setPosition(Vec2(pt.x,pt.y+25));
	destroy();
	m_sprMissile->setVisible(false);
	m_bExist = false;
}
Missile_teleport::~Missile_teleport()
{
}
