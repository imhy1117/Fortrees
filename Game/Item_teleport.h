
#include "Item_command.h"
class Item_teleport :
	public Item_command
{
public:
	Item_teleport();
	virtual void init(Scene_game* pScene, Tank* pTank);
	virtual void excute();
	virtual bool exist();
	~Item_teleport();
};

