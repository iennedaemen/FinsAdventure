#include "stdafx.h"
#include "Level.h"
#include <algorithm>


Level::Level()
	: m_BackgroundTexture("Resources/level.png")
	, m_Boundaries{ 0.0f, 0.0f, m_BackgroundTexture.GetWidth(), m_BackgroundTexture.GetHeight()}
	, m_LevelMusic{"Resources/Sound/Level.mp3"}
{
	SVGParser::GetVerticesFromSvgFile("Resources/level.svg", m_Vertices);
	CreatePlatforms();
	CreateColorPlatforms();
	CreateSpikes();
}


void Level::DrawBackground() const
{
	m_BackgroundTexture.Draw();

	for (size_t i = 0; i < m_pPlatforms.size(); i++)
	{
		m_pPlatforms[i]->Draw();
	}

	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		m_pColorPlatforms[i]->Draw();
	}

	for (size_t i = 0; i < m_Spikes.size(); i++)
	{
		m_Spikes[i].Draw();
	}
}

void Level::Update(float elapsedSec, const Type& powerUp)
{
	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		m_pColorPlatforms[i]->Update(elapsedSec, powerUp);
	}
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f actorShapeTop{ Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height) };
	Point2f actorShapeBottom{ Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom) };
	utils::HitInfo hitInfo{};

	for (std::vector<Point2f> v : m_Vertices)
	{
		if (utils::Raycast(v, actorShapeTop, actorShapeBottom, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0.0f;
		}
	}

	for (size_t i = 0; i < m_pPlatforms.size(); i++)
	{
		m_pPlatforms[i]->HandleCollision(actorShape, actorVelocity);
	}

	
	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		m_pColorPlatforms[i]->HandleCollision(actorShape, actorVelocity);
	}

	for (size_t i = 0; i < m_Spikes.size(); i++)
	{
		m_Spikes[i].HandleCollision(actorShape, actorVelocity);
	}
}

bool Level::IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f actorShapeTop{ Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height) };
	Point2f actorShapeBottom{ Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom) };
	utils::HitInfo hitInfo{};


	for (size_t i = 0; i < m_pPlatforms.size(); i++)
	{
		if (m_pPlatforms[i]->IsOnGround(actorShape, actorVelocity)) return true;
	}

	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		if(m_pColorPlatforms[i]->IsOnGround(actorShape, actorVelocity)) return true;
	}

	for (std::vector<Point2f> v : m_Vertices)
	{
		return (utils::Raycast(v, actorShapeTop, actorShapeBottom, hitInfo));
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

Vector2f Level::GetColorPlatformVelocity(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		if (m_pColorPlatforms[i]->IsOnGround(actorShape, actorVelocity)) return m_pColorPlatforms[i]->GetVelocity();
	}
	return { 0.0f, 0.0f };
}

bool Level::IsOnColorPlatform(const Rectf& actorShape, Vector2f& actorVelocity) const
{
	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		if (m_pColorPlatforms[i]->IsOnGround(actorShape, actorVelocity)) return true;
	}
	return false;
}

bool Level::IsOnSpikes(const Rectf& actorShape, const Vector2f actorVelocity) const
{
	for (size_t i = 0; i < m_Spikes.size(); i++)
	{
		if(m_Spikes[i].IsOnSpikes(actorShape, actorVelocity)) return true;
	}
	return false;
}

void Level::CreatePlatforms()
{
	m_pPlatforms.push_back(std::make_unique<Platform>(Point2f(550, 400), Platform::PlatformSize::big));
	m_pPlatforms.push_back(std::make_unique<Platform>(Point2f(100, 400), Platform::PlatformSize::small));
	m_pPlatforms.push_back(std::make_unique<Platform>(Point2f(200, 600), Platform::PlatformSize::small));
	m_pPlatforms.push_back(std::make_unique<Platform>(Point2f(100, 800), Platform::PlatformSize::small));
	m_pPlatforms.push_back(std::make_unique<Platform>(Point2f(750, 800), Platform::PlatformSize::big));

}

void Level::CreateColorPlatforms()
{
	m_pColorPlatforms.push_back(std::make_unique<ColorPlatform>(ColorPlatform{ Type::red, ColorPlatform::PlatformType::vertical, { 1600, 150 }, { 1600, 500 } }));
	m_pColorPlatforms.push_back(std::make_unique<ColorPlatform>(ColorPlatform{ Type::blue, ColorPlatform::PlatformType::horizontal,{ 250, 800 },{ 450, 800 } }));
	m_pColorPlatforms.push_back(std::make_unique<ColorPlatform>(ColorPlatform{ Type::yellow, {800, 1200} }));
	m_pColorPlatforms.push_back(std::make_unique<ColorPlatform>(ColorPlatform{ Type::red,{ 1100, 1200 } }));
	m_pColorPlatforms.push_back(std::make_unique<ColorPlatform>(ColorPlatform{ Type::yellow,{ 1300, 1400 } }));

}

void Level::CreateSpikes()
{
	m_Spikes.push_back(Point2f{ 130, 70 });
}

void Level::Reset()
{
	for (size_t i = 0; i < m_pColorPlatforms.size(); i++)
	{
		m_pColorPlatforms[i]->Reset();
	}
}