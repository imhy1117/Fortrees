#include "MissileManager.h"
#include "Missile.h"
#include "FieldManager.h"
MissileManager* MissileManager::pInstance = NULL;

MissileManager* MissileManager::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new MissileManager();
	}
	return pInstance;
}
MissileManager::MissileManager()
{
	m_pCurMissile = NULL;
}

void MissileManager::add(Missile* pMissile)
{
	m_vecMissile.push_back(pMissile);
	m_pCurMissile = pMissile;
}

void MissileManager::release()
{
	for (auto iter = m_vecMissile.begin(); iter != m_vecMissile.end(); iter++)
	{
		delete *iter;
	}
	m_vecMissile.clear();
}
void MissileManager::update(float delta)
{
	if (m_pCurMissile != NULL)
	{
		m_pCurMissile->update(delta);
		if (FieldManager::getInstance()->hitCheck(m_pCurMissile->getRect()))
		{
			m_pCurMissile->explosion();
			m_pCurMissile = NULL;
		}
	}
}

MissileManager::~MissileManager()
{
}
