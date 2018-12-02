#include "Item_double.h"
#include "Scene_game.h"
#include "Tank.h"


Item_double::Item_double()
{
}

void Item_double::init(Scene_game* pScene, Tank* pTank)
{
	m_pScene = pScene;
	m_pTank = pTank;
	m_bUsed = false;
	m_sprItem = Sprite::create("item_double.png");
	m_sprItem->retain();
	m_item = ITEM_DOUBLE;
}
void Item_double::excute()
{
	if (m_pTank->useDouble())
	{
		m_bUsed = true;
		m_sprItem->setVisible(false);
	}
}
bool Item_double::exist()
{
	if (m_bUsed)
		return false;
	else
		return true;
}

Item_double::~Item_double()
{
}
