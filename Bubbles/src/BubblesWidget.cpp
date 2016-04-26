#include "stdafx.h"
#include "BubblesWidget.h"

BubblesWidget::BubblesWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _countPoints(20)
	, _bornTime(0.5f)
	, _lifeTime(1.f)
	, _score(0)
	, _isGameStart(false)
	, _isGameFinish(false)
{
	Init();
	StartGame();
}

BubblesWidget::~BubblesWidget() {}

void BubblesWidget::Init() {
	std::string line, name, value;
	std::ifstream file("input.txt", std::ios::in);
	if (file.is_open()) {
		while (!file.eof())	{
			std::getline(file, line);
			if (utils::ReadNvp(line, name, value)) {
				if (name == "CountPoints") {
					_countPoints = utils::lexical_cast<int>(value);
					// накладываем ограничение на пользоватедьский ввод
					if (_countPoints < 0) {
						_countPoints = 0;
					} else if (_countPoints > 100) {
						_countPoints = 100;
					}
				} else if (name == "BornTime") {
					_bornTime = utils::lexical_cast<float>(value);
					if (_bornTime < 0) {
						_bornTime = 0;
					} else if (_bornTime > 100) {
						_bornTime = 100;
					}
				} else if (name == "LifeTime") {
					_lifeTime = utils::lexical_cast<float>(value);
					if (_lifeTime < 0) {
						_lifeTime = 0;
					} else if (_lifeTime > 100) {
						_lifeTime = 100;
					}
				}
			}
		}
		file.close();
	}
	_backgroundTex = Core::resourceManager.Get<Render::Texture>("Background");
}

void BubblesWidget::CreateBubbles() {
	for (int i = 0; i < _countPoints; ++i) {
		_bubbles.push_back(std::unique_ptr<Bubble>(new Bubble()));
	}
}

void BubblesWidget::Update(float dt) {
	if (_isGameStart && _bigBubbles.size() == 0) {
		_isGameFinish = true;
		return;
	}
	auto bigBubble = _bigBubbles.begin();
	while (bigBubble != _bigBubbles.end()) {
		(*bigBubble)->Update(dt);
		if (!(*bigBubble)->IsAlive()) {
			bigBubble = _bigBubbles.erase(bigBubble);
		} else {
			++bigBubble;
		}
	}
	auto bubble = _bubbles.begin();
	while (bubble != _bubbles.end()) {
		(*bubble)->Update(dt);
		if (!(*bubble)->IsAlive()) {
			bubble = _bubbles.erase(bubble);
		} else {
			++bubble;
		}
	}
	CheckCollision();
}	

void BubblesWidget::Draw() {
	_backgroundTex->Draw();
	if (!_isGameFinish) {
		auto bigBubble = _bigBubbles.begin();
		while (bigBubble != _bigBubbles.end()) {
			(*bigBubble)->Draw();
			++bigBubble;
		}
		auto bubble = _bubbles.begin();
		while (bubble != _bubbles.end()) {
			(*bubble)->Draw();
			++bubble;
		}
	}
	DrawScore();
	if (_isGameFinish) {
		Render::device.SetTexturing(false);
		Render::BeginColor(Color(0, 0, 0, 100));
		Render::DrawRect(0, 0, Render::device.Width(), Render::device.Height());
		Render::EndColor();
		Render::device.SetTexturing(true);
		Render::BindFont("BauhausCTT-Regular");
		int dy = Render::getFontHeight();
		int x = Render::device.Width() / 2;
		int y = Render::device.Height() / 2;
		Render::PrintString(x, y, std::string("Total Score: ") + utils::lexical_cast(_score), 1.0f, CenterAlign, BottomAlign);
		Render::PrintString(x, y - dy, std::string("Press right mouse button for restart "), 0.7f, CenterAlign, BottomAlign);

	}
}

void BubblesWidget::DrawScore() {
	Render::BindFont("BauhausCTT-Regular");
	int dy = Render::getFontHeight();
	int x = 20;
	int y = Render::device.Height() - 20;
	std::string points = utils::lexical_cast(_countPoints - _bubbles.size()) + std::string("/") + utils::lexical_cast(_countPoints);
	Render::PrintString(x, y -= dy, std::string("Bubbles: ") + points, 1.0f, LeftAlign, BottomAlign);
	Render::PrintString(x, y -= dy, std::string("Score: ") + utils::lexical_cast(_score), 1.0f, LeftAlign, BottomAlign);
}

