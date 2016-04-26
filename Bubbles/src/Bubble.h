#pragma once
class Bubble
{
public:
	Bubble();
	~Bubble();

	void Update(float dt);
	void Draw();
	const FPoint& GetPos() const;
	const float GetRadius() const;
	const Color& GetColor() const;
	void Explode(int score);
	const bool IsAlive() const;
	const bool IsExplode() const;
	
private:
	float _angle;
	float _speed;
	bool _isExplode;	
	int _score;
	float _scale;
	FPoint _pos;
	IRect _field;
	Color _color;
	Render::Texture* _tex;
	ParticleEffectPtr _tailEffect;
	EffectsContainer _container;

	void CheckBounce();	
};

