#include "FieldManager.h"
#include "Scene_game.h"
#define DATABIT 32
FieldManager* FieldManager::pInstance = NULL;
FieldManager::FieldManager()
{
}
FieldManager* FieldManager::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new FieldManager();
	}
	return pInstance;
}
void FieldManager::init(Scene_game* pScene)
{
	m_pScene = pScene;
	loadData();
}
bool FieldManager::checkBin(Vec2 pt)
{
	DWORD bin = ptToBin(pt);
	if (bin / DATABIT > m_vecMapData.size())
		return false;
	DWORD flag = 1;
	flag = flag << bin%DATABIT;
	DWORD result = m_vecMapData[bin / DATABIT] & flag;
	if (result > 0)
		return true;
	else
		return false;
}
DWORD FieldManager::ptToBin(Vec2 pt)
{
	DWORD result =(DWORD)pt.x + (DWORD)pt.y*m_sizeMap.width;
	return result;
}

void  FieldManager::removeMap(Vec2 pt)
{
	DWORD bin = ptToBin(pt);
	if (bin / DATABIT > m_vecMapData.size())
		return;
	DWORD flag = 1;
	flag = ~(flag << bin%DATABIT);
	m_vecMapData[bin / DATABIT] = m_vecMapData[bin / DATABIT] & flag;
}
void FieldManager::explosion(Vec2 pt, int r, int damage)
{
	DrawNode* stencil = DrawNode::create();
	stencil->drawSolidCircle(pt, r, 60.0f, 100.0f, Color4F(0, 0, 0, 1));
	m_ndStencil->addChild(stencil);
	m_pScene->checkDamage(pt, r,damage);
	for (int i = pt.y - r; i < pt.y + r; i++)
	{
		for (int j = pt.x - r; j < pt.x + r; j++)
		{
			float result = sqrt(pow(pt.x - j, 2) + pow(pt.y - i, 2));
			if (result < r)
				removeMap(Vec2(j, i));
		}
	}
}
bool FieldManager::hitCheck(Rect rc)
{
	if (m_sprField->getBoundingBox().intersectsRect(rc))
	{
		for (int i = rc.getMinY(); i < rc.getMaxY(); i++)
		{
			for (int j = rc.getMinX(); j < rc.getMaxX(); j++)
			{
				if (checkBin(Vec2(j, i)))
					return true;
			}
		}
	}
	return false;
}
void FieldManager::loadData()
{
	m_cpNode = ClippingNode::create();
	m_ndStencil = Sprite::create();
	m_cpNode->setStencil(m_ndStencil);

	m_sprMap = Sprite::create("map1.png");
	m_sizeMap.height = m_sprMap->getTexture()->getPixelsHigh();
	m_sizeMap.width = m_sprMap->getTexture()->getPixelsWide();
	m_sprMap->setAnchorPoint(Vec2(0, 0));
	m_sprMap->setPosition(0, 0);
	m_pScene->getGameLayer()->addChild(m_sprMap,0);
	
	std::string path = "..\\Resources\\data.bin";
	log("%s", path.c_str());
	FILE* fp = fopen(path.c_str(), "rb");
	MAPDATA header;
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);
		char* buffer = new char[size];
		memset(buffer, 0, size);
		fseek(fp, 0, SEEK_SET);
		fread(buffer, size, 1, fp);
		fclose(fp);
		memcpy(&header, buffer, sizeof(header));
		DWORD bufIndex = sizeof(header);
		m_sizeField.height = header.height;
		m_sizeField.width = header.width;
		for (int i = 0; i < m_sizeMap.height*m_sizeMap.width / DATABIT; i++)
		{
			DWORD pNew = 0;
			m_vecMapData.push_back(pNew);
		}
		DWORD count =0;
		for (DWORD i = bufIndex; i < size; i++)
		{
			if (buffer[i] == 1)
			{
				DWORD flag = 1;
				m_vecMapData[count / DATABIT]= m_vecMapData[count / DATABIT] | (flag<<count%DATABIT);
			}
				count++;
		}
		delete[] buffer;
	}
	m_sprField = Sprite::create("field.png");
	m_sprField->setAnchorPoint(Vec2(0,0));
	m_sprField->setPosition(header.x, header.y);
	m_cpNode->setAnchorPoint(Vec2(0, 0));
	m_cpNode->setPosition(Vec2(0,0));
	m_cpNode->addChild(m_sprField);
	m_cpNode->setInverted(true);
	m_cpNode->setAlphaThreshold(0.0f);
	m_pScene->getGameLayer()->addChild(m_cpNode,2);
}
void FieldManager::release()
{
	m_vecMapData.clear();
}
FieldManager::~FieldManager()
{
}
