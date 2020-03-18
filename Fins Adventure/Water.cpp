#include "stdafx.h"
#include "Water.h"
#include "utils.h"


Water::Water(float speed, Point2f widthHeight)
	: m_Shape{0.0f, 0.0f, widthHeight.x, widthHeight.y}
	, m_RisingSpeed{ speed }
	, m_BeginHeight{widthHeight.y}
{
}

void Water::Draw() const
{
	glColor4f(0.0f, 0.4f, 1.0f, 0.4f);
	utils::FillRect( m_Shape );

}

void Water::Update(float elapsedSec)
{
	m_Shape.height += elapsedSec * m_RisingSpeed;
}

bool Water::IsInWater(const Rectf& actorShape) const
{
	if (utils::IsOverlapping(m_Shape, actorShape))
	{
		return true;
	}
	return false;
}

bool Water::Drowning(const Rectf& actorShape) const
{
	Rectf avatar{ actorShape.left, actorShape.bottom + actorShape.height / 4 * 3, actorShape.width, actorShape.height / 4 };
	if (utils::IsOverlapping(m_Shape, avatar))
	{
		return true;
	}
	return false;
}

void Water::Reset()
{
	m_Shape = { m_Shape.left, m_Shape.bottom, m_Shape.width, m_BeginHeight };
}