﻿#include "EventBusinessLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "Resource.h"
#include "GlobalInstance.h"
#include "MyRes.h"
#include "MapEventLayer.h"
#include "BuySingleResLayer.h"
#include "AnimationEffect.h"
#include "SimpleResPopLayer.h"
#include "EquipDescLayer.h"
#include "MapBlockScene.h"

EventBusinessLayer::EventBusinessLayer()
{
}


EventBusinessLayer::~EventBusinessLayer()
{
}

EventBusinessLayer* EventBusinessLayer::create()
{
	EventBusinessLayer *pRet = new(std::nothrow)EventBusinessLayer();
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

bool EventBusinessLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color,0,"colorLayer");

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("eventBusinessLayer.csb"));
	this->addChild(csbnode);

	int langtype = GlobalInstance::getInstance()->getLang();

	//标题
	cocos2d::ui::ImageView* titleimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("titleimg");
	titleimg->loadTexture(ResourcePath::makeTextImgPath("eventbusinesstitle", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	cocos2d::ui::Widget* closebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(EventBusinessLayer::onBtnClick, this));

	cocos2d::ui::ImageView* mypackagetxt = (cocos2d::ui::ImageView*)csbnode->getChildByName("sepbox")->getChildByName("text");
	mypackagetxt->loadTexture(ResourcePath::makeTextImgPath("package_text", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	cocos2d::ui::Text* carrytext = (cocos2d::ui::Text*)csbnode->getChildByName("carrytext");
	carrytext->setString(ResourceLang::map_lang["carrytext"]);

	carrycountlbl = (cocos2d::ui::Text*)csbnode->getChildByName("carrycount");

	mycoinlbl = (cocos2d::ui::Text*)csbnode->getChildByName("coincountlbl");
	mysilverlbl = (cocos2d::ui::Text*)csbnode->getChildByName("silvercountlbl");

	cocos2d::ui::ScrollView* buyscrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview");
	buyscrollview->setScrollBarEnabled(false);
	buyscrollview->setBounceEnabled(true);
	vec_scrollview.push_back(buyscrollview);

	cocos2d::ui::ScrollView* mypackagescrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollview0");
	mypackagescrollview->setScrollBarEnabled(false);
	mypackagescrollview->setBounceEnabled(true);
	vec_scrollview.push_back(mypackagescrollview);

	updateRichLabel(0);

	this->schedule(schedule_selector(EventBusinessLayer::updateRichLabel), 1.0f);

	this->scheduleOnce(schedule_selector(EventBusinessLayer::delayShowUI), 0.1f);
	//屏蔽下层点击
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);

	return true;
}

void EventBusinessLayer::delayShowUI(float dt)
{
	loadScrollviewData();
	updateScrollviewContent();
}

void EventBusinessLayer::updateScrollviewContent()
{
	for (unsigned int i = 0; i < 2; i++)
	{
		cocos2d::ui::ScrollView* sv = vec_scrollview[i];
		sv->removeAllChildrenWithCleanup(true);

		std::vector<MSGAWDSDATA> vec_res;
		if (i == 0)
		{
			std::sort(vec_buyres.begin(), vec_buyres.end(), sortBuyResByType);
			vec_res = vec_buyres;
		}
		else
			vec_res = vec_mypackagres;
		int ressize = vec_res.size();
		int row = ressize % 4 == 0 ? ressize / 4 : (ressize / 4 + 1);

		int itemheight = 160;

		int innerheight = itemheight * row;

		int contentheight = sv->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		sv->setInnerContainerSize(Size(sv->getContentSize().width, innerheight));


		for (unsigned int m = 0; m < vec_res.size(); m++)
		{
			std::string resid = vec_res[m].rid;
			int qu = 0;
			std::string qustr = "ui/resbox.png";

			int t = 0;
			for (; t < sizeof(RES_TYPES_CHAR) / sizeof(RES_TYPES_CHAR[0]); t++)
			{
				if (resid.compare(0, 1, RES_TYPES_CHAR[t]) == 0)
					break;
			}
			if (t >= T_ARMOR && t <= T_FASHION)
			{
				qu = vec_res[m].qu;
				qustr = StringUtils::format("ui/resbox_qu%d.png", qu);
			}
			else if (t >= T_WG && t <= T_NG)
			{
				qu = GlobalInstance::map_GF[resid].qu;
				qustr = StringUtils::format("ui/resbox_qu%d.png", qu);
			}
			else if (t >= T_RENS && t <= T_BOX)
			{
				qu = atoi(resid.substr(1).c_str()) - 1;
				qustr = StringUtils::format("ui/resbox_qu%d.png", qu);
			}

			cocos2d::ui::ImageView* boxItem = cocos2d::ui::ImageView::create(qustr, cocos2d::ui::Widget::TextureResType::PLIST);
			boxItem->addTouchEventListener(CC_CALLBACK_2(EventBusinessLayer::onclick, this));
			boxItem->setTouchEnabled(true);

			boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 20 + m % 4 * itemheight, innerheight - itemheight / 2 - m / 4 * itemheight + 5));
			boxItem->setTag(i * 10000 + m);

			sv->addChild(boxItem);

			CommonFuncs::playResBoxEffect(boxItem, t, qu, 0);

			std::string str = GlobalInstance::getInstance()->getResUIFrameName(resid, qu);

			Sprite * res = Sprite::createWithSpriteFrameName(str);
			res->setPosition(Vec2(boxItem->getContentSize().width / 2, boxItem->getContentSize().height / 2));
			boxItem->addChild(res);

			if (t == T_EPIECE)
			{
				Sprite* pieceicon = Sprite::createWithSpriteFrameName("ui/pieceicon.png");
				pieceicon->setAnchorPoint(Vec2(0, 1));
				pieceicon->setPosition(10, boxItem->getContentSize().height - 10);
				boxItem->addChild(pieceicon);
			}

			Label *namelbl = Label::createWithTTF(GlobalInstance::map_AllResources[resid].name, FONT_NAME, 23);
			namelbl->setColor(Color3B(34, 74, 79));
			namelbl->setPosition(Vec2(boxItem->getContentSize().width / 2, -10));
			boxItem->addChild(namelbl);

			std::string countstr = StringUtils::format("%d", vec_res[m].count);
			Label *countlbl = Label::createWithTTF(countstr, FONT_NAME, 23);
			countlbl->setAnchorPoint(Vec2(1, 0));
			countlbl->setColor(Color3B::WHITE);
			countlbl->setPosition(Vec2(boxItem->getContentSize().width - 10, 10));
			boxItem->addChild(countlbl);
		}
	}
	std::string str = StringUtils::format("%d/%d", MyRes::getMyPackageCount(), GlobalInstance::myOutMapCarry/*GlobalInstance::getInstance()->getTotalCarry()*/);
	carrycountlbl->setString(str);

	if (MyRes::getMyPackageCount() >= GlobalInstance::myOutMapCarry)
	{
		carrycountlbl->setTextColor(Color4B(255, 61, 61, 255));
	}
	else
	{
		carrycountlbl->setTextColor(Color4B(255, 255, 255, 255));
	}
}

