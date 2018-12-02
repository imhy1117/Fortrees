#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__
#include "cocos2d.h"
USING_NS_CC;
class Scene_title :public cocos2d::Scene
{
private:
	Sprite* m_sprBack;
	Sprite* m_sprStart;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onMouseDown(Event* event);
	CREATE_FUNC(Scene_title);
	~Scene_title();
};
#endif
