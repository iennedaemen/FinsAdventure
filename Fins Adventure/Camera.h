#pragma once
#include "structs.h"

class Camera
{
public:
	Camera(float width, float height);
	
	void SetBoundaries(const Rectf& boundaries);
	void Draw(const Rectf& toTrack) const;
	Point2f GetPosition(const Rectf& toTrack) const;

private:
	float m_Width;
	float m_Height;
	Rectf m_Boundaries;

	Point2f Track(const Rectf& toTrack) const;
	void Clamp(Point2f& bottomLeftPos) const;
};

