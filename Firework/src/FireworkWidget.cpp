#include "stdafx.h"
#include "FireworkWidget.h"

FireworkWidget::FireworkWidget(const std::string& name, rapidxml::xml_node<>* elem) 
	: Widget(name)
	, _maxLevel(3)
	, _childrensCount(5)
{
	Init();
}

void FireworkWidget::Init() {
	std::string line, name, value;
	std::ifstream file("input.txt", std::ios::in);
	if (file.is_open()) {
		while (!file.eof())	{
			std::getline(file, line);
			if (utils::ReadNvp(line, name, value)) {
				if (name == "Level") {
					_maxLevel = utils::lexical_cast<int>(value);
					// ограничим пользовательский ввод
					if (_maxLevel < 1) {
						_maxLevel = 1;
					} else if (_maxLevel > 10) {
						_maxLevel = 10;
					}
				}
				else if (name == "Count") {
					_childrensCount = utils::lexical_cast<int>(value);
					if (_childrensCount < 0) {
						_childrensCount = 0;
					} else if (_childrensCount > 50) {
						_childrensCount = 50;
					}
				}
			}
		}
		file.close();
	}
}

void FireworkWidget::Update(float dt) {
	auto firework = _fireworks.begin();
	while (firework != _fireworks.end()) {
		(*firework)->Update(dt);
		if ((*firework)->IsFinish()) {
			firework = _fireworks.erase(firework);
			continue;
		}
		++firework;
	}
	firework = _fireworks.begin();
	while (firework != _fireworks.end()) {
		auto otherFirework = firework + 1;
		while (otherFirework != _fireworks.end()) {
			(*firework)->CheckCollision(*(*otherFirework));
			++otherFirework;
		}
		++firework;
	}
}	

void FireworkWidget::Draw() {
	auto firework = _fireworks.begin();
	while (firework != _fireworks.end()) {
		(*firework)->Draw();
		++firework;
	}
}

bool FireworkWidget::MouseDown(const IPoint& mouse_pos) {
	if (Core::mainInput.GetMouseLeftButton()) {
		_fireworks.push_back(std::unique_ptr<Firework>(new Firework(mouse_pos, math::PI / 2.f, 1, _maxLevel, _childrensCount)));
	}
	return false;
}
