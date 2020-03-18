#include "stdafx.h"
#include "Finnish.h"
#include "utils.h"


Finnish::Finnish(Point2f position)
	: m_Texture{"Resources/Flag.png"}
	, m_IsFinnished{false}

	, m_SpriteSheetLeft{0.0f}
	, m_SpriteSheetTop{1000.0f}
	, m_SpriteSheetWidth{108.0f}
	, m_SpriteSheetHeight{1000.0f}
	, m_Cols{10}
	, m_Rows{1}
	, m_FramesPerSec{15}
	, m_FrameTime{}
	, m_Scale{0.6f}

	, m_AccuSec{}
	, m_CurFrame{}
{
	m_FrameTime = 1.0f / m_FramesPerSec;

	m_Shape = { position.x, position.y, m_Texture.GetWidth(), m_Texture.GetHeight()/m_Cols };
	m_SrcRect = { 0.0f, 0.0f, m_Texture.GetWidth(), m_Texture.GetHeight() / m_Cols };
}

void Finnish::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0);
	glScalef(m_Scale, m_Scale, 1);
	glTranslatef(-m_Shape.left - m_Shape.width / 2, -m_Shape.bottom, 0);
	m_Texture.Draw(m_Shape, m_SrcRect);
	glPopMatrix();
}

void Finnish::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec > m_FrameTime)
	{
		++m_CurFrame;
		m_AccuSec -= m_FrameTime;
		UpdateSourceRect();
	}
}

void Finnish::UpdateSourceRect()
{
	m_SrcRect.bottom = m_SpriteSheetTop + m_SrcRect.height * m_CurFrame;
}

bool Finnish::IsFinnished(const Rectf& actorshape)
{
	Rectf shape{ m_Shape.left + m_Shape.width / 3, m_Shape.bottom, m_Shape.width / 3 * 2, m_Shape.height / 3 * 2 };
	if (utils::IsOverlapping(shape, actorshape)) return true;
	return false;
}