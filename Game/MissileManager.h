#include "cocos2d.h"
USING_NS_CC;
class Missile;
class MissileManager
{
private:
	static MissileManager* pInstance;
	std::vector<Missile*>m_vecMissile;
	Missile* m_pCurMissile;
	MissileManager();
public:
	static MissileManager* getInstance();
	void release();
	void add(Missile* pMissile);
	inline Missile* getCurMissile()
	{
		return m_pCurMissile;
	}
	void update(float delta);
	~MissileManager();
};

