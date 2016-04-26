#include "stdafx.h"
#include "Bubble.h"

Bubble::Bubble()
	: _isExplode(false)
	, _scale(0.25f)
{
	_tex = Core::resourceManager.Get<Render::Texture>("Bubble");
	_field = IRect(0, 0, Render::device.Width(), Render::device.Height());
	_pos.x = math::random(0, _field.width);
	_pos.y = math::random(0, _field.height);
	_tailEffect = _container.AddEffect("Tail");
	_tailEffect->SetPos(_pos);
	_tailEffect->Reset();
	_speed = 250.f + math::random(0.f, 50.f);
	_angle = math::random(0.f, 2 * math::PI);
	_color = Color(math::random(0, 255), math::random(0, 255), math::random(0, 255), 150);
}


Bubble::~Bubble() {}

void Bubble::Update(float dt) {
	if (!_isExplode) {
		_pos.x += _speed * dt * math::cos(_angle);
		_pos.y += _speed * dt * math::sin(_angle);
		CheckBounce();
		_tailEffect->SetPos(_pos);
	}
	_container.Update(dt);
}

void Bubble::Draw() {
	Render::BeginColor(_color);
	_container.Draw();
	if (!_isExplode) {
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(_pos);
		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(-_tex->Width() / 2.f, -_tex->Height() / 2.f, 0.f);
		_tex->Draw();
		Render::device.PopMatrix();
	} else {
		Render::BindFont("BauhausCTT-Regular");
		Render::PrintString(_pos.x, _pos.y, std::string("+") + utils::lexical_cast(_score), 1.0f, LeftAlign, BottomAlign);
	}
	Render::EndColor();
}


void Bubble::CheckBounce() {
	float r = GetRadius();
	if (_pos.x - r  < _field.x) {
		_pos.x = _field.x + r;
		_angle = math::PI - _angle;
	} else if (_pos.x + r > _field.width) {
		_pos.x = _field.width - r;
		_angle = math::PI - _angle;
	}
	if (_angle < 0) {
		_angle += 2 * math::PI;
	}
	if (_pos.y - r < _field.y) {
		_pos.y = _field.y + r;
		_angle = 2 * math::PI - _angle;
	} else if (_pos.y + r > _field.height) {
		_pos.y = _field.height - r;
		_angle = 2 * math::PI - _angle;
	}
}

const FPoint& Bubble::GetPos() const {
	return _pos;
}

const float Bubble::GetRadius() const {
	float radius = math::max(_tex->Width(), _tex->Height()) / 2.f;
	return _scale * radius;
}

const Color& Bubble::GetColor() const {
	return _color;
}

const bool Bubble::IsAlive() const {
	return !_isExplode || !_container.IsFinished();
}

const bool Bubble::IsExplode() const {
		return _isExplode;
}

void Bubble::Explode(int score) {
	_score = score;
	_isExplode = true;
	_tailEffect->Finish();
	ParticleEffectPtr eff = _container.AddEffect("MiniBubble");
	eff->SetPos(_pos);
	eff->Reset(); 
}
