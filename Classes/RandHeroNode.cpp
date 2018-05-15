﻿#include "RandHeroNode.h"
#include "CommonFuncs.h"
#include "Resource.h"
#include "HeroAttrLayer.h"

//5个品质英雄颜色
Color3B POTENTIALCOLOR[5] = { Color3B(36, 255, 82), Color3B(52, 179, 240), Color3B(252, 77, 226), Color3B(255, 214, 71), Color3B(255, 69, 69)};

RandHeroNode::RandHeroNode()
{

}


RandHeroNode::~RandHeroNode()
{

}

RandHeroNode* RandHeroNode::create(Hero* herodata)
{
	RandHeroNode *pRet = new(std::nothrow)RandHeroNode();
	if (pRet && pRet->init(herodata))
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

bool RandHeroNode::init(Hero* herodata)
{
	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("randHeroNode.csb"));
	this->addChild(csbnode);
	cocos2d::ui::Widget* bg = (cocos2d::ui::Widget*)csbnode->getChildByName("randheadbox");
	bg->addTouchEventListener(CC_CALLBACK_2(RandHeroNode::onClick, this));

	//头像框
	headbox = (cocos2d::ui::ImageView*)csbnode->getChildByName("hbox");
	
	//头像
	headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");

	//名字
	namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("name");

	//职业
	vocationtextlbl = (cocos2d::ui::Text*)csbnode->getChildByName("vocationtext");
	vocationtextlbl->setString(ResourceLang::map_lang["vocationtext"]);

	vocationlbl = (cocos2d::ui::Text*)csbnode->getChildByName("vocation");

	//品质
	potentialtextlbl = (cocos2d::ui::Text*)csbnode->getChildByName("potentialtext");
	potentialtextlbl->setString(ResourceLang::map_lang["potentialtext"]);

	potentiallbl = (cocos2d::ui::Text*)csbnode->getChildByName("potential");

	isrecruitedWidget = (cocos2d::ui::Text*)csbnode->getChildByName("isrecruited_text");
	isrecruitedWidget->setVisible(false);

	m_heroData = herodata;

	setData();
	return true;
}

void RandHeroNode::onClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Layer* layer = HeroAttrLayer::create(NEWHERO, m_heroData);
		this->getParent()->addChild(layer, 0, this->getTag());
	}
}

void RandHeroNode::setData()
{
	std::string str = StringUtils::format("ui/h_%d_%d.png", m_heroData->getVocation(), m_heroData->getSex());
	headimg->loadTexture(ResourcePath::makePath(str), cocos2d::ui::Widget::TextureResType::PLIST);

	str = StringUtils::format("ui/herobox_%d.png", m_heroData->getPotential());
	headbox->loadTexture(ResourcePath::makePath(str), cocos2d::ui::Widget::TextureResType::PLIST);

	str = StringUtils::format("vocation_%d", m_heroData->getVocation());
	vocationlbl->setString(ResourceLang::map_lang[str]);

	str = StringUtils::format("potential_%d", m_heroData->getPotential());
	potentiallbl->setString(ResourceLang::map_lang[str]);

	potentialtextlbl->setColor(Color3B(POTENTIALCOLOR[m_heroData->getPotential()]));
	potentiallbl->setColor(Color3B(POTENTIALCOLOR[m_heroData->getPotential()]));

	namelbl->setString(m_heroData->getName());
}

void RandHeroNode::markRecruited()
{
	isrecruitedWidget->setVisible(true);
}