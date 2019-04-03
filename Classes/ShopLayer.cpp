#include "ShopLayer.h"
#include "Resource.h"
#include "CommonFuncs.h"
#include "GlobalInstance.h"
#include "Const.h"
#include "MovingLabel.h"
#include "MainScene.h"
#include "AnimationEffect.h"
#include "ShopNode.h"
#include "DynamicValue.h"
#include "MyRes.h"
#include "GiftContentLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PaySelectLayer.h"
#endif

USING_NS_CC;

int ShopLayer::payindex = -1;
bool ShopLayer::isPaying = false;

ShopLayer::ShopLayer()
{

}

ShopLayer::~ShopLayer()
{

}


ShopLayer* ShopLayer::create()
{
	ShopLayer *pRet = new(std::nothrow)ShopLayer();
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
bool ShopLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color, 0, "colorLayer");

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("shopLayer.csb"));
	this->addChild(csbnode);
	int langtype = GlobalInstance::getInstance()->getLang();

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onBtnClick, this));

	cocos2d::ui::Text* text = (cocos2d::ui::Text*)csbnode->getChildByName("qqtext");
	text->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onQQClick, this));

	std::string qqstr = ResourceLang::map_lang["officalQQ"];
	if (GlobalInstance::qq.length() > 0)
	{
		qqstr.append(GlobalInstance::qq);
		text->setString(qqstr);

		DrawNode* underlineNode = DrawNode::create();
		text->addChild(underlineNode, 1);
		underlineNode->setLineWidth(2.0f);
		underlineNode->drawLine(Vec2(0, 0), Vec2(text->getContentSize().width, 0), Color4F(text->getDisplayedColor()));
	}
	else
	{
		text->setVisible(false);
	}

	coinlbl = (cocos2d::ui::Text*)csbnode->getChildByName("coinbox")->getChildByName("countlbl");

	scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("scrollView");

	int showcount = 0;
	for (unsigned int i = 0; i < GlobalInstance::vec_shopdata.size(); i++)
	{
		if (GlobalInstance::vec_shopdata[i].show)
			showcount++;
	}

	int size = showcount;
	int itemheight = 160;

	int innerheight = itemheight * size;
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(640, innerheight));

	int step = 0;
	for (unsigned int i = 0; i < GlobalInstance::vec_shopdata.size(); i++)
	{
		if (GlobalInstance::vec_shopdata[i].show)
		{
			ShopNode* node = ShopNode::create(&GlobalInstance::vec_shopdata[i]);
			node->setTag(i);
			scrollView->addChild(node);
			//node->setPosition(Vec2(scrollView->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
			node->setPosition(Vec2(scrollView->getContentSize().width + 600, innerheight - step * itemheight - itemheight / 2));
			node->runAction(EaseSineIn::create(MoveBy::create(0.15f + step * 0.07f, Vec2(-scrollView->getContentSize().width / 2 - 600, 0))));
			step++;
		}
	}

	updateCoinLable(0);
	this->schedule(schedule_selector(ShopLayer::updateCoinLable), 0.1f);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	/*listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		AnimationEffect::closeAniEffect((Layer*)this);
	};*/
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void ShopLayer::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AnimationEffect::closeAniEffect((Layer*)this);
	}
}

void ShopLayer::onQQClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo methodInfo;
		char p_str1[32] = { 0 };
		sprintf(p_str1, "%s", GlobalInstance::qq.c_str());
		std::string clsname = StringUtils::format("%s/AppActivity", ANDOIRJNICLS);
		if (JniHelper::getStaticMethodInfo(methodInfo, clsname.c_str(), "copyToClipboard", "(Ljava/lang/String;)V"))
		{
			jstring str1 = methodInfo.env->NewStringUTF(p_str1);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str1);
		}
		MovingLabel::show(ResourceLang::map_lang["copyqq"]);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		copytoclipboard((char*)GlobalInstance::qq.c_str());
		MovingLabel::show(ResourceLang::map_lang["copyqq"]);
#endif
	}
}

void ShopLayer::updateCoinLable(float dt)
{
	std::string str = StringUtils::format("%d", GlobalInstance::getInstance()->getMyCoinCount().getValue());
	coinlbl->setString(str);
}

void ShopLayer::onExit()
{
	Layer::onExit();
}

