#pragma once
#include "structs.h"
#include "Texture.h"

class Water
{
public:
	Water(float speed, Point2f WidthHeight);
	void Draw() const;
	void Update(float elapsedSec);
	bool IsInWater(const Rectf& actorShape) const;
	bool Drowning(const Rectf& actorShape) const;

	void Reset();

private:;
	Rectf m_Shape;
	float m_BeginHeight;
	float m_RisingSpeed;


};

