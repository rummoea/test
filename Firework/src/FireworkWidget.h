#pragma once

#include "Firework.h"

class FireworkWidget : public GUI::Widget
{
public:
	FireworkWidget(const std::string& name, rapidxml::xml_node<>* elem);

	virtual void Draw() override;
	virtual void Update(float dt) override;
	virtual bool MouseDown(const IPoint& mouse_pos) override;

private:
	int _maxLevel;
	int _childrensCount;
	std::vector<std::unique_ptr<Firework>> _fireworks;
	void Init();
};
