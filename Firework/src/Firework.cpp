#include "stdafx.h"
#include "Firework.h"

const float Firework::TiME_ALIVE = 2.5f;
const float Firework::RADIUS = 10.f;

const float Firework::sqr(float f) {
	return f * f;
}

Firework::Firework(const FPoint& pos, float angle, int curLevel, int maxLevel, int numChildrens)
	: _pos(pos)
	, _angle(angle)
	, _curLevel(curLevel)
	, _maxLevel(maxLevel)
	, _numChildrens(numChildrens)
	, _timer(0.f)
	, _speed(250)
	, _lifeVariation(0.f)
	, _isExplode(false)
{
	std::string tailEffectName = "Tail";
	if (_curLevel == 1) {
		tailEffectName = "BigTail";
	}
	_tailEffect = _container.AddEffect(tailEffectName);
	_tailEffect->SetPos(_pos);
	_tailEffect->Reset();
	if (_curLevel != 1) {
		_lifeVariation = math::random(0.f, 0.5f);
		_speed = math::random(_speed / _curLevel, _speed);
	}
}

Firework::~Firework() {}

void Firework::Update(float dt) {
	if (_tailEffect) {
		_timer += dt;
		if (_timer < (TiME_ALIVE + _lifeVariation) / _curLevel ) {
			_pos.x += _speed * dt * math::cos(_angle) / _curLevel;
			_pos.y += _speed * dt * math::sin(_angle) / _curLevel;
			if (_curLevel == 1) {
				_pos.x += math::random(0.f, 0.5f) * math::sin(4 * math::PI * _timer);
				_pos.y -= sqr(_timer) / 2.f;
			}
			_tailEffect->SetPos(_pos);
		} else {
			Explode();
		}
	}
	auto child = _childrens.begin();
	while (child != _childrens.end()) {
		(*child)->Update(dt);
		if ((*child)->IsFinish()) {
			child = _childrens.erase(child);
		} else {
			++child;
		}
	}
	_container.Update(dt);
}

void Firework::Draw() {
	auto child = _childrens.begin();
	while (child != _childrens.end()) {
		(*child)->Draw();
		++child;
	}
	_container.Draw();
}


void Firework::Explode() {
	_isExplode = true;
	_tailEffect->Finish();
	_tailEffect = nullptr;
	std::string effectName = "SmallExplosion";
	if (_curLevel == 1) {
		effectName = "BigExplosion";
	} else if (_curLevel == _maxLevel) {
		effectName = "Particles";
	}
	ParticleEffectPtr explode = _container.AddEffect(effectName);
	explode->SetPos(_pos);
	explode->Reset();
	if (_curLevel < _maxLevel) {
		float offsetAngle = math::random(0.f, 2 * math::PI / _numChildrens);
		for (int i = 0; i < _numChildrens; ++i) {
			float angle = offsetAngle + 2 * math::PI * i / _numChildrens;
			FPoint newPos;
			newPos.x = _pos.x + RADIUS * math::cos(angle);
			newPos.y = _pos.y + RADIUS * math::sin(angle);
			_childrens.push_back(std::unique_ptr<Firework>(new Firework(newPos, angle, _curLevel + 1, _maxLevel, _numChildrens)));
		}
	}
}

const bool Firework::IsFinish() const {
	bool finish = _container.IsFinished();
	bool empty = _childrens.empty();
	return _container.IsFinished() && _childrens.empty();
}

const bool Firework::IsExplode() const {
	return _isExplode;
}

const FPoint& Firework::GetPos() const {
	return _pos;
}

void Firework::CheckCollision(Firework& other) {
	if (!_isExplode) {
		if (!other.IsExplode()) {
			if (sqr(other.GetPos().x - _pos.x) + sqr(other.GetPos().y - _pos.y) < sqr(RADIUS)) {
				Explode();
				other.Explode();
			}
		} else {
			other.CheckCollision(*this);
		}
	} else {
		auto child = _childrens.begin();
		while (child != _childrens.end()) {
			(*child)->CheckCollision(other);
			++child;
		}
	}
}
