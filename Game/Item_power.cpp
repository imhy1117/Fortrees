#include "Item_power.h"
#include "Scene_game.h"
#include "Tank.h"


Item_power::Item_power()
{
}
void Item_power::init(Scene_game* pScene, Tank* pTank)
{
	m_pScene = pScene;
	m_pTank = pTank;
	m_bUsed = false;
	m_sprItem = Sprite::create("item_power.png");
	m_sprItem->retain();
	m_item=ITEM_POWER;
}
void Item_power::excute()
{
	if (m_pTank->usePower())
	{
		m_bUsed = true;
		m_sprItem->setVisible(false);
	}
}
bool Item_power::exist()
{
	if (m_bUsed)
		return false;
	else
		return true;
}


Item_power::~Item_power()
{
}
