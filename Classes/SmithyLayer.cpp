﻿#include "SmithyLayer.h"
#include "Resource.h"
#include "CommonFuncs.h"
#include "GlobalInstance.h"
#include "ConsumeResActionLayer.h"
#include "MovingLabel.h"
#include "DataSave.h"
#include "MyRes.h"

USING_NS_CC;

SmithyLayer::SmithyLayer()
{

}

SmithyLayer::~SmithyLayer()
{
	
}


SmithyLayer* SmithyLayer::create(Building* buidingData)
{
	SmithyLayer *pRet = new(std::nothrow)SmithyLayer();
	if (pRet && pRet->init(buidingData))
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
bool SmithyLayer::init(Building* buidingData)
{
    if ( !Layer::init() )
    {
        return false;
    }

	m_buidingData = buidingData;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("smithyLayer.csb"));
	this->addChild(csbnode);

	langtype = GlobalInstance::getInstance()->getLang();
	//标题
	cocos2d::ui::ImageView* titleimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("titleimg");
	titleimg->loadTexture(ResourcePath::makeTextImgPath("smithytilte", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	//等级
	lvUIlbl = (cocos2d::ui::Text*)csbnode->getChildByName("lv");
	std::string str = StringUtils::format("%d%s", buidingData->level.getValue() + 1, ResourceLang::map_lang["lvtext"].c_str());
	lvUIlbl->setString(str);

	//升级按钮
	cocos2d::ui::Widget* lvUpbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("lvupbtn");
	lvUpbtn->setTag(1000);
	lvUpbtn->addTouchEventListener(CC_CALLBACK_2(SmithyLayer::onBtnClick, this));

	//升级按钮文字
	cocos2d::ui::ImageView* lvUpbtntxt = (cocos2d::ui::ImageView*)lvUpbtn->getChildByName("text");
	lvUpbtntxt->loadTexture(ResourcePath::makeTextImgPath("smithylv_text", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	//关闭按钮
	cocos2d::ui::Widget* closebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	closebtn->setTag(1001);
	closebtn->addTouchEventListener(CC_CALLBACK_2(SmithyLayer::onBtnClick, this));

	//滚动控件
	m_contentscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("contentscroll");

	m_contentscroll->setScrollBarEnabled(false);
	m_contentscroll->setBounceEnabled(true);

	Node* categoryBtnNode = csbnode->getChildByName("catanode");
	for (int i = 0; i < categoryBtnNode->getChildrenCount(); i++)
	{
		std::string btnstr = StringUtils::format("btn%d", i);
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)categoryBtnNode->getChildByName(btnstr);
		btn->setTag(i);
		btn->addTouchEventListener(CC_CALLBACK_2(SmithyLayer::onCategory, this));
		vec_categoryBtn.push_back(btn);
	}
	loadData();
	updateContent(0);

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

void SmithyLayer::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* clicknode = (Node*)pSender;
		int tag = clicknode->getTag();
		switch (tag)
		{
		case 1000://升级
			if (m_buidingData->level.getValue() < m_buidingData->maxlevel.getValue() - 1)
			{
				ConsumeResActionLayer* layer = ConsumeResActionLayer::create(m_buidingData, CA_BUILDINGLVUP);
				this->addChild(layer);
			}
			else
			{
				MovingLabel::show(ResourceLang::map_lang["maxlv"]);
			}
			break;
		case 1001://关闭
			this->removeFromParentAndCleanup(true);
			break;
		default:
			break;
		}
	}
}

void SmithyLayer::loadData()
{
	map_cateRes.clear();
	for (int v = 0; v <= m_buidingData->level.getValue(); v++)
	{
		if (v < m_buidingData->vec_exdata.size())
		{
			for (unsigned int i = 0; i < m_buidingData->vec_exdata[v].size(); i++)
			{
				std::string resid = m_buidingData->vec_exdata[v][i];
				if (resid.compare(0, 1, "a") == 0)
					map_cateRes[SMCATA_1].push_back(resid);
				else if (resid.compare(0, 1, "e") == 0)
					map_cateRes[SMCATA_2].push_back(resid);
				else if (resid.compare(0, 1, "h") == 0)
					map_cateRes[SMCATA_3].push_back(resid);
				else if (resid.compare(0, 1, "f") == 0)
					map_cateRes[SMCATA_4].push_back(resid);
				else
					map_cateRes[SMCATA_0].push_back(resid);
			}
		}
	}
}

void SmithyLayer::updateContent(int category)
{
	m_contentscroll->removeAllChildrenWithCleanup(true);

	for (unsigned int i = 0; i < vec_categoryBtn.size(); i++)
	{
		cocos2d::ui::ImageView* btntext = (cocos2d::ui::ImageView*)vec_categoryBtn[i]->getChildByName("text");

		if (category != i)
		{
			std::string textstr = StringUtils::format("smcatabtn%d_text_n", i);
			btntext->loadTexture(ResourcePath::makeTextImgPath(textstr, langtype), cocos2d::ui::Widget::TextureResType::PLIST);
			vec_categoryBtn[i]->setBright(true);
		}
		else
		{
			std::string textstr = StringUtils::format("smcatabtn%d_text_s", i);
			vec_categoryBtn[i]->setBright(false);
			btntext->loadTexture(ResourcePath::makeTextImgPath(textstr, langtype), cocos2d::ui::Widget::TextureResType::PLIST);
		}
	}

	int itemheight = 140;

	int size = map_cateRes[category].size();
	int innerheight = itemheight * size;
	int contentheight = m_contentscroll->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_contentscroll->setInnerContainerSize(Size(720, innerheight));

	
	for (int i = 0; i < size; i++)
	{
		Node* itemnode = CSLoader::createNode(ResourcePath::makePath("makeResNode.csb"));

		itemnode->setPosition(Vec2(m_contentscroll->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
		m_contentscroll->addChild(itemnode);

		cocos2d::ui::Widget* itembg= (cocos2d::ui::Widget*)itemnode->getChildByName("resitem");
		itembg->addTouchEventListener(CC_CALLBACK_2(SmithyLayer::onItemClick, this));
		itembg->setUserData((void*)map_cateRes[category][i].c_str());
		itembg->setSwallowTouches(false);

		cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)itemnode->getChildByName("res");
		std::string resstr = StringUtils::format("ui/%s.png", map_cateRes[category][i].c_str());
		resimg->loadTexture(ResourcePath::makePath(resstr), cocos2d::ui::Widget::TextureResType::PLIST);

		cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)itemnode->getChildByName("name");
		namelbl->setString(GlobalInstance::map_AllResources[map_cateRes[category][i]].name);

		cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)itemnode->getChildByName("desc");
		desclbl->setString(GlobalInstance::map_AllResources[map_cateRes[category][i]].desc);
	}
}


void SmithyLayer::onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (lastCategoryindex == node->getTag())
			return;

		lastCategoryindex = node->getTag();

		updateContent(node->getTag());
	}
}

