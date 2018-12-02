#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
USING_NS_CC;

enum GAME_INPUT
{
	INPUT_LEFT=-1,
	INPUT_RIGHT=1,
	INPUT_UP,
	INPUT_DOWN
};
enum GAMESTAT
{
	READY,
	RUN,
	GAMEOVER
};
class Missile;
class Tank;
class AI;
class Scene_game:public cocos2d::Scene
{
private:
	GAMESTAT m_gamestat;
	Vec2 m_ptCam,m_ptMouse;
	Tank* m_pTank;
	Tank* m_pPlayerTank;
	Layer* m_layerGame;
	std::vector<Tank*>m_vecTank;
	std::vector<AI*>m_vecAI;
	Vector<Sprite*>m_vecWind;
	RepeatForever* m_actTimer;
	int m_iTankNum, m_iTurn;
	float m_fPower;
	int m_iTimer;
	bool m_bInputLeft, m_bInputRight, m_bInputSpace;
	int m_iWind,m_iWindDir;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onMouseMove(Event* event);
	virtual void onMouseDown(Event* event);
	virtual void update(float delta);
	virtual void setCamera(Vec2 pt);
	virtual void setCamera();
	virtual void startGame();
	virtual void handOverTurn();
	virtual void checkDamage(Vec2 pt, int r,int damage);
	virtual void goToTitle();
	inline int getWind()
	{
		return m_iWind;
	}
	void progressTime();
	void stopTimer();
	void makeTimer();
	virtual void gameover();
	inline virtual Layer* getGameLayer()
	{
		return m_layerGame;
	}
	inline virtual Tank* getPlayerTank()
	{
		return m_pPlayerTank;
	}
	CREATE_FUNC(Scene_game);
	~Scene_game();
};

#endif