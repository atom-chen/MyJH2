﻿#include "MainScene.h"
#include "Resource.h"
#include "MainMenuLayer.h"
#include "GlobalInstance.h"
#include "Building.h"
#include "InnRoomLayer.h"
#include "HomeHillLayer.h"
#include "Const.h"
#include "MyRes.h"
#include "MovingLabel.h"

USING_NS_CC;
MainScene* g_mainScene = NULL;
MainScene::MainScene()
{

}

MainScene::~MainScene()
{
	g_mainScene = NULL;
}

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_mainScene = MainScene::create();

    // add layer as a child to scene
	scene->addChild(g_mainScene);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int langtype = GlobalInstance::getInstance()->getLang();

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("MainLayer.csb"));
	this->addChild(csbnode);

	Node* menunode = MainMenuLayer::create();
	this->addChild(menunode);

	scroll_3 = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scroll_3");
	scroll_3->setScrollBarEnabled(false);
	scroll_3->jumpToPercentHorizontal(32);
	scroll_3->addEventListener(CC_CALLBACK_2(MainScene::srollviewlistenEvent, this));
	scroll_3->setSwallowTouches(false);

	scroll_2 = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scroll_2");
	scroll_2->setScrollBarEnabled(false);
	scroll_2->jumpToPercentHorizontal(32);

	scroll_1 = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scroll_1");
	scroll_1->setScrollBarEnabled(false);
	scroll_1->jumpToPercentHorizontal(32);
	std::map<std::string, Building*>::iterator it;
	int i = 1;
	for (it = Building::map_buildingDatas.begin(); it != Building::map_buildingDatas.end(); it++)
	{
		std::string buidingNomalName;
		std::string buidingSelectName;
		std::string bulidingname;
		cocos2d::ui::ImageView* buildingNomal;
		cocos2d::ui::ImageView* buildingSelect;
		cocos2d::ui::ImageView* buildnametext;

		buidingNomalName = StringUtils::format("main_%02d_n", i);//可点击
		buidingSelectName = StringUtils::format("main_%02d_s", i);//选中

		bulidingname = StringUtils::format("main_%02d_t", i);//文字

		Node* buildParent;
		if (i <= 5)
		{
			buildParent = scroll_3;
		}
		else if (i <= 8)
		{
			buildParent = scroll_2;
		}
		else
		{
			buildParent = scroll_1;
		}
		buildingNomal = (cocos2d::ui::ImageView*)buildParent->getChildByName(buidingNomalName);

		buildingSelect = (cocos2d::ui::ImageView*)buildParent->getChildByName(buidingSelectName);

		buildnametext = (cocos2d::ui::ImageView*)buildParent->getChildByName(bulidingname);

		buildnametext->loadTexture(ResourcePath::makeTextImgPath(bulidingname, langtype), cocos2d::ui::TextureResType::PLIST);

		buildingNomal->setSwallowTouches(true);
		buildingNomal->setUserData((void*)buildingSelect);
		buildingNomal->addTouchEventListener(CC_CALLBACK_2(MainScene::onBuildingClick, this));
		buildingSelect->setVisible(false);
		buildingSelect->setUserData((void*)it->first.c_str());
		log(it->first.c_str());
		i++;
	}

    return true;
}

void MainScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	HttpDataSwap::init(this)->getServerTime();
}

void MainScene::srollviewlistenEvent(Ref* ref, ui::ScrollView::EventType eventType)
{
	Vec2 pos = scroll_3->getInnerContainerPosition();
	switch (eventType) 
	{
		//最外层滑动时，带动后两层滑动，可修改时间调整效果
		case ui::ScrollView::EventType::CONTAINER_MOVED:
			//将引擎中的startAutoScrollToDestination 修改为pulic
			scroll_2->startAutoScrollToDestination(pos, 0.1f, true);
			scroll_1->startAutoScrollToDestination(pos, 0.2f, true);
			break;
		default:
			break;
	}
}

void MainScene::onBuildingClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* clicknode = (Node*)pSender;
	Node* snode = (Node*)clicknode->getUserData();
	std::string buildname = (char*)snode->getUserData();
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		snode->setVisible(true);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		snode->setVisible(false);
		Layer* layer = NULL;
		if (buildname.compare("6innroom") == 0)
		{
			layer = InnRoomLayer::create(Building::map_buildingDatas[buildname]);
		}
		else if (buildname.compare("7homehill") == 0)
		{
			layer = HomeHillLayer::create(Building::map_buildingDatas[buildname]);
		}
		if (layer != NULL)
			this->addChild(layer, 0, buildname);
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		snode->setVisible(false);
		break;
	default:
		break;
	}
}

void MainScene::onExit()
{
	Layer::onExit();
}

void MainScene::onFinish(int code)
{
	if (code == SUCCESS)
	{
		if (GlobalInstance::getInstance()->getRefreshHeroTime() == 0)
		{
			GlobalInstance::getInstance()->saveRefreshHeroTime(GlobalInstance::servertime);
		}
		if (GlobalInstance::getInstance()->getRefreshResTime() == 0)
		{
			GlobalInstance::getInstance()->saveRefreshResTime(GlobalInstance::servertime);
		}
		updateTime(0);
		this->schedule(schedule_selector(MainScene::updateTime), 1);
	}
}

void MainScene::updateTime(float dt)
{
	GlobalInstance::servertime++;
	int respasttime = GlobalInstance::servertime - GlobalInstance::getInstance()->getRefreshResTime();
	if (respasttime >= REFRESHRESTIME)
	{
		GlobalInstance::getInstance()->saveRefreshResTime(GlobalInstance::servertime - respasttime%REFRESHRESTIME);
		for (unsigned int i = 0; i < GlobalInstance::vec_resCreators.size(); i++)
		{
			ResCreator* rescreator = GlobalInstance::vec_resCreators[i];
			if (rescreator->getFarmersCount().getValue() > 0)
			{
				int addcount = respasttime * rescreator->getFarmersCount().getValue() / REFRESHRESTIME;
				int maxcount = rescreator->getMaxCap(rescreator->getLv().getValue()).getValue();

				std::string showtext = StringUtils::format("%s+%d", GlobalInstance::map_AllResources[rescreator->getName()].name.c_str(), addcount);
				MovingLabel::show(showtext);
				ResBase* resbase = MyRes::getMyResource(rescreator->getName());
				if (resbase != NULL)
				{
					if (addcount + resbase->getCount().getValue() >= maxcount)
						addcount = maxcount - resbase->getCount().getValue();
				}
				else
				{
					if (addcount > maxcount)
						addcount = maxcount;
				}
				if (addcount > 0)
					MyRes::Add(rescreator->getName(), addcount, MYSTORAGE);

			}
		}
	}
}

void MainScene::checkHint(float dt)
{

}
