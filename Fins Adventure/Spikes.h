#pragma once
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"

class Spikes
{
public:
	Spikes(Point2f pos);

	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnSpikes(const Rectf& actorShape, const Vector2f actorVelocity) const;


private:
	Texture m_Texture;
	Rectf m_Shape;

};

