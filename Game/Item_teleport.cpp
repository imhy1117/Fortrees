#include "Item_teleport.h"
#include "Scene_game.h"
#include "Tank.h"



Item_teleport::Item_teleport()
{
}

void Item_teleport::init(Scene_game* pScene, Tank* pTank)
{
	m_pScene = pScene;
	m_pTank = pTank;
	m_bUsed = false;
	m_sprItem = Sprite::create("item_teleport.png");
	m_sprItem->retain();
	m_item = ITEM_TELEPORT;
}
void Item_teleport::excute()
{
	if (m_pTank->useTeleport())
	{
		m_bUsed = true;
		m_sprItem->setVisible(false);
	}
}
bool Item_teleport::exist()
{
	if (m_bUsed)
		return false;
	else
		return true;
}

Item_teleport::~Item_teleport()
{
}
