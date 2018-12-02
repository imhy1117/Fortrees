#include "Missile.h"
#include "Tank.h"
#include "Scene_game.h"
#include "FieldManager.h"
#define GRAVITY 10
#define SHOT_SPEED 10
#define SHOT_POWER 1.5
Missile::Missile()
{
}

void Missile::init(Vec2 pt, Tank* pTank, Scene_game* pScene)
{
	m_sprMissile = Sprite::create("ball.png");
	m_sprMissile->setPosition(pt);
	m_pt = pt;
	m_sprMissile->retain();
	pScene->getGameLayer()->addChild(m_sprMissile, 3);
	m_fWind = 0;
	m_fPower = 0;
	m_fTime = 0;
	m_iDegree=0;
	m_bExist = true;
	m_bPowerup = false;
	m_pTank = pTank;
	m_pScene = pScene;
	m_iRad = 70;
	m_iDamage = 100;
}
void Missile::fire(int degree, float power, float wind)
{
	m_iDegree = degree;
	m_fPower = SHOT_POWER*power;
	m_fWind = wind;
}
void Missile::update(float delta)
{
	if (m_bExist)
	{
		m_fTime += delta*SHOT_SPEED;
		float radian = m_iDegree*3.14 / 180;
		float x = (m_fPower*cos(radian)*m_fTime+(m_fTime*m_fTime*m_pScene->getWind() / 2));
		float y = (m_fPower*sin(radian)*m_fTime) - ((GRAVITY*m_fTime*m_fTime) / 2);
		m_sprMissile->setPosition(Vec2(m_pt.x + x, m_pt.y + y));
		Vec2 pt = m_sprMissile->getPosition();
		m_pScene->setCamera(pt);
		if (pt.x>FieldManager::getInstance()->getMapSize().width+30||pt.x<-10||pt.y<-10)
		{
			destroy();
		}
	}
}
void  Missile::explosion()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("explosion.plist");
	Vector<SpriteFrame*> animFrames(4);
	char szFileName[256] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		sprintf(szFileName, "explosion%d.png", i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
		animFrames.pushBack(frame);
	}
	m_sprExplosion= Sprite::createWithSpriteFrameName("explosion0.png");
	m_sprExplosion->setPosition(m_sprMissile->getPosition());
	m_pScene->getGameLayer()->addChild(m_sprExplosion,4);
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.12f);
	Animate* animate = Animate::create(animation);
	auto cb = CallFunc::create(CC_CALLBACK_0(Missile::destroy, this));
	m_sprExplosion->runAction(Sequence::create(animate, cb, NULL));
	m_sprMissile->setVisible(false);
	m_bExist = false;
	FieldManager::getInstance()->explosion(m_sprMissile->getPosition(),m_iRad,m_iDamage);
}
void Missile::destroy()
{
	if (!m_bExist)
	{
		m_sprExplosion->removeFromParent();
	}
	m_sprMissile->removeFromParent();
	m_pTank->checkImpact();
	m_bExist = false;

}
Missile::~Missile()
{
}
