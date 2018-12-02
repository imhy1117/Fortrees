#include "Scene_title.h"
#include "Scene_game.h"


Scene* Scene_title::createScene()
{
	return Scene_title::create();
}
bool Scene_title::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Director::getInstance()->setContentScaleFactor(1.0f);
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Scene_title::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	m_sprBack = Sprite::create("title.png");
	m_sprBack->setAnchorPoint(Vec2(0, 0));
	this->addChild(m_sprBack);
	m_sprStart = Sprite::create("start.png");
	auto blink = Blink::create(1, 1);
	auto repeat = RepeatForever::create(blink);
	m_sprStart->runAction(repeat);
	this->addChild(m_sprStart);
	m_sprStart->setPosition(Vec2(visibleSize.width/2,150));
	return true;
}
void Scene_title::onMouseDown(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	Vec2 pt = Vec2(e->getCursorX(), e->getCursorY());
	auto button = e->getMouseButton();
	if (button == EventMouse::MouseButton::BUTTON_LEFT)
	{
		if (m_sprStart->getBoundingBox().containsPoint(pt))
		{
			auto scene = TransitionFade::create(3, Scene_game::createScene());
			Director::getInstance()->replaceScene(scene);
		}
	}
}
Scene_title::~Scene_title()
{
}
