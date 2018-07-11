﻿#ifndef _MOVING_LBAEL_H_
#define _MOVING_LBAEL_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class MovingLabel :public Label
{
public:
	MovingLabel();
	~MovingLabel();


	static void show(std::string text, Vec2 pos = Vec2(360, 640));
private:
	bool init(std::string text, Vec2 pos);

	static MovingLabel* create(std::string text, Vec2 pos);
	void showAction();
	void removeSelf();

private:
	static std::queue<MovingLabel*> queue_labels;
	bool ismoving;
};
#endif
