﻿#include "MainMenuLayer.h"
#include "Resource.h"
#include "CommonFuncs.h"
#include "GlobalInstance.h"

USING_NS_CC;

MainMenuLayer::MainMenuLayer()
{

}

MainMenuLayer::~MainMenuLayer()
{
	
}


// on "init" you need to initialize your instance
bool MainMenuLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int langtype = GlobalInstance::getInstance()->getLang();

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("MainMenuLayer.csb"));
	this->addChild(csbnode);

	for (int i = 0; i < csbnode->getChildrenCount(); i++)
	{
		std::string name = csbnode->getChildren().at(i)->getName();
		cocos2d::ui::Widget* clickwidget = (cocos2d::ui::Widget*)csbnode->getChildByName(name);
		clickwidget->setTag(i);
		clickwidget->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::onClick, this));

		if (i >= R001BTN && i <= R005BTN)
		{
			reslbl[i - R001BTN] = (cocos2d::ui::Text*)clickwidget->getChildByName("countlbl");
		}
		else if (i == ACHBTN)
		{
			achredpoint = (cocos2d::ui::Widget*)clickwidget->getChildByName("redpoint");
			achredpoint->setVisible(false);
		}
		else if (i == MAILBTN)
		{
			mailredpoint = (cocos2d::ui::Widget*)clickwidget->getChildByName("redpoint");
			mailredpoint->setVisible(false);
		}
		else if (i == SILVERBOX)
		{
			mysilverlbl = (cocos2d::ui::Text*)clickwidget->getChildByName("countlbl");
		}
		else if (i == COINBOX)
		{
			mycoinlbl = (cocos2d::ui::Text*)clickwidget->getChildByName("countlbl");
		}

		if (i >= SETBTN && i <= SHOPBTN)
		{
			cocos2d::ui::ImageView* textimg =  (cocos2d::ui::ImageView*)clickwidget->getChildByName("text");
			std::string textname = StringUtils::format("main_%s_text", name.c_str());
			textimg->loadTexture(ResourcePath::makeTextImgPath(textname, langtype), cocos2d::ui::Widget::TextureResType::PLIST);
		}
	}
	this->schedule(schedule_selector(MainMenuLayer::updateUI), 1.0f);
    return true;
}

void MainMenuLayer::updateUI(float dt)
{
	for (int i = 0; i < 5; i++)
	{
		reslbl[i]->setString("10");
	}
	mycoinlbl->setString("100");
	mysilverlbl->setString("100");
}

void MainMenuLayer::onClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* clicknode = (Node*)pSender;
	int menuType = clicknode->getTag();
	if (menuType >= SETBTN && menuType <= SHOPBTN)
		CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		switch (menuType)
		{
		case HEADBOX:
			break;
		case R001BTN:
			break;
		case R002BTN:
			break;
		case R003BTN:
			break;
		case R004BTN:
			break;
		case R005BTN:
			break;
		case SETBTN:
			break;
		case ACHBTN:
			break;
		case RANKBTN:
			break;
		case MAILBTN:
			break;
		case SHOPBTN:
			break;
		case SILVERBOX:
		case ADDSILVERBTN:
			break;
		case COINBOX:
		case ADDCOINBTN:
			break;
		default:
			break;
		}
	}
}

void MainMenuLayer::onExit()
{
	Layer::onExit();
}