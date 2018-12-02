#include "Item_command.h"
class Item_health :
	public Item_command
{
public:
	Item_health();
	virtual void init(Scene_game* pScene, Tank* pTank);
	virtual void excute();
	virtual bool exist();
	virtual void lock();
	~Item_health();
};