void EventBusinessLayer::loadScrollviewData()
{
	MapEventLayer* parentlayer = (MapEventLayer*)this->getParent();

	int rcount = GlobalInstance::getInstance()->createRandomNum(5) + 8;

	MapEventLayer::loadEventData("shopevent");
	MapEventLayer::loadPrData();

	for (int i = 0; i < rcount; i++)
	{
		std::string resid = MapEventLayer::getDataIdByPr();
		int m_count = MapEventLayer::getResCountRand(resid);
		int qu = 0;
		int t = 0;
		for (; t < sizeof(RES_TYPES_CHAR) / sizeof(RES_TYPES_CHAR[0]); t++)
		{
			if (resid.compare(0, 1, RES_TYPES_CHAR[t]) == 0)
				break;
		}
		if (t >= T_ARMOR && t <= T_FASHION)
		{
			qu = MapEventLayer::getEquipQuRand(resid);
		}
		else if (t >= T_WG && t <= T_NG)
		{
			qu = GlobalInstance::map_GF[resid].qu;
		}
		else if (t >= T_RENS && t <= T_BOX)
		{
			qu = atoi(resid.substr(1).c_str()) - 1;
		}

		MSGAWDSDATA fprop;
		fprop.rid = resid;
		fprop.count = m_count;
		fprop.qu = qu;
		vec_buyres.push_back(fprop);
	}
	loadMyPackageRes();
}

