#include "cocos2d.h"
#ifndef __ITEM_COMMAND_H__
#define __ITEM_COMMAND_H__
enum ITEM_INDEX
{
	ITEM_DOUBLE,
	ITEM_HEALTH,
	ITEM_POWER,
	ITEM_TELEPORT,
};
USING_NS_CC;
class Tank;
class Scene_game;
class Item_command
{
protected:
	Sprite* m_sprItem;
	Tank* m_pTank;
	Scene_game* m_pScene; 
	ITEM_INDEX m_item;
	bool m_bUsed;
public:
	Item_command();
	~Item_command();
	virtual void init(Scene_game* pScene, Tank* pTank) = 0;
	virtual void excute() = 0;
	virtual bool exist() = 0;
	inline virtual void lock()
	{
		m_sprItem->setVisible(false);
	}
	inline virtual Sprite* getSprite()
	{
		return m_sprItem;
	}
	inline virtual void release()
	{
		m_sprItem->release();
	}
	inline virtual ITEM_INDEX getItemInfo()
	{
		return m_item;
	}
};
#endif // 
