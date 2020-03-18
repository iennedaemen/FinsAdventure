#pragma once
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
#include "PowerUp.h"
#include "Platform.h"

class ColorPlatform
{
public:
	enum PlatformType
	{
		horizontal,
		vertical,
		disapairing
	};

	ColorPlatform(Type type, PlatformType move, Point2f boundaryMax, Point2f boundaryMin);
	ColorPlatform(Type type, Point2f pos);

	void Draw() const;
	void Update(float elapsedSec, const Type& powerUp);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, const Vector2f actorVelocity) const;
	Vector2f GetVelocity() const;

	void Reset();

private:
	Rectf m_Shape;
	Rectf m_SrcRect;

	Texture m_TexPlatforms;
	Texture m_TexDisapaired;

	PlatformType m_PlatformType;
	Type m_Type;

	float m_MoveSpeed;
	Vector2f m_Velocity;
	Vector2f m_TempVelocity;

	Point2f m_BoundaryMax;
	Point2f m_BoundaryMin;
	
	bool m_IsActive;

};

