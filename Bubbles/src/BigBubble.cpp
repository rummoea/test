#include "stdafx.h"
#include "BigBubble.h"


BigBubble::BigBubble(const FPoint& pos, float bornTime, float lifeTime, const Color& color, int multiplier)
	: _pos(pos)
	, _bornTime(bornTime)
	, _lifeTime(lifeTime)
	, _color(color)
	, _multiplier(multiplier)
	, _dieTime(bornTime / 2.f)
	, _scale(0.f)
	, _isExplode(false)
	, _timer(0.f)
{
	_tex = Core::resourceManager.Get<Render::Texture>("Bubble");
}


BigBubble::~BigBubble() {}


void BigBubble::Update(float dt) {
	_timer += dt;
	if (!_isExplode) {
		if (_timer < _bornTime) {
			_scale += dt / _bornTime;
		} else if (_timer > _bornTime + _lifeTime + _dieTime) {
			Explode();
		} else if (_timer > _bornTime + _lifeTime) {
			_scale -= 2 * dt / _bornTime;
		}
	}
	_container.Update(dt);
}

void BigBubble::Draw() {
	Render::BeginColor(_color);
	_container.Draw();
	if (!_isExplode) {
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(_pos);
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-_tex->Width() / 2.f, -_tex->Height() / 2.f, 0.f);
		_tex->Draw();
		Render::device.PopMatrix();
	}
	Render::EndColor();
}

void BigBubble::Explode() {
	_isExplode = true;
	ParticleEffectPtr effect = _container.AddEffect("Explosion");
	effect->SetPos(_pos);
	effect->Reset();
}

const bool BigBubble::IsAlive() const {
	return !_isExplode || !_container.IsFinished();
}

const bool BigBubble::IsExplode() const {
	return _isExplode;
}

const FPoint& BigBubble::GetPos() const {
	return _pos;
}

const float BigBubble::GetRadius() const {
	float radius = math::max(_tex->Width(), _tex->Height()) / 2.f;
	return _scale * radius;
}

const int BigBubble::GetMultiplier() const {
	return _multiplier;
}
