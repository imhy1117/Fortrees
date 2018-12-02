#include "Item_health.h"
#include "Scene_game.h"
#include "Tank.h"



Item_health::Item_health()
{
}
void Item_health::init(Scene_game* pScene, Tank* pTank)
{
	m_pScene = pScene;
	m_pTank = pTank;
	m_bUsed = false;
	m_sprItem = Sprite::create("item_heal.png");
	m_sprItem->retain();
	m_item = ITEM_HEALTH;
}
void Item_health::excute()
{
	if (m_pTank->useHealth())
	{
		m_bUsed = true;
		m_sprItem->setVisible(false);
	}
}
bool Item_health::exist()
{
	if (m_bUsed)
		return false;
	else
		return true;
}
void Item_health::lock()
{

}

Item_health::~Item_health()
{
}
