#include "Item_command.h"
class Item_power :
	public Item_command
{
public:
	Item_power();
	virtual void init(Scene_game* pScene, Tank* pTank);
	virtual void excute();
	virtual bool exist();

	~Item_power();
};

