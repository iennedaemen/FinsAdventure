#include "stdafx.h"
#include "PowerUpManager.h"
#include "utils.h"

PowerUpManager::PowerUpManager()
	: m_Type{Type::none}
	, m_pHit{ }
	, m_pItems{}
	, m_PickUpSound{"Resources/Sound/PickUp.mp3"}
{

}

std::shared_ptr<PowerUp> PowerUpManager::AddItem(const Point2f &center, const Type& type)
{
	m_pItems.push_back( std::make_unique<PowerUp>(center, type));
	return m_pItems.back();
}



void PowerUpManager::Draw() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		if(m_pItems[i] != m_pHit) m_pItems[i]->Draw();
	}
}

size_t PowerUpManager::Size() const
{
	return m_pItems.size();
}

bool PowerUpManager::HitItem(const Rectf& rect)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		if (m_pItems[i]->IsOverlapping(rect) && m_pHit != m_pItems[i])
		{
			m_PickUpSound.Play(0);
			m_pHit = m_pItems[i];
			m_Type = m_pItems[i]->GetPowerUpType();
			return true;
		}
	}
	return false;
}

Type PowerUpManager::GetType() const
{
	return m_Type;
}

void PowerUpManager::Reset()
{
	m_Type = Type::none;
	m_pHit = nullptr;

}

void PowerUpManager::SetVolumeDown(const int& decrease)
{
	utils::SetVolumeDown(m_PickUpSound, decrease);
}

void PowerUpManager::SetVolumeUp(const int& increase)
{
	utils::SetVolumeUp(m_PickUpSound, increase);
}