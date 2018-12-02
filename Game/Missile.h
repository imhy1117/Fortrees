#include "cocos2d.h"
#ifndef __MISSILE_H__
#define __MISSILE_H__
USING_NS_CC;
class Tank;
class Scene_game;
class Missile
{
protected:
	Sprite* m_sprMissile;
	Sprite* m_sprExplosion;
	float m_fWind, m_fPower, m_fTime;
	Vec2 m_pt;
	int m_iDegree;
	bool m_bExist;
	bool m_bPowerup;
	Tank* m_pTank;
	int m_iRad,m_iDamage;
	Scene_game* m_pScene;
public:
	Missile();
	virtual void init(Vec2 pt,Tank* pTank,Scene_game* pScene);
	virtual void fire(int degree,float power,float wind);
	virtual void update(float delta);
	virtual void explosion();
	virtual void destroy();
	inline virtual void powerUp()
	{
		m_bPowerup = true;
		m_iDamage *= 2;
	}
	inline virtual int getDamage()
	{
		return m_iDamage;
	}
	inline virtual int getRad()
	{
		return m_iRad;
	}
	inline virtual bool check()
	{
		return m_bExist;
	}
	inline virtual Rect getRect()
	{
		return m_sprMissile->getBoundingBox();
	}
	~Missile();
};

#endif // 