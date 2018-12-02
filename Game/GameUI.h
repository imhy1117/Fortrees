#include "cocos2d.h"
USING_NS_CC;

class Scene_game;
class Item_command;
class GameUI
{
private:
	Sprite* m_sprUI;
	Sprite* m_sprAngle;
	Sprite* m_sprMinDeg;
	Sprite* m_sprMaxDeg;
	Sprite* m_sprDestDeg;
	Sprite* m_sprMissile;
	Sprite* m_sprSpMissile;
	Item_command* m_pItem[4];
	Scene_game* m_pScene;
	ProgressTimer* m_guageHp;
	ProgressTimer* m_guageMove;
	ProgressTimer* m_guagePower;
	ProgressTimer* m_guageWindL;
	ProgressTimer* m_guageWindR;
	Layer* m_LayerUI;
	Label* m_labTimer;
	Label* m_labCamera;
	Label* m_labDegree;
	Label* m_labAngle;
	Repeat* m_processGuage;
	int m_iMinDeg, m_iMaxdeg, m_iDestDeg;
	int m_iCurHp;
	int m_iDestHp;
	bool m_bProcessHp;
	GameUI();
	static GameUI* pInstance;
public:
	static GameUI* getInstance();
	void init(Scene_game* pScene);
	void setTimer(int time);
	void setGuagePower(float percent);
	void setGuageHp(float percent);
	void setProgressHp(float percent);
	void setGuageMove(float percent);
	void setCamerapt(Vec2 pt);
	void setDegree(int degree);
	void setAngle(int dir,int angle);
	void setWind(int wind);
	void setDegreeLimit(int min, int max);
	void checkClick(Vec2 pt);
	void setMissileUI(Sprite* missile, Sprite* spMissile);
	void setItem(int slot, Item_command* item);
	void update();
	void displayGameOver();
	~GameUI();
};

