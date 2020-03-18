#pragma once
#include "PowerUp.h"
#include <vector>
#include <memory>
#include "SoundEffect.h"

class PowerUpManager
{
public:
	PowerUpManager( );

	std::shared_ptr<PowerUp> AddItem( const Point2f &center, const Type& type );
	void Draw( ) const;

	size_t Size( ) const;
	bool HitItem( const Rectf& rect ) ;
	Type GetType() const;

	void Reset();

	void SetVolumeUp(const int& increase);
	void SetVolumeDown(const int& decrease);

private:
	std::vector<std::shared_ptr<PowerUp>> m_pItems;
	Type m_Type;
	std::shared_ptr<PowerUp> m_pHit;
	SoundEffect m_PickUpSound;
};
