#include "Tank.h"
#include "Missile_redBall.h"
#include "Missile_teleport.h"
#include "Scene_game.h"
#include "FieldManager.h"
#include "MissileManager.h"
#include "GameUI.h"
#include "Item_health.h"
#include "Item_double.h"
#include "Item_power.h"
#include "Item_teleport.h"
#include "AI.h"
Tank::Tank()
{
}
void Tank::init(Scene_game* pScene)
{
	m_pScene = pScene;
	m_AI = NULL;
	m_sprUnit = Sprite::create("tank.png");
	m_pScene->getGameLayer()->addChild(m_sprUnit, 3);
	m_fMove = 100;
	m_iPower = 0;
	m_iCharge = 0;
	m_iTankAngle = 0;
	m_iLeftShoot = 1;
	m_tankMissile = TANK_MISSILE_NORMAL;
	m_playerType = PLAYER_TYPE_USER;
	m_fHp = 100;
	m_iDegree = 30;
	m_iMaxDegree = 50;
	m_iMinDegree=10;
	m_iAngleLimit=45;
	m_bMyTurn = false;
	m_bMove = false;
	m_bDebug = false;
	m_bUsedItem = false;
	m_bUseTeleport = false;
	m_bUsePower = false;
	m_iTankAngle = 0;
	m_tankDir=TANK_DIR_LEFT;
	m_tankStat = TANK_STAT_IDLE;
	m_sprHpBar = Sprite::create("hpBar.png");
	m_pScene->getGameLayer()->addChild(m_sprHpBar,3);
	m_guageHp = ProgressTimer::create(Sprite::create("hp_green.png"));
	m_guageHp->setType(ProgressTimer::Type::BAR);
	m_guageHp->setAnchorPoint(Vec2(0, 0));
	m_guageHp->setMidpoint(Vec2(0, 0));
	m_guageHp->setBarChangeRate(Vec2(1, 0));
	m_guageHp->setPosition(Vec2(1, 1));
	m_guageHp->setPercentage(100.0f);
	m_sprHpBar->addChild(m_guageHp);
	m_labName= Label::createWithSystemFont("Player", "Amatic SC", 22);
	m_labName->setColor(Color3B(0, 255, 0));
	m_pScene->getGameLayer()->addChild(m_labName,3);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cursor.plist");
	Vector<SpriteFrame*> animFrames(4);
	char szFileName[256] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		sprintf(szFileName, "cursor_%d.png", i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFileName);
		animFrames.pushBack(frame);
	}
	m_sprCursor = Sprite::createWithSpriteFrameName("cursor_0.png");
	m_pScene->getGameLayer()->addChild(m_sprCursor,3);
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	Animate* animate = Animate::create(animation);
	m_sprCursor->runAction(RepeatForever::create(animate));
	m_sprCursor-> setVisible(false);
	if (m_bDebug)
	{
		m_sprHitBox = Sprite::create("hitbox.png");
		m_pScene->getGameLayer()->addChild(m_sprHitBox);
		m_sprHitBox->setPosition(m_sprUnit->getPosition());
	}
	m_sprMissile = Sprite::create("ball_select.png");
	m_sprMissile->retain();
	m_sprSpMissile = Sprite::create("redball_select_n.png");
	m_sprSpMissile->retain();
	for (int i = 0; i < 4; i++)
	{
		m_pItem[i] = NULL;
	}
	m_labDamage = NULL;
}
void Tank::setItem(int slot, Item_command* item)
{
	if (slot < 4&&slot>=0)
	{
		if (m_pItem[slot] != NULL)
		{
			m_pItem[slot]->release();
			delete m_pItem[slot];
		}
		m_pItem[slot] = item;
		m_pItem[slot]->init(m_pScene, this);
	}
}
void Tank::setPosition(Vec2 pt)
{
	m_sprUnit->setPosition(pt);
}

void Tank::setAIdegree(int degree)
{
	m_iDegree = degree;
	if (m_iDegree > m_iMaxDegree)
		m_iDegree = m_iMaxDegree;
	else if (m_iDegree < m_iMinDegree)
		m_iDegree = m_iMinDegree;
}

