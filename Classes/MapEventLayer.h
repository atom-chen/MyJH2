﻿#ifndef __MAPEVENT_LAYER_H__
#define __MAPEVENT_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GlobalInstance.h"

USING_NS_CC;

typedef enum
{
	MAP_JUMP = 0,//跳崖
	POS_CAVE,//山洞
	POS_WOMAN,//民女
	POS_ELDER,//老人
	POS_THIEF,//山贼
	POS_BET,//赌博
	POS_BUSINESS//商人
}MAPEVENTTYPE;

class MapEventLayer : public cocos2d::Layer
{
public:
	MapEventLayer();
	~MapEventLayer();

	static MapEventLayer* create(int eventindex);
	virtual void onExit();
    bool init(int eventindex);
private:
	void onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void boxEventNode();
	void eventNothing();
	void eventHurt();

	void loadPrData();
	int getAllPr();//获取总概率
	std::string getDataIdByPr();//随机获取数据
	static bool larger_callback(EventData a, EventData b);

	int getEquipQuRand();//随机装备品质
	int getResCountRand(std::string id);//随机数量
	void loadBoxUI(cocos2d::ui::ImageView* box,std::string resid);

private:
	Node* eventnode_1;
	Node* eventnode_2;
	Node* eventnode_3;

	cocos2d::ui::ImageView* eventimg;
	std::vector<EventData> vec_eventdata;
	std::vector<std::vector<std::string>> vec_eventrewards;

	int langtype;
	int m_eventindex;
};

#endif
