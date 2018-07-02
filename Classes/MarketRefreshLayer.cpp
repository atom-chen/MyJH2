#include "MarkerRefreshLayer.h"
#include "Resource.h"
#include "CommonFuncs.h"
#include "GlobalInstance.h"
#include "Const.h"
#include "MovingLabel.h"
#include "MyRes.h"
#include "MarketLayer.h"

USING_NS_CC;

#define SILVERREFRESH_NUM 100
#define COINREFRESH_NUM 10

MarkerRefreshLayer::MarkerRefreshLayer()
{

}

MarkerRefreshLayer::~MarkerRefreshLayer()
{

}


MarkerRefreshLayer* MarkerRefreshLayer::create()
{
	MarkerRefreshLayer *pRet = new(std::nothrow)MarkerRefreshLayer();
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
bool MarkerRefreshLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("marketRefreshLayer.csb"));
	this->addChild(csbnode);
	int langtype = GlobalInstance::getInstance()->getLang();

	cocos2d::ui::Widget* smallbg = (cocos2d::ui::Widget*)csbnode->getChildByName("smallbg");
	smallbg->setSwallowTouches(true);

	//按钮
	cocos2d::ui::Widget* coinbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("coinbtn");
	coinbtn->addTouchEventListener(CC_CALLBACK_2(MarkerRefreshLayer::onreBtnClick, this));
	coinbtn->setTag(10);

	cocos2d::ui::Widget* sliverbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("sliverbtn");
	sliverbtn->addTouchEventListener(CC_CALLBACK_2(MarkerRefreshLayer::onreBtnClick, this));
	sliverbtn->setTag(100);

	cocos2d::ui::Widget* closebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(MarkerRefreshLayer::closebtnClick, this));

	//屏蔽下层点击
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		this->removeFromParentAndCleanup(true);
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void MarkerRefreshLayer::onreBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* clicknode = (Node*)pSender;
		int tag = clicknode->getTag();
		switch (tag)
		{
		case 10://元宝刷新
			if (GlobalInstance::getInstance()->getMyCoinCount().getValue() >= COINREFRESH_NUM)
			{
				DynamicValueInt dval;
				dval.setValue(COINREFRESH_NUM);
				GlobalInstance::getInstance()->costMyCoinCount(dval);
				MarketLayer* market = (MarketLayer*)this->getParent();
				if (market!=NULL)
				{
					market->resetStockRes();
				}
				this->removeFromParentAndCleanup(true);
			}
			else
			{
				MovingLabel::show(ResourceLang::map_lang["nomorecoin"]);
			}
			break;
		case 100://银两刷新
			if (GlobalInstance::getInstance()->getMySoliverCount().getValue() >= SILVERREFRESH_NUM)
			{
				DynamicValueInt dval;
				dval.setValue(SILVERREFRESH_NUM);
				GlobalInstance::getInstance()->costMySoliverCount(dval);
				MarketLayer* market = (MarketLayer*)this->getParent();
				if (market != NULL)
				{
					market->resetStockRes();
				}
				this->removeFromParentAndCleanup(true);
			}
			else
			{
				MovingLabel::show(ResourceLang::map_lang["nomoresilver"]);
			}
			break;
		default:
			break;
		}
	}
}

void MarkerRefreshLayer::closebtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void MarkerRefreshLayer::onExit()
{
	Layer::onExit();
}