void Tank::setDegree(int degree)
{
	if (m_tankStat == TANK_STAT_MYTURN)
	{
		switch (degree)
		{
		case INPUT_UP:
			if (m_iDegree < m_iMaxDegree)
				m_iDegree++;
			break;
		case INPUT_DOWN:
			if (m_iDegree > m_iMinDegree)
				m_iDegree--;
			break;
		}
		if(m_playerType == PLAYER_TYPE_USER)
		GameUI::getInstance()->setDegree(m_iDegree);
	}
}
void Tank::changeMissile()
{
	if (m_tankStat == TANK_STAT_MYTURN)
	{
		m_tankMissile *= -1;
		setMissileUI();
	}	
}
void Tank::setMissileUI()
{
	if (m_tankMissile == TANK_MISSILE_NORMAL)
	{
		m_sprMissile->setTexture(Director::getInstance()->getTextureCache()->addImage("ball_select.png"));
		m_sprSpMissile->setTexture(Director::getInstance()->getTextureCache()->addImage("redball_select_n.png"));
	}
	else if (m_tankMissile == TANK_MISSILE_SPECIAL)
	{
		m_sprMissile->setTexture(Director::getInstance()->getTextureCache()->addImage("ball_select_n.png"));
		m_sprSpMissile->setTexture(Director::getInstance()->getTextureCache()->addImage("redball_select.png"));
	}
}
bool Tank::checkDamage(Vec2 pt, int r,int damage)
{
	if (m_tankStat != TANK_STAT_DESTROY)
	{
		Vec2 tmp = m_sprUnit->getPosition();
		float result = sqrt(pow(pt.x - tmp.x, 2) + pow(pt.y - tmp.y, 2));
		if (result > r)
			return false;
		else
		{
			float f_damage = (damage - result) / 3;
			m_fHp -= f_damage;
			if (m_fHp < 30)
			{
				m_guageHp->setSprite(Sprite::create("hp_red.png"));
			}
			if (m_fHp < 0)
			{
				m_fHp = 0;
				m_tankStat = TANK_STAT_DESTROY;
				m_sprUnit->setTexture(Director::getInstance()->getTextureCache()->addImage("tank_dead.png"));
			}
			if(m_labDamage!=NULL)
				m_labDamage->removeFromParent();
			m_labDamage = Label::createWithSystemFont("0000", "Amatic SC", 22);
			m_labDamage->setColor(Color3B(255, 0, 0));
			int displayDamage = (int)(f_damage * 10);
			char ch_damage[6];
			sprintf(ch_damage, "-%d", displayDamage);
			m_labDamage->setString(ch_damage);
			m_pScene->getGameLayer()->addChild(m_labDamage);
			m_labDamage->setPosition(Vec2(m_sprUnit->getBoundingBox().getMidX(),m_sprUnit->getBoundingBox().getMaxY()));
			auto moveby = MoveBy::create(1, Vec2(0, 10));
			auto cbDamage = CallFunc::create(CC_CALLBACK_0(Tank::releaseDamage, this));
			m_labDamage->runAction(Sequence::create(moveby, cbDamage,NULL));
			m_guageHp->setPercentage(m_fHp);
			if(m_playerType==PLAYER_TYPE_USER)
			GameUI::getInstance()->setProgressHp(m_fHp);
			return true;
		}
	}
	return false;
}
void Tank::releaseDamage()
{
	m_labDamage->removeFromParent();
	m_labDamage = NULL;
}
void Tank::setUnitAngle()
{
	bool block = false;
	float degree = 0;
	int tmpX = 0;
	for (int i = 1; i <20; i++)
	{
		for (int x = (int)m_sprUnit->getBoundingBox().getMinX(); x < (int)m_sprUnit->getBoundingBox().getMinX()+5; x++)
		{
			if (FieldManager::getInstance()->checkBin(Vec2(x,(int)m_sprUnit->getBoundingBox().getMinY() - i)))
			{
				block = true;
				tmpX = x;
				break;
			}
		}
		if (i == 19)
			block = true;
		if (block)
		{
			if (i > 1&&tmpX>0)
			{
				float x = tmpX-(int)m_sprUnit->getBoundingBox().getMidX();
				float y = -i;
				double rad = atan2(y, x);
				double deg = CC_RADIANS_TO_DEGREES(rad)+180;
				degree -= deg;
			}
			break;
		}
	}
	block = false;
	tmpX = 0;
	for (int i = 1; i <20; i++)
	{
		for (int x = (int)m_sprUnit->getBoundingBox().getMaxX()-5; x < (int)m_sprUnit->getBoundingBox().getMaxX(); x++)
		{
			if (FieldManager::getInstance()->checkBin(Vec2(x, (int)m_sprUnit->getBoundingBox().getMinY() - i)))
			{
				block = true;
				tmpX = x;
				break;
			}
		}
		if (i == 19)
			block = true;
		if (block)
		{
			if (i > 1&&tmpX>0)
			{
				float x = tmpX-(int)m_sprUnit->getBoundingBox().getMidX();
				float y = -i;
				double rad = atan2(y, x);
				double deg = CC_RADIANS_TO_DEGREES(rad);
				degree-= deg;
			}
			break;
		}
	}
	m_iTankAngle = (int)degree;
	m_sprUnit->setRotation(m_iTankAngle);
}
void Tank::move(int dir)
{
	if (m_tankStat == TANK_STAT_MYTURN&&!m_bMove)
	{
		MoveBy* moveBy;
		bool b_move = true;
		int m_iClimb = 0;
		switch (dir)
		{
		case INPUT_LEFT:
			if (m_sprUnit->getPosition().x > 10)
			{
				for (int i = 1; i < 7; i++)
				{
					if (!FieldManager::getInstance()->checkBin(Vec2((int)m_sprUnit->getBoundingBox().getMidX() - 1, (int)m_sprUnit->getBoundingBox().getMinY() + i)))
					{
						m_iClimb = i;
						break;
					}
				}
			}
			m_sprUnit->setFlippedX(false);
			m_tankDir = TANK_DIR_LEFT;
			if (m_iTankAngle > m_iAngleLimit)
			{
				b_move = false;
			}
			break;
		case INPUT_RIGHT:
			if (m_sprUnit->getPosition().x < FieldManager::getInstance()->getMapSize().width)
			{
				for (int i = 1; i < 7; i++)
				{
					if (!FieldManager::getInstance()->checkBin(Vec2((int)m_sprUnit->getBoundingBox().getMidX() + 1, (int)m_sprUnit->getBoundingBox().getMinY() + i)))
					{
						m_iClimb = i;
						break;
					}
				}
			}
			m_sprUnit->setFlippedX(true);
			m_tankDir = TANK_DIR_RIGHT;
			if (m_iTankAngle < -m_iAngleLimit)
			{
				b_move = false;
			}
			break;
		}
		if (b_move&&m_fMove > 0)
		{
			m_fMove-=0.5f;
			if (m_fMove < 0)
				m_fMove = 0;
			m_bMove = true;
			auto cb = CallFunc::create(CC_CALLBACK_0(Tank::cbMove, this));
			auto angle = CallFunc::create(CC_CALLBACK_0(Tank::setUnitAngle, this));
			moveBy = MoveBy::create(1 / 60, Vec2(m_tankDir, m_iClimb));
			m_sprUnit->runAction(Sequence::create(moveBy, angle, cb, NULL));
			if(m_playerType == PLAYER_TYPE_USER)
			GameUI::getInstance()->setGuageMove(m_fMove);
			
		}
		m_pScene->setCamera(m_sprUnit->getPosition());
	}
}
void Tank::checkField()
{
	if (m_tankStat != TANK_STAT_FALLDOWN)
	{
		if (m_sprUnit->getBoundingBox().getMinY() > 1)
		{
			bool b_check = true;
			int i_dropSpeed = 0;
			for(int i=0;i<7;i++)
			{
				if (FieldManager::getInstance()->checkBin(Vec2((int)m_sprUnit->getBoundingBox().getMidX(), (int)m_sprUnit->getBoundingBox().getMinY()- i)))
				{
					i_dropSpeed = i;
					break;
				}
				i_dropSpeed++;
			}
			if (i_dropSpeed > 0)
			{
				m_bMove = true;
				auto fall = MoveBy::create(1 / 60, Vec2(0, -1 * i_dropSpeed));
				auto cb= CallFunc::create(CC_CALLBACK_0(Tank::cbMove, this));
				auto angle = CallFunc::create(CC_CALLBACK_0(Tank::setUnitAngle, this));
				m_sprUnit->runAction(Sequence::create(fall,cb,angle,NULL));
			}
		}
		else
		{
			m_bMove = true;
			auto cb = CallFunc::create(CC_CALLBACK_0(Tank::cbMove, this));
			auto fall = MoveBy::create(1 / 60, Vec2(0, -10));
			m_sprUnit->runAction(Sequence::create(fall, cb, NULL));
			if (m_sprUnit->getBoundingBox().getMinY() < -20)
			{
				m_tankStat = TANK_STAT_FALLDOWN;
				if(m_bMyTurn)
					handOver();
			}
		}
	}
}
void Tank::cbMove()
{
	if (m_bMove)
	{
		m_bMove = false;
	}
}
void Tank::charge()
{
	if(m_tankStat==TANK_STAT_MYTURN&&!m_bMove)
	m_tankStat = TANK_STAT_CHARGE;
}
void Tank::update()
{
	checkField();
	if (m_playerType == PLAYER_TYPE_AI)
	{
		m_AI->update();
	}
	if (m_tankStat == TANK_STAT_CHARGE)
	{
		m_iCharge++;
		if (m_iCharge >= 100)
		{
			m_iCharge = 100;
			fire();
		}
		if(m_playerType==PLAYER_TYPE_USER)
		GameUI::getInstance()->setGuagePower(m_iCharge);
	}
	if (m_bMyTurn&&m_playerType == PLAYER_TYPE_USER)
		GameUI::getInstance()->setAngle(m_tankDir,m_iTankAngle);
	Vec2 pt = m_sprUnit->getPosition();
	m_sprHpBar->setPosition(Vec2(pt.x, pt.y - 30));
	m_sprCursor->setPosition(Vec2(pt.x, pt.y + 70));
	m_labName->setPosition(Vec2(pt.x, pt.y - 50));
	if (m_bDebug)
	{
		m_sprHitBox->setPosition(m_sprUnit->getPosition());
	}
}
void Tank::getMyTurn()
{
	m_tankStat = TANK_STAT_MYTURN;
	m_bMyTurn = true;
	m_bUsedItem = false;
	m_bUseTeleport = false;
	m_bUsePower = false;
	m_fMove = 100;
	m_iPower = 0;
	m_iCharge = 0;
	m_iLeftShoot = 1;
	if (m_playerType == PLAYER_TYPE_USER)
	{
		GameUI::getInstance()->setGuageHp(m_fHp);
		GameUI::getInstance()->setDegreeLimit(m_iMinDegree, m_iMaxDegree);
		GameUI::getInstance()->setDegree(m_iDegree);
		GameUI::getInstance()->setMissileUI(m_sprMissile, m_sprSpMissile);
		for (int i = 0; i < 4; i++)
		{
			if(m_pItem[i]!=NULL)
			GameUI::getInstance()->setItem(i, m_pItem[i]);
		}
	}
	if (m_playerType == PLAYER_TYPE_AI)
	{
		if (m_AI != NULL)
			m_AI->getMyTurn();
	}
	m_pScene->setCamera(m_sprUnit->getPosition());
	m_pScene->makeTimer();
	m_sprCursor->setVisible(true);
	
}
void Tank::checkImpact()
{
	m_iLeftShoot--;
	if (m_iLeftShoot == 0)
	{
		if(m_tankStat!=TANK_STAT_DESTROY)
			m_tankStat = TANK_STAT_IDLE;
		auto delay = DelayTime::create(1);
		auto cbHandover = CallFunc::create(CC_CALLBACK_0(Tank::handOver, this));
		m_sprUnit->runAction(Sequence::create(delay, cbHandover, NULL));
	}
	else
	{
		m_tankStat = TANK_STAT_CHARGE;
		fire();
	}
}
void Tank::handOver()
{
	if (m_bMyTurn)
	{
		m_bMyTurn = false;
		m_sprCursor->setVisible(false);
		m_pScene->handOverTurn();
		if (m_playerType == PLAYER_TYPE_AI)
		{
			if (m_AI != NULL)
				m_AI->handOver();
		}
	}
}
void Tank::releaseItem()
{
	m_sprUseItem->removeFromParent();
}
bool Tank::useDouble()
{
	if (!m_bUsedItem&&m_bMyTurn)
	{
		m_iLeftShoot = 2;
		m_bUsedItem = true;
		m_sprUseItem = Sprite::create("item_double_use.png");
		Vec2 pt = m_sprUnit->getPosition();
		m_sprUseItem->setPosition(pt.x + 30, pt.y + 20);
		m_pScene->getGameLayer()->addChild(m_sprUseItem,4);
		auto cb = CallFunc::create(CC_CALLBACK_0(Tank::releaseItem, this));
		auto delay = DelayTime::create(1);
		m_sprUnit->runAction(Sequence::create(delay, cb, NULL));
		return true;
	}
	else
		return false;
}
bool Tank::usePower()
{
	if (!m_bUsedItem&&m_bMyTurn)
	{
		m_bUsePower = true;
		m_bUsedItem = true;
		m_sprUseItem = Sprite::create("item_power_use.png");
		Vec2 pt = m_sprUnit->getPosition();
		m_sprUseItem->setPosition(pt.x + 30, pt.y + 20);
		m_pScene->getGameLayer()->addChild(m_sprUseItem,4);
		auto cb = CallFunc::create(CC_CALLBACK_0(Tank::releaseItem, this));
		auto delay = DelayTime::create(1);
		m_sprUnit->runAction(Sequence::create(delay, cb, NULL));
		return true;
	}
	else
		return false;
}
bool Tank::useTeleport()
{
	if (!m_bUsedItem&&m_bMyTurn)
	{
		m_bUseTeleport = true;
		m_bUsedItem = true;
		m_sprUseItem = Sprite::create("item_teleport_use.png");
		Vec2 pt = m_sprUnit->getPosition();
		m_sprUseItem->setPosition(pt.x + 30, pt.y + 20);
		m_pScene->getGameLayer()->addChild(m_sprUseItem,4);
		auto cb = CallFunc::create(CC_CALLBACK_0(Tank::releaseItem, this));
		auto delay = DelayTime::create(1);
		m_sprUnit->runAction(Sequence::create(delay, cb, NULL));
		return true;
	}
	else
		return false;
}
bool Tank::useHealth()
{
	if (!m_bUsedItem&&m_bMyTurn)
	{
		m_bUsedItem = true;
		m_fHp += 50;
		if (m_fHp > 100)
			m_fHp = 100;
		m_sprUseItem = Sprite::create("item_heal_use.png");
		Vec2 pt = m_sprUnit->getPosition();
		m_sprUseItem->setPosition(pt.x + 30, pt.y + 20);
		m_pScene->getGameLayer()->addChild(m_sprUseItem,4);
		auto cb = CallFunc::create(CC_CALLBACK_0(Tank::releaseItem, this));
		auto delay = DelayTime::create(1);
		m_sprUnit->runAction(Sequence::create(delay, cb, NULL));
		if(m_playerType==PLAYER_TYPE_USER)
			GameUI::getInstance()->setProgressHp(m_fHp);
		m_guageHp->setPercentage(m_fHp);
		m_guageHp->setSprite(Sprite::create("hp_green.png"));
		return true;
	}
	else
		return false;
}
void Tank::fire()
{
	if (m_tankStat == TANK_STAT_CHARGE)
	{
		Missile* pNew;
		if (m_bUseTeleport)
			pNew = new Missile_teleport();
		else if (m_tankMissile == TANK_MISSILE_NORMAL)
			pNew = new Missile();
		else if (m_tankMissile == TANK_MISSILE_SPECIAL)
			pNew = new Missile_redBall();
		pNew->init(m_sprUnit->getPosition(), this,m_pScene);
		if (m_bUsePower)
			pNew->powerUp();
		MissileManager::getInstance()->add(pNew);
		switch (m_tankDir)
		{
		case TANK_DIR_LEFT:
			pNew->fire(180 - m_iDegree+(m_iTankAngle*-1), m_iCharge, 0);
			break;
		case TANK_DIR_RIGHT:
			pNew->fire(m_iDegree + (m_iTankAngle*-1), m_iCharge, 0);
			break;
		}
		m_tankStat = TANK_STAT_FIRE;
		m_sprCursor->setVisible(false);
	}
}
void Tank::fire(int power)
{
	m_iCharge = power;
	m_tankStat = TANK_STAT_CHARGE;
	fire();
}
void  Tank::setPlayer(PLAYER_TYPE type)
{
	m_playerType = type;
	if (m_playerType == PLAYER_TYPE_AI)
	{
		m_labName->setString("AI");
		m_labName->setColor(Color3B(255, 0, 0));
	}
	else
	{
		m_labName->setString("Player");
		m_labName->setColor(Color3B(0, 255, 0));
	}
}
void  Tank::setAI(AI* ai)
{
	if (m_playerType == PLAYER_TYPE_AI)
	m_AI = ai;
}
void Tank::release()
{
	m_sprMissile->release();
	m_sprSpMissile->release();
	for (int i = 0; i < 4; i++)
	{
		if (m_pItem[i] != NULL)
		{
			m_pItem[i]->release();
			delete m_pItem[i];
			m_pItem[i] = NULL;
		}
	}
}
Tank::~Tank()
{
}
