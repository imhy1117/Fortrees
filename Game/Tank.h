#include "cocos2d.h"
USING_NS_CC;
enum TANK_DIR
{
	TANK_DIR_LEFT=-1,
	TANK_DIR_RIGHT=1
};
enum TANK_MISSILE
{
	TANK_MISSILE_NORMAL = -1,
	TANK_MISSILE_SPECIAL=1
};
enum TANK_STAT
{
	TANK_STAT_IDLE,
	TANK_STAT_MYTURN,
	TANK_STAT_CHARGE,
	TANK_STAT_FIRE,
	TANK_STAT_DESTROY,
	TANK_STAT_FALLDOWN
};
enum PLAYER_TYPE
{
	PLAYER_TYPE_USER,
	PLAYER_TYPE_AI
};
class Scene_game;
class Item_command;
class AI;
class Tank
{
private:
	Sprite* m_sprUnit;
	Sprite* m_sprHpBar;
	Sprite* m_sprCursor;
	Sprite* m_sprHitBox;
	Sprite* m_sprMissile;
	Sprite* m_sprSpMissile;
	Sprite* m_sprUseItem;
	Scene_game* m_pScene;
	Label* m_labName;
	ProgressTimer* m_guageHp;
	Item_command* m_pItem[4];
	AI* m_AI;
	int m_iDegree,m_iPower,m_iCharge;
	float m_fMove, m_fHp;
	int m_iTankAngle;
	int m_iLeftShoot;
	int m_iMaxDegree, m_iMinDegree;
	int m_iAngleLimit;
	Label* m_labDamage;
	bool m_bMyTurn,m_bMove;
	bool m_bDebug;
	bool m_bUsedItem;
	bool m_bUseTeleport;
	bool m_bUsePower;
	TANK_DIR m_tankDir;
	TANK_STAT m_tankStat;
	PLAYER_TYPE m_playerType;
	int m_tankMissile;
public:
	Tank();
	virtual void init(Scene_game* pScene);
	virtual void setDegree(int degree);
	virtual void move(int dir);
	virtual void checkField();
	virtual void charge();
	virtual void update();
	virtual void setUnitAngle();
	virtual void getMyTurn();
	virtual void checkImpact();
	virtual void handOver();
	virtual void setPosition(Vec2 pt);
	virtual bool checkDamage(Vec2 pt, int r,int damage);
	virtual void releaseDamage();
	virtual void cbMove();
	virtual void setItem(int slot,Item_command* item);
	virtual void setAIdegree(int degree);
	virtual bool useDouble();
	virtual bool usePower();
	virtual bool useTeleport();
	virtual bool useHealth();
	virtual void releaseItem();
	virtual void changeMissile();
	virtual void setMissileUI();
	inline virtual Vec2 getPosition()
	{
		return m_sprUnit->getPosition();
	}
	inline virtual bool checkAlive()
	{
		if (m_tankStat == TANK_STAT_DESTROY|| m_tankStat == TANK_STAT_FALLDOWN)
			return false;
		else
			return true;
	}
	virtual void setPlayer(PLAYER_TYPE type);
	virtual void setAI(AI* ai);
	virtual AI* getAI()
	{
		return m_AI;
	}
	virtual void fire();
	virtual void fire(int power);
	inline virtual float getHp()
	{
		return m_fHp;
	}
	inline virtual Item_command* getItem(int slot)
	{
		if (slot >= 0 && slot < 4)
			return m_pItem[slot];
		else
			return NULL;
	}
	inline virtual int getDegree()
	{
		return m_iDegree;
	}
	inline virtual int getAngle()
	{
		return m_iTankAngle;
	}
	inline virtual float getMove()
	{
		return m_fMove;
	}
	void release();
	~Tank();
};

