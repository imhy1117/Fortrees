#include "Item_command.h"
class Item_double :
	public Item_command
{

public:
	Item_double();
	virtual void init(Scene_game* pScene, Tank* pTank);
	virtual void excute();
	virtual bool exist();

	~Item_double();
};

