#pragma once
class BigBubble
{
public:
	BigBubble(const FPoint& pos, float bornTime, float lifeTime, const Color& color, int multiplier);
	~BigBubble();

	void Update(float dt);
	void Draw();

	const bool IsAlive() const;
	const bool IsExplode() const;
	const FPoint& GetPos() const;
	const float GetRadius() const;
	const int GetMultiplier() const;
private:
	float _timer;
	float _bornTime;
	float _lifeTime;
	float _dieTime;
	float _scale;
	int _multiplier;
	bool _isExplode;
	FPoint _pos;
	Color _color;
	Render::Texture* _tex;
	EffectsContainer _container;

	void Explode();
};

