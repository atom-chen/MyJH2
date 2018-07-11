#pragma once
#pragma once
#pragma once
#ifndef _TaskTalk_LAYER_H_
#define _TaskTalk_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalInstance.h"

USING_NS_CC;

class TaskTalkLayer :public Layer
{
public:
	TaskTalkLayer();
	~TaskTalkLayer();

	virtual bool init(std::string npcid);

	static TaskTalkLayer* create(std::string npcid);

private:
	cocos2d::ui::Button* givebtn;
	cocos2d::ui::Button* fightbtn;
	cocos2d::ui::Button* closebtn;

	std::string m_npcid;

private:
	void onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

};
#endif
