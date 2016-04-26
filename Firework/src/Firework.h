#pragma once
class Firework
{
public:
	Firework(const FPoint& pos, float angle, int curLevel, int maxLevel, int numChildrens);
	~Firework();

	void Update(float dt);
	void Draw();

	void Explode();
	void CheckCollision(Firework& other);
	const bool IsFinish() const;
	const bool IsExplode() const;
	const FPoint& GetPos() const;

private:
	static const float TiME_ALIVE;
	static const float RADIUS;

	float _timer;
	float _angle;
	float _speed;
	int _curLevel;
	int _maxLevel;
	int _numChildrens;
	float _lifeVariation;
	bool _isExplode;
	FPoint _pos;
	ParticleEffectPtr _tailEffect;
	EffectsContainer _container;
	std::vector<std::unique_ptr<Firework>> _childrens;

	static const float sqr(float f);	
};

