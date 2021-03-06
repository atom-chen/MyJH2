﻿#ifndef __HEROATTR_LAYER_H__
#define __HEROATTR_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Hero.h"
#include "ResBase.h"
#include "HttpDataSwap.h"
USING_NS_CC;

typedef enum
{
	ATTR_FIREBTN = 1000,//解雇按钮
	ATTR_CHANGEBTN,//转职按钮
	ATTR_BACKBTN,//返回按钮
	ATTR_RECRUITBTN//招募按钮
}ATTRBTN;

class HeroAttrLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate, public HTTPDataDelegateProtocol
{
public:
	HeroAttrLayer();
	~HeroAttrLayer();
	static HeroAttrLayer* create(Hero* herodata, int fromwhere = 0, int clickwhere = 0);//0-客栈点击；1-训练场点击；2--医馆点击
	virtual void onExit();
    bool init(Hero* herodata, int fromwhere = 0, int clickwhere = 0);//clickwhere = 0，招募进，1-从客栈,训练场，医院进，2-卷轴进，3-出城进

	void changeEquip(ResBase* res);

	void takeOn(ResBase* res);

	void takeOff(ResBase* res);

	void changeButton();

	void delayShowNewerGuide(float dt);
	void showNewerGuide(int step);

	void fireHero();

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
	点击物品事件
	*************************************/
	void onGoodsClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/************************************
	全身像点击
	*************************************/
	void onHeroFullClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/************************************
	人物信息说明点击
	*************************************/
	void onHeroHintClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/************************************
	修改名字editBox控件
	*************************************/
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);

	void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action);

	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);

	void editBoxReturn(cocos2d::ui::EditBox *editBox);

	void updateEquipUi(ResBase* res, int barindex);

	void updataAtrrUI(float dt);

	void updateAtrBtnUI();

	void finishMovingAction();

	void updateVocationUI();

	void addHeroExp(int tag);

	void longTouchUpdate(float delay);
	void cancelLongTouch();

	void JumpSceneCallback(cocos2d::Ref* pScene, cocos2d::ui::PageView::EventType type);
	
	void loadHeroUI();

	void onFinish(int errcode);

	void modifyName(std::string utf8name = "");

	void updateHeroNameAction(std::string newname);
private:
	Node* lvnode;
	//
	Node* heroattrbottom;
	//装备栏
	Node* equipnode;
	//英雄数据
	Hero* m_heroData;
	//名字编辑控件
	cocos2d::ui::EditBox* m_editName;

	//修改PEN按钮
	cocos2d::ui::Widget* moditybtn;

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

	//经验
	cocos2d::ui::LoadingBar* expbar;
	cocos2d::ui::Text* explbl;

	//等级
	cocos2d::ui::Text* lvUIText;

	//职业
	cocos2d::ui::Text* vocation;

	//全身像
	cocos2d::ui::ImageView* herofullimg;

	cocos2d::ui::ImageView* heroattrqu;

	//点击的装备栏位置
	int clickindex;

	cocos2d::ui::Text* addexplbl;

	int langtype;

	std::string lastchangedname;

	cocos2d::ui::Button* btnArr[5];
	cocos2d::ui::Text* goodarr[4];
	bool isMovingAction;
	
	int lastVaction;
	int lastLv;
	int lastexp;
	int breakcount;

	cocos2d::ui::ImageView* blankclick;
	cocos2d::ui::ImageView* cantclick;

	bool m_isLongPress;
	Node* m_longTouchNode;

	bool isCanClickFullHero;

	cocos2d::ui::Widget* heroattrnamebox;
	cocos2d::ui::Widget* redtip;

	cocos2d::ui::PageView* pageView;
	int m_fromwhere;
	std::vector<Hero*> vec_norheros;

	Point beginTouchPoint;
	int pageMoveClickIndex;

	cocos2d::ui::Widget* redpointArr[6];
};

#endif