bool BubblesWidget::MouseDown(const IPoint& mouse_pos) {
	if (Core::mainInput.GetMouseLeftButton()) {
		if (!_isGameStart) {
			_isGameStart = true;
			_bigBubbles.push_back(std::unique_ptr<BigBubble>(new BigBubble(mouse_pos, _bornTime, _lifeTime, Color(255, 255, 255, 150), 1)));
		}
	} else if (Core::mainInput.GetMouseRightButton()) {
		if (_isGameFinish) {
			StartGame();
		}
	}
	return false;
}

void BubblesWidget::StartGame() {
	_isGameStart = false;
	_isGameFinish = false;
	_score = 0;
	_bubbles.clear();
	_bigBubbles.clear();
	CreateBubbles();
}

void BubblesWidget::CheckCollision() {
	int sizeBigBubbles = _bigBubbles.size();
	for (int i = 0; i < sizeBigBubbles; ++i) {
		// такой цикл, потому что в вектор могут добавиться новые элементы, произойдет перераспределение памяти и итераторы станут некорректными
		int sizeBubbles = _bubbles.size();
		for (int j = 0; j < sizeBubbles; ++j) {
			// для единообразия аналогичный цикл по элементам
			if (!_bubbles[j]->IsExplode() && !_bigBubbles[i]->IsExplode()) {
				if (IsIntersect(_bubbles[j]->GetPos(), _bubbles[j]->GetRadius(), _bigBubbles[i]->GetPos(), _bigBubbles[i]->GetRadius())) {
					int score = _bigBubbles[i]->GetMultiplier() * BubblesWidget::BASE_SCORE;
					_score += score;
					_bubbles[j]->Explode(score);
					_bigBubbles.push_back(std::unique_ptr<BigBubble>(
						new BigBubble(_bubbles[j]->GetPos(), _bornTime, _lifeTime, _bubbles[j]->GetColor(), _bigBubbles[i]->GetMultiplier() * 3)));
				}
			}
		}
	}
}

const bool BubblesWidget::IsIntersect(const FPoint& p1, float r1, const FPoint& p2, float r2) {
	// решив систему квадратных уравнений вида
	// (x-x1)^2 + (y-y1)^2=r1^2 
	// (x-x2)^2 + (y-y2)^2=r2^2 
	// получаем следующие формулы для нахождения точки пересечения двух окружностей
	// x=p*y + q + x1
	// (p^2+1)*y^2 + 2*y*(p*q - y1) = r1^2 - y1^2 - q^2;
	// где
	// p=(y1-y2)/(x2-x1)
	// q=(r1^2 - r2^2 + x2^2 - x1^2 + y2^2 - y1^2) / 2*(x2 - x1) - x1
	// 
	// a=p^2+1
	// b=2*(p*q - y1)
	// c=y1^2 + q^2 - r1^2
	// a*y^2 + b*y + c
	// если дискриминант для квадратного уравнения больше или равен нулю,
	// то существует пересечение или касание окружностей
	// d=b^2 - 4*a*c >=0
	//
	// отдельно проверяем случай, когда x1 = x, тогда в знаменателе получается ноль
	if (math::abs(p1.x - p2.x) < 0.5f) {
		if (math::abs(p1.y - p2.y) > r1 + r2) {
			return false;
		} else {
			return true;
		}
	}
	float p = (p1.y - p2.y) / (p2.x - p1.x);
	float q = (Sqr(r1) - Sqr(r2) + Sqr(p2.x) - Sqr(p1.x) + Sqr(p2.y) - Sqr(p1.y)) / (2 * (p2.x - p1.x)) - p1.x;

	float a = (Sqr(p) + 1);
	float b = 2 * (p * q - p1.y);
	float c = Sqr(p1.y) + Sqr(q) - Sqr(r1);
	float d = Sqr(b) - 4 * a * c;
	return d > 0;
}
