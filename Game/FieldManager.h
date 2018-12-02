#include "cocos2d.h"
#include "DefineData.h"
USING_NS_CC;
class Scene_game;
class FieldManager
{
private:
	FieldManager();
	Sprite* m_sprField;
	Sprite* m_sprMap;
	Scene_game* m_pScene;
	ClippingNode* m_cpNode;
	Node* m_ndStencil;
	Size m_sizeField, m_sizeMap;
	std::vector<DWORD>m_vecMapData;
	Vector<Sprite*>m_vecWind;
	static FieldManager* pInstance;
public:
	static FieldManager* getInstance();
	void init(Scene_game* pScene);
	bool hitCheck(Rect rc);
	DWORD ptToBin(Vec2 pt);
	bool checkBin(Vec2 pt);
	void removeMap(Vec2 pt);
	void explosion(Vec2 pt,int r,int damage);
	void release();
	void loadData();
	inline Size getMapSize()
	{
		return m_sizeMap;
	}
	inline Size getFieldSize()
	{
		return m_sizeField;
	}
	~FieldManager();
};

