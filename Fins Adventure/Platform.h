#pragma once
#include "structs.h"
#include "Vector2f.h"
#include "Texture.h"

class Platform
{
public:
	enum class PlatformSize
	{
		big,
		small
	};

	Platform(const Point2f& bottomLeft, PlatformSize size);
	
	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, const Vector2f actorVelocity) const;

private:
	Rectf m_Shape;
	Texture m_TextureBig;
	Texture m_TextureSmall;
	PlatformSize m_Size;
};

