#include "stdafx.h"
#include "Camera.h"
#include "utils.h"


Camera::Camera(float width, float height)
	: m_Width{width}
	, m_Height{height}
	, m_Boundaries{0.0f, 0.0f, width, height}
{

}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
}

void Camera::Draw(const Rectf& toTrack) const
{
	Point2f position = Track(toTrack);
	Clamp(position);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	utils::DrawRect(position, m_Width, m_Height, 2.0f);
}

Point2f Camera::Track(const Rectf& toTrack) const
{
	return Point2f{ toTrack.left + toTrack.width / 2 - m_Width / 2, toTrack.bottom + toTrack.height / 2 - m_Height / 2 };
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < m_Boundaries.left)
	{
		bottomLeftPos.x = m_Boundaries.left;
	}
	if (bottomLeftPos.x + m_Width > m_Boundaries.left + m_Boundaries.width)
	{
		bottomLeftPos.x = m_Boundaries.left + m_Boundaries.width - m_Width;
	}
	if (bottomLeftPos.y < m_Boundaries.bottom)
	{
		bottomLeftPos.y = m_Boundaries.bottom;
	}
	if (bottomLeftPos.y + m_Height > m_Boundaries.bottom + m_Boundaries.height)
	{
		bottomLeftPos.y = m_Boundaries.bottom + m_Boundaries.height - m_Height;
	}
}

Point2f Camera::GetPosition(const Rectf& toTrack) const
{
	Point2f position = Track(toTrack);
	Clamp(position);
	return position;
}
