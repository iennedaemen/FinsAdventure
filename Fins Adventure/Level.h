#pragma once
#include <vector>
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
#include "utils.h"
#include "SVGParser.h"
#include "Platform.h"
#include "ColorPlatform.h"
#include "Spikes.h"
#include <memory>
#include "SoundStream.h"

class Level
{
public:
	Level();

	void Update(float elapsedSec, const Type& powerUp);
	void DrawBackground() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnColorPlatform(const Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnSpikes(const Rectf& actorShape, const Vector2f actorVelocity) const;
	Rectf GetBoundaries() const;
	Vector2f GetColorPlatformVelocity(const Rectf& actorShape, const Vector2f& actorVelocity) const;

	void Reset();

private:
	std::vector<std::vector<Point2f>> m_Vertices;
	Texture m_BackgroundTexture;
	Rectf m_Boundaries;
	std::vector<std::unique_ptr<Platform>> m_pPlatforms;
	std::vector<std::unique_ptr<ColorPlatform>> m_pColorPlatforms;
	std::vector<Spikes> m_Spikes;

	SoundStream m_LevelMusic;

	// helper functions
	void CreatePlatforms();
	void CreateColorPlatforms();
	void CreateSpikes();
};

