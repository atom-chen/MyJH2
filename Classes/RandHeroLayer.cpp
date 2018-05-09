﻿#include "RandHeroLayer.h"
#include "Resource.h"
#include "CommonFuncs.h"
#include "GlobalInstance.h"
#include "Building.h"

USING_NS_CC;

#define SILVERREFRESH_NUM 100
#define COINREFRESH_NUM 10
RandHeroLayer::RandHeroLayer()
{

}

RandHeroLayer::~RandHeroLayer()
{
	
}


RandHeroLayer* RandHeroLayer::create()
{
	RandHeroLayer *pRet = new(std::nothrow)RandHeroLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

// on "init" you need to initialize your instance
bool RandHeroLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("randHeroLayer.csb"));
	this->addChild(csbnode);
	int langtype = GlobalInstance::getInstance()->getLang();

	//标题
	cocos2d::ui::ImageView* titleimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("titleimg");
	titleimg->loadTexture(ResourcePath::makeTextImgPath("innroomtitle", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	//等级
	cocos2d::ui::Text* lvUIText = (cocos2d::ui::Text*)csbnode->getChildByName("lv");
	std::string str = StringUtils::format("%d%s", Building::map_buildingDatas["innroom"]->level.getValue(), ResourceLang::map_lang["lvtext"].c_str());
	lvUIText->setString(str);


	std::string btnname[] = { "srefreshbtn", "crefreshbtn", "silverbox", "addbtn1", "coinbox", "addbtn2","closebtn"};//与BTNTYPE对应
	for (int i = 0; i < sizeof(btnname) / sizeof(btnname[0]); i++)
	{
		cocos2d::ui::Widget* btn = (cocos2d::ui::Widget*)csbnode->getChildByName(btnname[i]);
		btn->setTag(i);
		btn->addTouchEventListener(CC_CALLBACK_2(RandHeroLayer::onBtnClick, this));

		if (i == BTN_S_REFRESH)
		{
			//银子刷新按钮文字
			cocos2d::ui::ImageView* srefreshbtntxt = (cocos2d::ui::ImageView*)btn->getChildByName("text");
			srefreshbtntxt->loadTexture(ResourcePath::makeTextImgPath("srefresh_text", langtype), cocos2d::ui::Widget::TextureResType::PLIST);
		}
		else if (i == BTN_C_REFRESH)
		{
			//元宝刷新按钮文字
			cocos2d::ui::ImageView* crefreshbtntxt = (cocos2d::ui::ImageView*)btn->getChildByName("text");
			crefreshbtntxt->loadTexture(ResourcePath::makeTextImgPath("crefresh_text", langtype), cocos2d::ui::Widget::TextureResType::PLIST);
		}
		else if (i == BTN_ADD_SILVERBOX)
		{
			mysilverlbl = (cocos2d::ui::Text*)btn->getChildByName("countlbl");
		}
		else if (i == BTN_ADD_COINBOX)
		{
			mycoinlbl = (cocos2d::ui::Text*)btn->getChildByName("countlbl");
		}
	}


	refreshsilverlbl = (cocos2d::ui::Text*)csbnode->getChildByName("snumbl");
	std::string lblstr = StringUtils::format("%d", SILVERREFRESH_NUM);
	refreshsilverlbl->setString(lblstr);
	refreshcoinlbl = (cocos2d::ui::Text*)csbnode->getChildByName("cnumbl");
	lblstr = StringUtils::format("%d", COINREFRESH_NUM);
	refreshcoinlbl->setString(lblstr);


	for (int i = 0; i < 3; i++)
	{
		heronode[i] = RandHeroNode::create();
		heronode[i]->setPosition(140+i*220, 730);
		this->addChild(heronode[i]);
	}

	this->schedule(schedule_selector(RandHeroLayer::updateUI), 1.0f);

	//屏蔽下层点击
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void RandHeroLayer::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* clicknode = (Node*)pSender;
	int tag = clicknode->getTag();
	if (tag != BTN_ADD_COINBOX && tag != BTN_ADD_SILVERBOX)
		CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		switch (tag)
		{
		case BTN_S_REFRESH://银子刷新

			break;
		case BTN_C_REFRESH://元宝刷新
			break;
		case BTN_ADD_SILVERBOX://增加银子
		case BTN_ADD_SILVER://增加银子
			break;
		case BTN_ADD_COINBOX://增加元宝
		case BTN_ADD_COIN://增加元宝
			break;
		case BTN_CLOSE://关闭
			this->removeFromParentAndCleanup(true);
			break;
		default:
			break;
		}
	}
}

void RandHeroLayer::updateUI(float dt)
{
	mysilverlbl->setString("10");
	mycoinlbl->setString("10");
}

void RandHeroLayer::onExit()
{
	Layer::onExit();
}
