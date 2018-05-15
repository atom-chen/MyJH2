﻿#ifndef __HEROATTR_LAYER_H__
#define __HEROATTR_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Hero.h"
USING_NS_CC;

typedef enum
{
	NEWHERO,//招募新英雄
	MYHERO//已有英雄
}ENTERTYPE;

typedef enum
{
	ATTR_FIREBTN,//解雇按钮
	ATTR_CHANGEBTN,//转职按钮
	ATTR_BACKBTN,//返回按钮
	ATTR_RECRUITBTN//招募按钮
}ATTRBTN;

class HeroAttrLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	HeroAttrLayer();
	~HeroAttrLayer();
	static HeroAttrLayer* create(ENTERTYPE etype, Hero* herodata);
	virtual void onExit();
    bool init(ENTERTYPE etype, Hero* herodata);

private:

	/************************************
	点击按钮事件
	*************************************/
	void onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/************************************
	点击装备事件
	*************************************/
	void onEquipClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/************************************
	修改名字editBox控件
	*************************************/
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);

	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);

	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);

	void editBoxReturn(cocos2d::ui::EditBox *editBox);
private:

	//英雄数据
	Hero* m_heroData;
	//名字编辑控件
	cocos2d::ui::EditBox* m_editName;

	//血量label
	cocos2d::ui::Text* hplbl;

	//攻击label
	cocos2d::ui::Text* atkbl;

	//防御label
	cocos2d::ui::Text* dflbl;

	//攻击速度label
	cocos2d::ui::Text* atkspeedlbl;

	//暴击label
	cocos2d::ui::Text* critlbl;

	//闪避label
	cocos2d::ui::Text* dodgelbl;
};

#endif