void ShopLayer::beginPay(int index)
{
	if (isPaying)
		return;

	isPaying = true;
	payindex = index;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	//PaySelectLayer* layer = PaySelectLayer::create(payindex);
	//Director::getInstance()->getRunningScene()->addChild(layer, 1);

	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", GlobalInstance::vec_shopdata[payindex].name.c_str());
	std::string clsname = StringUtils::format("%s/PayAction", ANDOIRJNICLS);
	if (JniHelper::getStaticMethodInfo(methodInfo, clsname.c_str(), "pay", "(Ljava/lang/String;I)V"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str1, GlobalInstance::vec_shopdata[payindex].price * 100);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	buy(GlobalInstance::vec_shopdata[payindex].paycode.c_str());
#endif
}


void ShopLayer::setMessage(PYARET ret)
{
	if (ret == PAY_SUCC && payindex >= 0)
	{
		int type = GlobalInstance::vec_shopdata[payindex].type;
		if (type == COIN)
		{
			for (unsigned int i = 0; i < GlobalInstance::vec_shopdata[payindex].res.size(); i++)
			{
				std::vector<std::string> vec_res = GlobalInstance::vec_shopdata[payindex].res[i];
				std::string resid = vec_res[0];
				int count = atoi(vec_res[1].c_str());
				DynamicValueInt dal;
				dal.setValue(count);
				GlobalInstance::getInstance()->addMyCoinCount(dal);	
#ifdef UMENG
				umeng::MobClickCpp::pay(GlobalInstance::vec_shopdata[payindex].price, 2, count);
#endif
			}
			Node* buycoinlayer = Director::getInstance()->getRunningScene()->getChildByName("buycoinlayer");
			if (buycoinlayer != NULL)
			{
				AnimationEffect::closeAniEffect((Layer*)buycoinlayer);
			}
		}
		else if (type == GIFT)
		{
			for (unsigned int i = 0; i < GlobalInstance::vec_shopdata[payindex].res.size(); i++)
			{
				std::vector<std::string> vec_res = GlobalInstance::vec_shopdata[payindex].res[i];
				std::string resid = vec_res[0];
				int count = atoi(vec_res[1].c_str());
				int qu = 0;
				if (vec_res.size() > 2)
				{
					qu = atoi(vec_res[2].c_str());
				}
				int stonescount = GlobalInstance::getInstance()->generateStoneCount(qu);

				if (resid.compare("r006") == 0)
				{
					DynamicValueInt dvint;
					dvint.setValue(count);
					GlobalInstance::getInstance()->addMySoliverCount(dvint);
				}
				else if (resid.compare("r012") == 0)
				{
					DynamicValueInt dvint;
					dvint.setValue(count);
					GlobalInstance::getInstance()->addMyCoinCount(dvint);
				}
				else
					MyRes::Add(resid, count, MYSTORAGE, qu, stonescount);
			}
			std::string iconname = GlobalInstance::vec_shopdata[payindex].icon;
			if (iconname.compare("firstcharge") == 0)
			{
				GlobalInstance::isBuyFirstCharge = true;
				Node* chargelayer = Director::getInstance()->getRunningScene()->getChildByName("firstcharge");
				if (chargelayer != NULL)
					AnimationEffect::closeAniEffect((Layer*)chargelayer);
			}
			else if (iconname.compare(0, 8, "timegift") == 0)
			{
				Node * giftlayer = Director::getInstance()->getRunningScene()->getChildByName(iconname);
				if (giftlayer != NULL)
					AnimationEffect::closeAniEffect((Layer*)giftlayer);
			}
		}
		else if (type == VIP)
		{
			if (g_mainScene != NULL)
				g_mainScene->showVipReward(payindex);
		}

		if (type != VIP)
			HttpDataSwap::init(NULL)->paySuccNotice(GlobalInstance::vec_shopdata[payindex].icon, GlobalInstance::vec_shopdata[payindex].price);

		GlobalInstance::totalPayAmout.setValue(GlobalInstance::totalPayAmout.getValue() + GlobalInstance::vec_shopdata[payindex].price);

		HttpDataSwap::init(NULL)->postAllData();
		payindex = -1;
#ifdef UMENG
		umeng::MobClickCpp::event(GlobalInstance::vec_shopdata[payindex].icon.c_str());
#endif
	}
	//std::string str = StringUtils::format("buy_%d", (int)ret);
	//MovingLabel::show(ResourceLang::map_lang[str]);
	isPaying = false;
}
