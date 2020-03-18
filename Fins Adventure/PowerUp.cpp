#include "stdafx.h"
#include "PowerUp.h"
#include "utils.h"
#include "Matrix2x3.h"

const float PowerUp::radius{ 8 };

PowerUp::PowerUp(const Point2f& center, Type type)
	: m_Type{type}
	, m_Shape{Circlef(center, radius)}
	, m_Texture{"Resources/PowerUps.png"}
	, m_TextClip{ (m_Texture.GetWidth() / 4) * (float(m_Type) - 1) , 0.0f , m_Texture.GetWidth()/4, m_Texture.GetHeight()}
{

}

void PowerUp::Draw() const
{
	m_Texture.Draw(Rectf(m_Shape.center.x, m_Shape.center.y, m_Shape.radius * 2, m_Shape.radius * 2), m_TextClip);
}	

bool PowerUp::IsOverlapping(const Rectf& rect) const
{
	if (utils::IsOverlapping(rect, m_Shape))
	{
		return true;
	}
	else return false;
}

Type PowerUp::GetPowerUpType() const
{
	return m_Type;
}