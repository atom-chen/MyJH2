#include "MessageDescLayer.h"
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "Resource.h"
#include "MyRes.h"
#include "HttpDataSwap.h"
#include "MessageLayer.h"
#include "MovingLabel.h"

MessageDescLayer::MessageDescLayer()
{

}


MessageDescLayer::~MessageDescLayer()
{

}

MessageDescLayer* MessageDescLayer::create(int index)
{
	MessageDescLayer *pRet = new(std::nothrow)MessageDescLayer();
	if (pRet && pRet->init(index))
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

bool MessageDescLayer::init(int index)
{
	MessageData data = GlobalInstance::vec_messsages[index];

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* m_csbnode = CSLoader::createNode(ResourcePath::makePath("messageDescLayer.csb"));
	this->addChild(m_csbnode);

	cocos2d::ui::Widget* smallbg = (cocos2d::ui::Widget*)m_csbnode->getChildByName("smallbg");
	smallbg->setSwallowTouches(true);

	cocos2d::ui::Widget* awdnode = (cocos2d::ui::Widget*)m_csbnode->getChildByName("awdnode");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int langtype = GlobalInstance::getInstance()->getLang();

	std::string actiontextstr;
	int btntag = 0;

	int textscrollheight = 0;
	std::string contentstr = ResourceLang::map_lang["testtext"]/*data.content*/;
	if (data.type == 0)
	{
		actiontextstr = "closebtn_text";
		btntag = 1000;
		textscrollheight = 500;
		awdnode->setVisible(false);
	}
	else
	{
		actiontextstr = "msgallget_text";
		btntag = 1001;
		textscrollheight = 350;

		contentstr = data.content;


		int pos = data.content.find_last_of("$$");
		if (pos >= 0)
		{
			contentstr = contentstr.substr(0, pos - 1);
			std::string awdstr = data.content.substr(pos + 2);
			std::vector<std::string> vec_str;
			CommonFuncs::split(awdstr, vec_str, ";");
			for (unsigned int n = 0; n < vec_str.size(); n++)
			{
				std::vector<std::string> one_str;
				CommonFuncs::split(vec_str[n], one_str, "-");

				MSGAWDSDATA adata;
				adata.rid = one_str[0];
				adata.count = atoi(one_str[1].c_str());
				adata.qu = atoi(one_str[2].c_str());
				awdslist.push_back(adata);
			}
		}

		int asize = awdslist.size();
		for (int m = 0; m < 5; m++)
		{
			int qu = awdslist[m].qu;
			std::string str = StringUtils::format("resbox%d", m);
			cocos2d::ui::ImageView*  resbox = (cocos2d::ui::ImageView*)awdnode->getChildByName(str);
			str = StringUtils::format("ui/resbox_qu%d.png", qu);
			resbox->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);

			if (m < asize)
			{
				std::string resid = awdslist[m].rid;
				cocos2d::ui::ImageView* res = (cocos2d::ui::ImageView*)resbox->getChildByName("res");
				str = StringUtils::format("ui/%s.png", resid.c_str());
				if (qu == 3)
				{
					str = StringUtils::format("ui/%s-2.png", resid.c_str());
				}
				else if (qu == 4)
				{
					str = StringUtils::format("ui/%s-3.png", resid.c_str());
				}
				resbox->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
				cocos2d::ui::Text* countlbl = (cocos2d::ui::Text*)resbox->getChildByName("countlbl");
				str = StringUtils::format("%d", awdslist[m].count);
				countlbl->setString(str);

				cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)resbox->getChildByName("name");
				namelbl->setString(GlobalInstance::map_AllResources[resid].name);
			}
			else
			{
				resbox->setVisible(false);
			}
		}
	}
	cocos2d::ui::Widget* actionbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(MessageDescLayer::onBtnClick, this));
	actionbtn->setTag(btntag);
	//��ť1����
	cocos2d::ui::ImageView* actionbtntxt = (cocos2d::ui::ImageView*)actionbtn->getChildByName("text");
	actionbtntxt->loadTexture(ResourcePath::makeTextImgPath(actiontextstr, langtype), cocos2d::ui::Widget::TextureResType::PLIST);
	actionbtntxt->ignoreContentAdaptWithSize(true);

	//����
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(data.title);

	cocos2d::ui::ScrollView* contentscroll = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("contentscroll");

	contentscroll->setContentSize(Size(contentscroll->getContentSize().width, textscrollheight));
	contentscroll->setBounceEnabled(true);

	int contentwidth = 500;
	Label* contentlbl = Label::createWithTTF(contentstr, FONT_NAME, 25);
	contentlbl->setAnchorPoint(Vec2(0, 1));
	contentlbl->setColor(Color3B(255, 255, 255));
	contentlbl->setHorizontalAlignment(TextHAlignment::LEFT);
	contentlbl->setLineBreakWithoutSpace(true);
	contentlbl->setMaxLineWidth(contentwidth);
	contentscroll->addChild(contentlbl);
	int innerheight = contentlbl->getStringNumLines() * 25;
	int contentheight = contentscroll->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	contentscroll->setInnerContainerSize(Size(contentscroll->getContentSize().width, innerheight));
	contentlbl->setPosition(Vec2((contentscroll->getContentSize().width - contentwidth)/2, innerheight));

	if (data.status == 0)
	{
		HttpDataSwap::init(NULL)->updateMessageStatus(data.id, 1);
	}

	this->scheduleOnce(schedule_selector(MessageDescLayer::delay), 0.5f);

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

void MessageDescLayer::delay(float dt)
{
	int tag = this->getTag();
	GlobalInstance::vec_messsages[tag].status = 1;
	MessageLayer* mlayer = (MessageLayer*)this->getParent();
	mlayer->updateStatus(tag);
}

void MessageDescLayer::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		switch (tag)
		{
		case 1000:
			this->removeFromParentAndCleanup(true);
			break;
		case 1001:
		{
			for (unsigned int i = 0; i < awdslist.size(); i++)
			{
				int qu = awdslist[i].qu;
				int stc = GlobalInstance::getInstance()->generateStoneCount(qu);
				MyRes::Add(awdslist[i].rid, awdslist[i].count, MYSTORAGE, qu, stc);
			}
			MovingLabel::show(ResourceLang::map_lang["msgawdsucc"]);

			this->removeFromParentAndCleanup(true);
		}
			break;

		default:
			break;
		}
	}
}