void EventBusinessLayer::loadMyPackageRes()
{
	vec_mypackagres.clear();
	for (unsigned int i = 0; i < MyRes::vec_MyResources.size(); i++)
	{
		ResBase* res = MyRes::vec_MyResources[i];
		if (res->getWhere() == MYPACKAGE)
		{
			MSGAWDSDATA fprop;
			fprop.rid = res->getId();
			fprop.count = res->getCount().getValue();
			fprop.qu = res->getQU().getValue();
			vec_mypackagres.push_back(fprop);
		}
	}
}

void EventBusinessLayer::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AnimationEffect::closeAniEffect(this);
	}
}

void EventBusinessLayer::buyRes(MSGAWDSDATA res)
{
	for (unsigned int i = 0; i < vec_buyres.size(); i++)
	{
		std::string resid = vec_buyres[i].rid;
		if (resid.compare(res.rid) == 0)
		{
			if (vec_buyres[i].count <= res.count)
				vec_buyres.erase(vec_buyres.begin() + i);
			else
				vec_buyres[i].count -= res.count;

			
			MyRes::Add(resid, res.count, MYPACKAGE, res.qu, GlobalInstance::getInstance()->generateStoneCount(res.qu));
			break;
		}
	}

	if (res.qu >= 4)
	{
		std::string st = StringUtils::format("potential_%d", res.qu);
		std::string namestr = StringUtils::format(ResourceLang::map_lang["libraryinfoequipname"].c_str(), GlobalInstance::map_AllResources[res.rid].name.c_str(), ResourceLang::map_lang[st].c_str());

		std::string contentstr = StringUtils::format(ResourceLang::map_lang["newtemplet2"].c_str(), GlobalInstance::getInstance()->getMyNickName().c_str(), namestr.c_str());
		MainScene::addNews(contentstr, 2);
	}

	updateRichLabel(0);

	loadMyPackageRes();
	updateScrollviewContent();

	g_MapBlockScene->calcStar(SA_BUSIBUY);
}

void EventBusinessLayer::updateRichLabel(float dt)
{
	std::string richstr = StringUtils::format("%d", GlobalInstance::getInstance()->getMyCoinCount().getValue());
	mycoinlbl->setString(richstr);
	richstr = StringUtils::format("%d", GlobalInstance::getInstance()->getMySoliverCount().getValue());
	mysilverlbl->setString(richstr);
}

void EventBusinessLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

	Node* clicknode = (Node*)pSender;
	int tag = clicknode->getTag();

	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		clickflag = true;
		beginTouchPoint = clicknode->convertToWorldSpace(Vec2(clicknode->getPositionX(), clicknode->getPositionY()));
	}
	else if (type == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 movedPoint = clicknode->convertToWorldSpace(Vec2(clicknode->getPositionX(), clicknode->getPositionY()));

		if (fabs(movedPoint.x - beginTouchPoint.x) >= CLICKOFFSETP || fabs(movedPoint.y - beginTouchPoint.y) >= CLICKOFFSETP)
			clickflag = false;
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!clickflag)
			return;
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		if (tag / 10000 == 0)
		{
			BuySingleResLayer* layer = BuySingleResLayer::create(vec_buyres[tag]);
			this->addChild(layer);
			AnimationEffect::openAniEffect(layer);
		}
		else
		{
			std::string resid = vec_mypackagres[tag % 10000].rid;
			int t = -1;
			for (int k = 0; k < sizeof(RES_TYPES_CHAR) / sizeof(RES_TYPES_CHAR[0]); k++)
			{
				if (resid.compare(0, 1, RES_TYPES_CHAR[k]) == 0)
				{
					t = k;
					break;
				}
			}

			if (t >= T_ARMOR && t <= T_NG)
			{
				Layer* layer = EquipDescLayer::create(resid, vec_mypackagres[tag % 10000].qu, 1);
				this->addChild(layer);
				AnimationEffect::openAniEffect(layer);
			}
			else
			{
				SimpleResPopLayer* layer = SimpleResPopLayer::create(resid);
				this->addChild(layer);
				AnimationEffect::openAniEffect(layer);
			}
		}
	}
}

bool EventBusinessLayer::sortBuyResByType(MSGAWDSDATA a, MSGAWDSDATA b)
{
	if (a.count > b.count)
		return true;
	else 
		return false;
}