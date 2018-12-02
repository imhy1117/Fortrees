#include "cocos2d.h"
USING_NS_CC;
enum AI_PROCESS
{
	AI_MOVE,
	AI_SETANGLE,
	AI_FIRE
};
class Scene_game;
class Tank;
class Item_command;
class AI
{
private:
	Scene_game* m_pScene;
	Tank* m_pTank;
	Item_command* m_pItem[4];
	AI_PROCESS m_aiProcess;
	bool m_bMyTurn;
	bool m_bUseItem;
	bool m_bHitUnit;
	bool m_bUseTeleport;
	int m_iLastPower;
	int m_iCurPower;
public:
	AI();
	void init(Scene_game* pScene,Tank* pTank);
	void getMyTurn();
	void useItem();
	void update();
	void setFire();
	void fire();
	void move();
	void setDegree();
	void setAngle();
	inline void checkHit(bool hit)
	{
		m_bHitUnit = hit;
	}
	inline void handOver()
	{
		m_bMyTurn = false;
	}
	~AI();
};

