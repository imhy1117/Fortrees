#include "Scene_game.h"
#include "Scene_title.h"
#include "Tank.h"
#include "Missile.h"
#include "FieldManager.h"
#include "MissileManager.h"
#include "GameUI.h"
#include "Item_health.h"
#include "Item_double.h"
#include "Item_power.h"
#include "Item_teleport.h"
#include "AI.h"
#define RESOLUTION_WIDTH 1280
#define RESOLUTION_HEIGHT 720
#define CAMERA_MOVESPEED 15
Scene* Scene_game::createScene()
{
	return Scene_game::create();
}
bool Scene_game::init()
{
	scheduleUpdate();
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Director::getInstance()->setContentScaleFactor(1.0f);
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(Scene_game::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(Scene_game::onKeyReleased, this);
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(Scene_game::onMouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(Scene_game::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	m_fPower = 0;
	m_iWind = 0;
	m_iWindDir = 1;
	m_gamestat = RUN;
	m_iTimer = 0;
	srand(GetTickCount());
	m_layerGame = Layer::create();
	this->addChild(m_layerGame,0);
	GameUI::getInstance()->init(this);
	FieldManager::getInstance()->init(this);
	m_iTankNum = 4;
	for (int i = 0; i < m_iTankNum; i++)
	{
		Tank* pTank = new Tank();
		pTank->init(this);
		pTank->setPosition(Vec2((rand() % 1800) + 20, 700));
		pTank->setItem(0, new Item_double());
		pTank->setItem(1, new Item_health());
		pTank->setItem(2, new Item_teleport());
		pTank->setItem(3, new Item_power());
		m_vecTank.push_back(pTank);
	}
	m_iTurn=0;
	m_pTank = m_vecTank[0];
	m_pPlayerTank= m_vecTank[0];
	for (int i = 1; i <m_iTankNum;i++)
	{
		AI* pAI = new AI();
		pAI->init(this, m_vecTank[i]);
		m_vecAI.push_back(pAI);
	}
	auto delay = DelayTime::create(5.0f);
	auto cb= CallFunc::create(CC_CALLBACK_0(Scene_game::startGame, this));
	m_layerGame->runAction(Sequence::create(delay, cb, NULL));
	for (int i = 0; i < 30; i++)
	{
		Sprite* pNew = Sprite::create("wind.png");
		pNew->setPosition(rand() % (int)FieldManager::getInstance()->getMapSize().width, rand()%(int)FieldManager::getInstance()->getMapSize().height);
		m_layerGame->addChild(pNew, 1);
		m_vecWind.pushBack(pNew);
	}
	m_ptMouse.setPoint(RESOLUTION_WIDTH / 2, RESOLUTION_HEIGHT / 2);
	setCamera(m_ptMouse);
	return true;
}
void Scene_game::startGame()
{
	m_pTank->getMyTurn();
	m_iWind += rand() % 5 * m_iWindDir;
	GameUI::getInstance()->setWind(m_iWind);
}
void  Scene_game::makeTimer()
{
	if (m_actTimer != NULL)
	{
		stopTimer();
	}
	if (m_actTimer == NULL)
	{
		m_iTimer = 41;
		auto act = CallFunc::create(CC_CALLBACK_0(Scene_game::progressTime, this));
		auto delay = DelayTime::create(1.0f);
		auto seq = Sequence::create(act, delay, NULL);
		m_actTimer = RepeatForever::create(seq);
		m_layerGame->runAction(m_actTimer);
	}
}
void Scene_game::stopTimer()
{
	if (m_actTimer != NULL)
	{
		m_layerGame->stopAction(m_actTimer);
		m_actTimer = NULL;
	}
	
}
void Scene_game::progressTime()
{
	m_iTimer--;
	if (m_iTimer < 0)
	{
		stopTimer();
		m_pTank->handOver();
	}
	GameUI::getInstance()->setTimer(m_iTimer);
}
void Scene_game::checkDamage(Vec2 pt, int r,int damage)
{
	AI* ai = m_pTank->getAI();
	bool b_hitPlayer = false;
	for (auto iter = m_vecTank.begin(); iter != m_vecTank.end(); iter++)
	{
		if ((*iter)->checkDamage(pt, r, damage))
		{
			if (*iter ==m_pPlayerTank)
			{
				b_hitPlayer = true;
				if (ai != NULL)
				{
					ai->checkHit(true);
				}
					
			}
		}
	}
	if (ai != NULL&&!b_hitPlayer)
		ai->checkHit(false);
}
void Scene_game::onMouseMove(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	m_ptMouse.x = e->getCursorX();
	m_ptMouse.y = e->getCursorY();
}
void Scene_game::onMouseDown(Event* event)
{
	EventMouse* e = (EventMouse*)event;
	Vec2 pt = Vec2(e->getCursorX(), e->getCursorY());
	auto button = e->getMouseButton();
	if (button == EventMouse::MouseButton::BUTTON_LEFT)
		GameUI::getInstance()->checkClick(pt);
}
void Scene_game::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (m_gamestat == RUN)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			m_bInputLeft = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			m_bInputRight = true;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			m_pPlayerTank->setDegree(INPUT_UP);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			m_pPlayerTank->setDegree(INPUT_DOWN);
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			stopTimer();
			m_pPlayerTank->charge();
			m_bInputSpace = true;
			break;
		case EventKeyboard::KeyCode::KEY_TAB:
			m_pPlayerTank->changeMissile();
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			goToTitle();
			break;
		}
	}
}

