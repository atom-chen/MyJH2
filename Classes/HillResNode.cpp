﻿#include "HillResNode.h"
#include "CommonFuncs.h"
#include "Resource.h"
#include "GlobalInstance.h"

HillResNode::HillResNode()
{

}


HillResNode::~HillResNode()
{

}

HillResNode* HillResNode::create(ResCreator* data)
{
	HillResNode *pRet = new(std::nothrow)HillResNode();
	if (pRet && pRet->init(data))
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

bool HillResNode::init(ResCreator* data)
{

	m_Data = data;

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("hillResNode.csb"));
	this->addChild(csbnode);

	cocos2d::ui::Widget* resbox = (cocos2d::ui::Widget*)csbnode->getChildByName("resbox");
	resbox->addTouchEventListener(CC_CALLBACK_2(HillResNode::onImgClick, this));

	cocos2d::ui::ImageView* resimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("res");
	std::string str = StringUtils::format("ui/%s.png", data->getName().c_str());
	resimg->loadTexture(ResourcePath::makePath(str), cocos2d::ui::Widget::TextureResType::PLIST);

	//名字
	namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	namelbl->setString(GlobalInstance::map_AllResources[data->getName()].name);

	//储备
	maxcaptext = (cocos2d::ui::Text*)csbnode->getChildByName("maxcaptext");
	maxcaptext->setString(ResourceLang::map_lang["maxcaptext"]);

	maxcap = (cocos2d::ui::Text*)csbnode->getChildByName("maxcap");

	//产量
	outputtext = (cocos2d::ui::Text*)csbnode->getChildByName("outputtext");
	outputtext->setString(ResourceLang::map_lang["outputtext"]);

	output = (cocos2d::ui::Text*)csbnode->getChildByName("output");

	//工人人数
	farmercounttext = (cocos2d::ui::Text*)csbnode->getChildByName("farmercounttext");
	farmercounttext->setString(ResourceLang::map_lang["farmercounttext"]);

	farmercount = (cocos2d::ui::Text*)csbnode->getChildByName("farmercount");

	//按钮
	cocos2d::ui::Widget* actbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("actionbtn");
	actbtn->setTag(0);
	actbtn->addTouchEventListener(CC_CALLBACK_2(HillResNode::onBtnClick, this));

	int langtype = GlobalInstance::getInstance()->getLang();

	//按钮文字
	cocos2d::ui::ImageView* actbtntxt = (cocos2d::ui::ImageView*)actbtn->getChildByName("text");
	actbtntxt->loadTexture(ResourcePath::makeTextImgPath("resitemlv_text", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	//加号按钮
	cocos2d::ui::Widget* addbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("addbtn");
	addbtn->addTouchEventListener(CC_CALLBACK_2(HillResNode::onBtnClick, this));
	addbtn->setTag(1);

	//减号按钮
	cocos2d::ui::Widget* subbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("subbtn");
	subbtn->addTouchEventListener(CC_CALLBACK_2(HillResNode::onBtnClick, this));
	subbtn->setTag(2);

	updateData();

	return true;
}

void HillResNode::updateData()
{
	std::string str = StringUtils::format("%d/%d", m_Data->);
	maxcap
}


void MyHeroNode::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		InnRoomLayer* innroomLayer = (InnRoomLayer*)g_mainScene->getChildByName("6innroom");
		innroomLayer->fireHero(this->getTag());
	}
}

void MyHeroNode::onbgClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Layer* layer = HeroAttrLayer::create(MYHERO, m_heroData);
		g_mainScene->getChildByName("6innroom")->addChild(layer, 0, this->getTag());
	}
}