void SmithyLayer::lvup()
{
	std::string str = StringUtils::format("%d%s", m_buidingData->level.getValue() + 1, ResourceLang::map_lang["lvtext"].c_str());
	lvUIlbl->setString(str);

	loadData();
	updateContent(lastCategoryindex);
}

void SmithyLayer::makeRes(std::string resid)
{
	//品质概率
	int qu = 0;
	int rnd = GlobalInstance::getInstance()->createRandomNum(100);
	if (m_buidingData->level.getValue() < 4)
	{
		qu = 0;
	}
	else if (m_buidingData->level.getValue() < 9)
	{
		if (rnd < 80)
			qu = 0;
		else
			qu = 1;
	}
	else if (m_buidingData->level.getValue() < 14)
	{
		if (rnd < 60)
			qu = 0;
		else if (rnd < 90)
			qu = 1;
		else
			qu = 2;
	}
	else
	{
		if (rnd < 55)
			qu = 0;
		else if (rnd < 85)
			qu = 1;
		else if (rnd < 95)
			qu = 2;
		else
			qu = 3;
	}
	int stc = GlobalInstance::getInstance()->generateStoneCount(qu);
	MyRes::Add(resid, 1, MYSTORAGE, qu, stc);
}

void SmithyLayer::onItemClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;

		ConsumeResActionLayer* layer = ConsumeResActionLayer::create(node->getUserData(), CA_MAKERES);
		this->addChild(layer);
	}
}