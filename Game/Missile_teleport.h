#include "Missile.h"
class Missile_teleport :
	public Missile
{
public:
	Missile_teleport();
	void init(Vec2 pt, Tank* pTank, Scene_game* pScene);
	void explosion();
	~Missile_teleport();
};

