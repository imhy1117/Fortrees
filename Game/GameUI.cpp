#include "GameUI.h"
#include "Scene_game.h"
#include "Item_health.h"
#include "Item_double.h"
#include "Item_power.h"
#include "Item_teleport.h"
GameUI* GameUI::pInstance = NULL;
GameUI::GameUI()
{
}
GameUI* GameUI::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new GameUI();
	}
	return pInstance;
}
void GameUI:: init(Scene_game* pScene)
{
	m_LayerUI = Layer::create();
	pScene->addChild(m_LayerUI,1);
	m_sprUI = Sprite::create("UI.png");
	m_sprUI->setAnchorPoint(Vec2(0, 0));
	m_LayerUI->addChild(m_sprUI);
	m_guageHp = ProgressTimer::create(Sprite::create("Guage_hp.png"));
	m_guageHp->setType(ProgressTimer::Type::BAR);
	m_guageHp->setAnchorPoint(Vec2(0, 0));
	m_guageHp->setMidpoint(Vec2(0, 0));
	m_guageHp->setBarChangeRate(Vec2(1, 0));
	m_guageHp->setPosition(Vec2(450, 90));
	m_guageHp->setPercentage(100.0f);
	m_sprUI->addChild(m_guageHp);
	m_guagePower= ProgressTimer::create(Sprite::create("Guage_power.png"));
	m_guagePower->setType(ProgressTimer::Type::BAR);
	m_guagePower->setAnchorPoint(Vec2(0, 0));
	m_guagePower->setMidpoint(Vec2(0, 0));
	m_guagePower->setBarChangeRate(Vec2(1, 0));
	m_guagePower->setPosition(Vec2(450, 55));
	m_guagePower->setPercentage(100.0f);
	m_sprUI->addChild(m_guagePower);
	m_guageMove = ProgressTimer::create(Sprite::create("Guage_move.png"));
	m_guageMove->setType(ProgressTimer::Type::BAR);
	m_guageMove->setAnchorPoint(Vec2(0, 0));
	m_guageMove->setMidpoint(Vec2(0, 0));
	m_guageMove->setBarChangeRate(Vec2(1, 0));
	m_guageMove->setPosition(Vec2(450, 20));
	m_guageMove->setPercentage(100.0f);
	m_sprUI->addChild(m_guageMove);
	m_guageWindL= ProgressTimer::create(Sprite::create("windMeter.png"));
	m_guageWindL->setType(ProgressTimer::Type::BAR);
	m_guageWindL->setAnchorPoint(Vec2(1, 0));
	m_guageWindL->setMidpoint(Vec2(1, 0));
	m_guageWindL->setBarChangeRate(Vec2(1, 0));
	m_guageWindL->setPosition(Vec2(145, 10));/////////////////////////////
	m_guageWindL->setPercentage(0.0f);
	m_sprUI->addChild(m_guageWindL);
	m_guageWindR=ProgressTimer::create(Sprite::create("windMeter.png"));
	m_guageWindR->setType(ProgressTimer::Type::BAR);
	m_guageWindR->setAnchorPoint(Vec2(0, 0));
	m_guageWindR->setMidpoint(Vec2(0, 0));
	m_guageWindR->setBarChangeRate(Vec2(1, 0));
	m_guageWindR->setPosition(Vec2(149, 10));//////////////////////////////
	m_guageWindR->setPercentage(0.0f);
	m_sprUI->addChild(m_guageWindR);

	m_sprAngle = Sprite::create("angle.png");
	m_sprAngle->setPosition(Vec2(148, 105));
	m_sprUI->addChild(m_sprAngle);
	m_sprMinDeg=Sprite::create("degree.png");
	m_sprAngle->addChild(m_sprMinDeg);
	m_sprMinDeg->setPosition(Vec2(m_sprMinDeg->getBoundingBox().size.width/2, m_sprMinDeg->getBoundingBox().size.height / 2));
	m_sprMaxDeg = Sprite::create("degree.png");
	m_sprAngle->addChild(m_sprMaxDeg);
	m_sprMaxDeg->setPosition(Vec2(m_sprMaxDeg->getBoundingBox().size.width / 2, m_sprMaxDeg->getBoundingBox().size.height / 2));
	m_sprDestDeg = Sprite::create("destDegree.png");
	m_sprAngle->addChild(m_sprDestDeg);
	m_sprDestDeg->setPosition(Vec2(m_sprDestDeg->getBoundingBox().size.width / 2, m_sprDestDeg->getBoundingBox().size.height / 2));
	m_iDestDeg = 0;
	m_iMinDeg = 0;
	m_iMaxdeg = 0;

	m_labTimer = Label::createWithSystemFont("00", "Amatic SC", 60);
	m_labTimer->setPosition(Vec2(1200, 100));
	m_sprUI->addChild(m_labTimer);
	setTimer(0);
	m_labCamera = Label::createWithSystemFont("0000000000", "Amatic SC", 30);
	m_labCamera->setPosition(Vec2(1200, 700));
	m_sprUI->addChild(m_labCamera);
	m_labCamera->setVisible(false);
	m_labDegree = Label::createWithSystemFont("000", "Amatic SC", 20);
	m_labDegree->setPosition(Vec2(180, 70));
	m_sprUI->addChild(m_labDegree);
	m_labAngle = Label::createWithSystemFont("000", "Amatic SC", 20);
	m_labAngle->setPosition(Vec2(120, 70));
	m_sprUI->addChild(m_labAngle);
	m_sprMissile = NULL;
	m_sprSpMissile = NULL;
	m_bProcessHp = false;
	for (int i = 0; i < 4; i++)
	{
		m_pItem[i] = NULL;
	}

}
void GameUI::checkClick(Vec2 pt)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_pItem[i]->getSprite()->getBoundingBox().containsPoint(pt))
		{
			m_pItem[i]->excute();
			break;
		}
	}
}
void GameUI::setProgressHp(float percent)
{
	m_iDestHp = (int)percent;
	m_iCurHp= (int)m_guageHp->getPercentage();
	m_bProcessHp = true;
}
void  GameUI::update()
{
	if(m_bProcessHp)
	{
		if (m_iDestHp >m_iCurHp)
		{
			m_iCurHp++;
		}
		else if (m_iDestHp < m_iCurHp)
		{
			m_iCurHp--;
		}
		else if (m_iDestHp == m_iCurHp)
		{
			m_bProcessHp = false;
		}
		m_guageHp->setPercentage(m_iCurHp);
	}
}
void GameUI::setWind(int wind)
{
	char ch_wind[6];
	sprintf(ch_wind, "%d", wind);
	if (wind > 0)
	{
		m_guageWindL->setPercentage(0);
		m_guageWindR->setPercentage(wind * 25);
	}
	else if (wind < 0)
	{
		m_guageWindL->setPercentage(-wind * 25);
		m_guageWindR->setPercentage(0);
	}
	else if (wind == 0)
	{
		m_guageWindL->setPercentage(0);
		m_guageWindR->setPercentage(0);
	}

}
void GameUI::setCamerapt(Vec2 pt)
{
	int camptX = (int)pt.x;
	int camptY = (int)pt.y;
	char ch_pt[20];
	sprintf(ch_pt, "%d,%d", camptX, camptY);
	m_labCamera->setString(ch_pt);
}
void GameUI::setGuagePower(float percent)
{
	float power = percent;
	if (power > 100)
		power = 100;
	m_guagePower->setPercentage(power);
}
void GameUI::setTimer(int time)
{
	char ch_timer[4];
	sprintf(ch_timer, "%d%d", (time / 10) % 10, time % 10);
	m_labTimer->setString(ch_timer);
}
void GameUI::setGuageHp(float percent)
{
	m_guageHp->setPercentage(percent);
}
void GameUI::setGuageMove(float percent)
{
	m_guageMove->setPercentage(percent);
}
void GameUI::setDegree(int degree)
{
	char ch_degree[6];
	sprintf(ch_degree, "%d", degree);
	m_labDegree->setString(ch_degree);
	m_iDestDeg = degree;
}
void GameUI::setAngle(int dir,int angle)
{
	int tmp = angle;
	if (dir == 1)
	{
		m_sprMinDeg->setRotation(-180-m_iMinDeg);
		m_sprMaxDeg->setRotation(-180-m_iMaxdeg);
		m_sprDestDeg->setRotation(-180-m_iDestDeg);	
		tmp *= -1;
	}
	if (dir == -1)
	{
		m_sprMinDeg->setRotation(m_iMinDeg);
		m_sprMaxDeg->setRotation(m_iMaxdeg);
		m_sprDestDeg->setRotation(m_iDestDeg);
	}
	char ch_angle[6];
	sprintf(ch_angle, "%d",tmp);
	m_labAngle->setString(ch_angle);
	m_sprAngle->setRotation(angle);
}
void GameUI::setDegreeLimit(int min, int max)
{
	m_iMinDeg = min;
	m_iMaxdeg = max;
}
void GameUI::setMissileUI(Sprite* missile, Sprite* spMissile)
{
	if (m_sprMissile != NULL)
		m_sprMissile->removeFromParent();
	if (m_sprSpMissile != NULL)
		m_sprSpMissile->removeFromParent();
	m_sprMissile = missile;
	m_sprMissile->setPosition(Vec2(25, 26));
	m_LayerUI->addChild(m_sprMissile);
	m_sprSpMissile = spMissile;
	m_sprSpMissile->setPosition(Vec2(265, 26));
	m_LayerUI->addChild(m_sprSpMissile);
}
void GameUI::setItem(int slot, Item_command* item)
{
	if (slot < 4 && slot >= 0)
	{
		if (m_pItem[slot] != NULL)
			m_pItem[slot]->getSprite()->removeFromParent();
		m_pItem[slot] = item;
		m_pItem[slot]->getSprite()->setPosition(Vec2(430+slot*57, 155));
		m_LayerUI->addChild(m_pItem[slot]->getSprite());
	}
}
void GameUI::displayGameOver()
{
	auto gameover= Label::createWithSystemFont("GAME OVER", "Amatic SC", 100);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	gameover->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	m_LayerUI->addChild(gameover);
}
GameUI::~GameUI()
{
}
