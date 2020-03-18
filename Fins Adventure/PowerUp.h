#pragma once
#include "Texture.h"
#include "structs.h"
#include "Matrix2x3.h"

class PowerUp 
{
public:

	PowerUp(const Point2f& center, Type type );

	void Draw( ) const;
	bool IsOverlapping(const Rectf& rect ) const;
	Type GetPowerUpType() const;
	static const float radius;

private:
	const Type m_Type;
	const Circlef m_Shape;
	const Texture m_Texture;
	const Rectf m_TextClip;
};