void Scene_game::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (m_gamestat == RUN)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			m_bInputLeft = false;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			m_bInputRight = false;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			m_bInputSpace = false;
			m_pPlayerTank->fire();
			break;
		}
	}
}
void Scene_game::setCamera(Vec2 pt)
{
	m_ptCam.x = (RESOLUTION_WIDTH / 2) -pt.x;
	m_ptCam.y =(RESOLUTION_HEIGHT /2)- pt.y;
}
void Scene_game::setCamera()
{
	if (m_ptCam.x > 0)
		m_ptCam.x = 0;
	else if (m_ptCam.x <RESOLUTION_WIDTH - FieldManager::getInstance()->getMapSize().width)
		m_ptCam.x = RESOLUTION_WIDTH - FieldManager::getInstance()->getMapSize().width;
	if (m_ptCam.y >140)
		m_ptCam.y = 140;
	else if (m_ptCam.y < RESOLUTION_HEIGHT - FieldManager::getInstance()->getMapSize().height)
		m_ptCam.y = RESOLUTION_HEIGHT - FieldManager::getInstance()->getMapSize().height;
	m_layerGame->setPosition(m_ptCam);
}
void Scene_game::handOverTurn()
{
	if (m_gamestat == RUN)
	{
		int count = 0;
		for (auto iter = m_vecTank.begin(); iter != m_vecTank.end(); iter++)
		{
			if ((*iter)->checkAlive())
				count++;
		}
		if (count <= 1)
		{
			gameover();
			return;
		}
		if (!m_pPlayerTank->checkAlive())
		{
			gameover();
			return;
		}
		m_iTurn++;
		if (m_iTurn > m_iTankNum - 1)
		{
			m_iTurn = 0;
		}
		m_pTank = m_vecTank[m_iTurn];
		if (m_pTank->checkAlive())
		{
			m_pTank->getMyTurn();
			int windDir = rand() % 100;
			if (windDir < 50)
				m_iWindDir = 1;
			else
				m_iWindDir = -1;
			m_iWind += (rand() % 3 + 1) * m_iWindDir;
			if (m_iWind > 4)
			{
				m_iWind = 4;
			}
			else if (m_iWind < -4)
			{
				m_iWind = -4;
			}
			GameUI::getInstance()->setWind(m_iWind);
		}
		else
			handOverTurn();
	}
}
void Scene_game::update(float delta)
{
		if (m_ptMouse.x < 10)
			m_ptCam.x += CAMERA_MOVESPEED;
		else if (m_ptMouse.x > 1270)
			m_ptCam.x -= CAMERA_MOVESPEED;
		if (m_ptMouse.y < 10)
			m_ptCam.y += CAMERA_MOVESPEED;
		else if (m_ptMouse.y >710)
			m_ptCam.y -= CAMERA_MOVESPEED;
		if (m_bInputLeft)
			m_pPlayerTank->move(INPUT_LEFT);
		else if (m_bInputRight)
			m_pPlayerTank->move(INPUT_RIGHT);
		if (m_gamestat != GAMEOVER)
		{
			for (auto iter = m_vecTank.begin(); iter != m_vecTank.end(); iter++)
			{
				(*iter)->update();
			}
		}
	
		for (auto iter = m_vecWind.begin(); iter != m_vecWind.end(); iter++)
		{
			if ((*iter)->getPosition().y < 0)
				(*iter)->setPosition(rand() % (int)FieldManager::getInstance()->getMapSize().width, FieldManager::getInstance()->getMapSize().height+20);
			MoveBy* move = MoveBy::create(1 / 60, Vec2(m_iWind/2, -2));
			RotateBy* rotate = RotateBy::create(1 / 60, m_iWindDir);
			(*iter)->runAction(Spawn::create(move,rotate,NULL));
		}
	setCamera();
	MissileManager::getInstance()->update(delta);
	GameUI::getInstance()->setCamerapt(m_ptCam);
	GameUI::getInstance()->update();
}
void Scene_game::gameover()
{
	stopTimer();
	m_gamestat = GAMEOVER;
	auto delay = DelayTime::create(5);
	auto cbTitle = CallFunc::create(CC_CALLBACK_0(Scene_game::goToTitle, this));
	m_layerGame->runAction(Sequence::create(delay, cbTitle, NULL));
	GameUI::getInstance()->displayGameOver();
}
void Scene_game::goToTitle()
{
	auto scene = TransitionTurnOffTiles::create(1,Scene_title::createScene());
	Director::getInstance()->replaceScene(scene);
}
Scene_game::~Scene_game()
{
	for (auto iter = m_vecTank.begin(); iter != m_vecTank.end(); iter++)
	{
		(*iter)->release();
		delete *iter;
	}
	for (auto iter = m_vecAI.begin(); iter != m_vecAI.end(); iter++)
	{
		delete *iter;
	}
	m_vecWind.clear();
	m_vecAI.clear();
	MissileManager::getInstance()->release();
}

