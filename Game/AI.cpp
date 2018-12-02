#include "AI.h"
#include "Scene_game.h"
#include "Tank.h"
#include "Item_command.h"
#include "FieldManager.h"
AI::AI()
{
}

void AI::init(Scene_game* pScene, Tank* pTank)
{
	m_pScene = pScene;
	m_pTank = pTank;
	m_pTank->setPlayer(PLAYER_TYPE_AI);
	for (int i = 0; i < 4; i++)
	{
		m_pItem[i] = NULL;
	}
	m_iCurPower = 0;
	m_iLastPower = 0;
	m_bMyTurn = false;
	m_bUseItem = false;
	m_bUseTeleport = false;
	m_bHitUnit = false;
	m_pTank->setAI(this);
	m_aiProcess = AI_MOVE;
}
void AI::setFire()
{
	srand(GetTickCount());
	Vec2 myPt = m_pTank->getPosition();
	Vec2 playerPt = m_pScene->getPlayerTank()->getPosition();
	float width = FieldManager::getInstance()->getFieldSize().width;
	float distance = myPt.x - playerPt.x;
	int wind = m_pScene->getWind();
	if (distance < 0)
	{
		m_pTank->move(TANK_DIR_RIGHT);
		if (wind > 0)
		{
			wind *= -1;
		}
	}
	if (distance >= 0)
	{
		m_pTank->move(TANK_DIR_LEFT);
		if (wind < 0)
		{
			wind *= -1;
		}
	}
	m_iCurPower = 0;
	if (m_bUseTeleport)
	{
		m_iCurPower = rand() % 90 + 10;
	}
	else
	{
		m_iCurPower = (int)((fabs(distance) / width) * 150)+(rand()%6-3)+(wind*5);
		if (m_iCurPower > 100)
			m_iCurPower = 100;
		else if (m_iCurPower < 20)
			m_iCurPower = 20;
	}
}
void  AI::fire()
{
	m_pTank->fire(m_iCurPower);
	m_iLastPower = m_iCurPower;
	m_pScene->stopTimer();
}
void AI::setDegree()
{
	Vec2 myPt = m_pTank->getPosition();
	Vec2 playerPt = m_pScene->getPlayerTank()->getPosition();
	float x = fabs(playerPt.x - myPt.x);
	float y = playerPt.y - myPt.y;
	if (y > 100)
	{
		double rad = atan2(y, x);
		double deg = CC_RADIANS_TO_DEGREES(rad);
		m_pTank->setAIdegree((int)deg);
	}
	else
	{
		m_pTank->setAIdegree(40);
	}
}
void AI::setAngle()
{
	int angle = m_pTank->getAngle();
	if (m_pTank->getMove() > 0)
	{
		if (angle < -10)
		{
			m_pTank->move(TANK_DIR_LEFT);
			return;
		}
		else if (angle > 10)
		{
			m_pTank->move(TANK_DIR_RIGHT);
			return;
		}
	}
		m_aiProcess = AI_FIRE;
		auto delay = DelayTime::create(0.5f);
		auto cbItem = CallFunc::create(CC_CALLBACK_0(AI::useItem, this));
		auto cbSetDegree = CallFunc::create(CC_CALLBACK_0(AI::setDegree, this));
		auto cbSetFire = CallFunc::create(CC_CALLBACK_0(AI::setFire, this));
		auto cbFire= CallFunc::create(CC_CALLBACK_0(AI::fire, this));
		m_pScene->runAction(Sequence::create(delay, cbItem, delay, cbSetDegree,delay,cbSetFire, delay,cbFire,NULL));
}
void AI::getMyTurn()
{
	m_bMyTurn = true;
	m_bUseItem = false;
	m_bUseTeleport = false;
	m_aiProcess = AI_MOVE;
	for (int i = 0; i < 4; i++)
	{
		m_pItem[i] = m_pTank->getItem(i);
	}
	
}
void AI::move()
{
	Vec2 myPt = m_pTank->getPosition();
	Vec2 playerPt = m_pScene->getPlayerTank()->getPosition();
	float distance = myPt.x - playerPt.x;
	if (m_pTank->getAngle() > 45 || m_pTank->getAngle() < 45)
		m_aiProcess = AI_SETANGLE;
	else if (distance < 0 && fabs(distance) < 100)
	{
		m_pTank->move(TANK_DIR_LEFT);
	}
	else if (distance >= 0 && fabs(distance) < 100)
	{
		m_pTank->move(TANK_DIR_RIGHT);
	}
	else
		m_aiProcess = AI_SETANGLE;
}

void AI::update()
{
	if (m_bMyTurn)
	{
		if (m_aiProcess == AI_MOVE)
		{
			move();
		}
		else if (m_aiProcess == AI_SETANGLE)
		{
			setAngle();
		}
	}
}
void AI::useItem()
{
	srand(GetTickCount());
	if (!m_bUseItem)
	{
		if (m_pTank->getHp() < 40)
		{
			for (int i = 0; i < 4; i++)
			{
				if (m_pItem[i]->getItemInfo() == ITEM_HEALTH)
				{
					if (m_pItem[i]->exist())
					{
						m_pItem[i]->excute();
						m_bUseItem = true;
						return;
					}
				}
			}
		}
		if (m_bHitUnit)
		{
			for (int i = 0; i < 4; i++)
			{
				if (m_pItem[i]->getItemInfo() == ITEM_DOUBLE||ITEM_POWER)
				{
					if (m_pItem[i]->exist())
					{
						m_pItem[i]->excute();
						m_bUseItem = true;
						return;
					}
				}
			}
		}
		if (rand() % 100 < 20)
		{
			for (int i = 0; i < 4; i++)
			{
				if (m_pItem[i]->getItemInfo() == ITEM_TELEPORT)
				{
					if (m_pItem[i]->exist())
					{
						m_pItem[i]->excute();
						m_bUseItem = true;
						m_bUseTeleport = true;
						return;
					}
				}
			}
		}
	}
}
AI::~AI()
{
}
