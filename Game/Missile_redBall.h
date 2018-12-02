#include "Missile.h"
class Missile_redBall :
	public Missile
{
public:
	Missile_redBall();
	void init(Vec2 pt, Tank* pTank, Scene_game* pScene);
	~Missile_redBall();
};

