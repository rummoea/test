#pragma once

#include "Bubble.h"
#include "BigBubble.h"

class BubblesWidget : public GUI::Widget
{
public:
	BubblesWidget(const std::string& name, rapidxml::xml_node<>* elem);

	~BubblesWidget();

	virtual void Draw() override;
	virtual void Update(float dt) override;
	virtual bool MouseDown(const IPoint& mouse_pos) override;

private:
	int _countPoints;
	float _bornTime;
	float _lifeTime;
	int _score;
	bool _isGameStart;
	bool _isGameFinish;
	std::vector<std::unique_ptr<Bubble>> _bubbles;
	std::vector<std::unique_ptr<BigBubble>> _bigBubbles;
	Render::Texture* _backgroundTex;

	void Init();
	void StartGame();
	void CreateBubbles();
	void DrawScore();
	void CheckCollision();

	// ѕровер€ет соприкосновение или пересечение двух окружностей
	// с центрами в p1 и p2 и радиусами r1 и r2
	static const bool IsIntersect(const FPoint& p1, float r1, const FPoint& p2, float r2);
	static const float Sqr(float num) { return num * num; }
	static const int BASE_SCORE = 100;
};

