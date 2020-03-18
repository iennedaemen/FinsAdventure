#pragma once
#include "Texture.h"
#include "structs.h"

class Finnish
{
public:
	Finnish(Point2f position);
	void Draw() const;
	void Update(float elapsedSec);
	bool IsFinnished(const Rectf& actorshape);

private:
	Texture m_Texture;
	Rectf m_Shape;
	bool m_IsFinnished;

	const float m_SpriteSheetLeft;
	const float m_SpriteSheetTop;
	const float m_SpriteSheetWidth;
	const float m_SpriteSheetHeight;
	const int m_Cols;
	const int m_Rows;
	int m_FramesPerSec;
	float m_FrameTime;
	float m_Scale;

	float m_AccuSec;
	int m_CurFrame;
	Rectf m_SrcRect;

	// helper functions
	void UpdateSourceRect();
};

