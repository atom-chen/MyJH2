#include "TaskLayer.h"
#include <algorithm>
#include "CommonFuncs.h"
#include "Const.h"
#include "SoundManager.h"
#include "Resource.h"
#include "MyRes.h"
#include "MyMenu.h"
#include "MovingLabel.h"
#include "TaskMainNode.h"
#include "TaskBranchNode.h"
#include "TaskDailyNode.h"
#include "DataSave.h"

TaskLayer::TaskLayer()
{
	lastCategoryindex = 0;
}


TaskLayer::~TaskLayer()
{
	GlobalInstance::getInstance()->saveMyTaskMainData();
	GlobalInstance::getInstance()->saveMyTaskBranchData();
	GlobalInstance::getInstance()->saveMyDailyTaskData();
}

bool TaskLayer::init()
{

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode(ResourcePath::makePath("taskLayer.csb"));
	this->addChild(m_csbnode);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	langtype = GlobalInstance::getInstance()->getLang();

	pnode = (cocos2d::ui::Widget*)m_csbnode->getChildByName("pnode");
	mypoint = (cocos2d::ui::Text*)pnode->getChildByName("mypoint");
	std::string pstr = StringUtils::format("%d", DataSave::getInstance()->getMyyDailyPoint());
	mypoint->setString(pstr);

	probar = (cocos2d::ui::LoadingBar*)pnode->getChildByName("probar");
	float per = DataSave::getInstance()->getMyyDailyPoint() * 100 / 200;
	probar->setPercent(per);

	//����
	cocos2d::ui::ImageView* titleimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("titleimg");
	titleimg->loadTexture(ResourcePath::makeTextImgPath("halltitle", langtype), cocos2d::ui::Widget::TextureResType::PLIST);

	cocos2d::ui::Widget* closebtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(TaskLayer::onBtnClick, this));

	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("scrollview");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	Node* categoryBtnNode = m_csbnode->getChildByName("catanode");
	for (int i = 0; i < categoryBtnNode->getChildrenCount(); i++)
	{
		std::string btnstr = StringUtils::format("btn%d", i);
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)categoryBtnNode->getChildByName(btnstr);
		btn->setTag(i);
		btn->addTouchEventListener(CC_CALLBACK_2(TaskLayer::onCategory, this));
		vec_categoryBtn.push_back(btn);
	}
	updateContent(0);

	updateDaily(0);
	this->schedule(schedule_selector(TaskLayer::updateDaily), 1.0f);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void TaskLayer::updateContent(int category)
{
	scrollview->removeAllChildrenWithCleanup(true);

	for (unsigned int i = 0; i < vec_categoryBtn.size(); i++)
	{
		cocos2d::ui::ImageView* btntext = (cocos2d::ui::ImageView*)vec_categoryBtn[i]->getChildByName("text");

		if (category != i)
		{
			std::string textstr = StringUtils::format("task_text_%d_n", i);
			btntext->loadTexture(ResourcePath::makeTextImgPath(textstr, langtype), cocos2d::ui::Widget::TextureResType::PLIST);
			vec_categoryBtn[i]->setBright(true);
		}
		else
		{
			std::string textstr = StringUtils::format("task_text_%d_s", i);
			vec_categoryBtn[i]->setBright(false);
			btntext->loadTexture(ResourcePath::makeTextImgPath(textstr, langtype), cocos2d::ui::Widget::TextureResType::PLIST);
		}
	}

	loadData(category);

	int ressize;
	if (category == 0)
	{
		ressize = GlobalInstance::vec_TaskMain.size();
		sort(GlobalInstance::vec_TaskMain.begin(), GlobalInstance::vec_TaskMain.end(), larger_callback);
		scrollview->setContentSize(Size(scrollview->getContentSize().width, 1010));
		pnode->setVisible(false);
	}
	else if (category == 1)
	{
		ressize = GlobalInstance::vec_TaskBranch.size();
		sort(GlobalInstance::vec_TaskBranch.begin(), GlobalInstance::vec_TaskBranch.end(), larger_branchcallback);
		scrollview->setContentSize(Size(scrollview->getContentSize().width, 1010));
		pnode->setVisible(false);
	}
	else
	{
		ressize = GlobalInstance::map_DTdata.size();
		scrollview->setContentSize(Size(scrollview->getContentSize().width, 850));
		pnode->setVisible(true);
	}

	int itemheight = 140;
	int innerheight = itemheight * ressize;

	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, innerheight));

	if (category != 2)
	{
		for (int i = 0; i < ressize; i++)
		{
			Node* node;
			if (category == 0)
			{
				node = TaskMainNode::create(&GlobalInstance::vec_TaskMain[i], this);
			}
			else if (category == 1)
			{
				node = TaskBranchNode::create(&GlobalInstance::vec_TaskBranch[i], this);
			}
			scrollview->addChild(node);
			node->setPosition(Vec2(scrollview->getContentSize().width / 2, innerheight - i*itemheight - itemheight*0.5));
		}
	}
	else
	{
		std::vector<DailyTaskData*> vec_fin;
		std::vector<DailyTaskData*> vec_unfin;
		std::vector<DailyTaskData*> vec_get;
		std::map<std::string, DailyTaskData>::iterator it;
		for (it = GlobalInstance::map_DTdata.begin(); it != GlobalInstance::map_DTdata.end(); it++)
		{
			DailyTaskData data = GlobalInstance::map_DTdata[it->first];
			if (data.state == DAILY_RECEIVE)
			{
				vec_get.push_back(&GlobalInstance::map_DTdata[it->first]);
			}
			else if (data.state == DAILY_FINISHED)
			{
				vec_fin.push_back(&GlobalInstance::map_DTdata[it->first]);
			}
			else
			{
				vec_unfin.push_back(&GlobalInstance::map_DTdata[it->first]);
			}
		}
		for (int i = 0; i < vec_fin.size(); i++)
		{
			Node* node = TaskDailyNode::create(vec_fin[i]);
			scrollview->addChild(node);
			node->setPosition(Vec2(scrollview->getContentSize().width / 2, innerheight - i*itemheight - itemheight*0.5));
		}
		for (int i = 0; i < vec_unfin.size(); i++)
		{
			Node* node = TaskDailyNode::create(vec_unfin[i]);
			scrollview->addChild(node);
			node->setPosition(Vec2(scrollview->getContentSize().width / 2, innerheight - (i + vec_fin.size())*itemheight - itemheight*0.5));
		}
		for (int i = 0; i < vec_get.size(); i++)
		{
			Node* node = TaskDailyNode::create(vec_get[i]);
			scrollview->addChild(node);
			node->setPosition(Vec2(scrollview->getContentSize().width / 2, innerheight - (i + vec_fin.size() + vec_unfin.size())*itemheight - itemheight*0.5));
		}
	}
	
}

void TaskLayer::onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

bool TaskLayer::larger_callback(TaskMainData a, TaskMainData b)
{
	int needcountA = a.isfinish;
	int needcountB = b.isfinish;
	if (needcountA < needcountB)
		return true;
	else
		return false;
}

bool TaskLayer::larger_branchcallback(TaskBranchData a, TaskBranchData b)
{
	int needcountA = a.isfinish;
	int needcountB = b.isfinish;
	if (needcountA < needcountB)
		return true;
	else
		return false;
}

void TaskLayer::updateDaily(float dt)
{
	int m_point = DataSave::getInstance()->getMyyDailyPoint();
	std::string pstr = StringUtils::format("%d", m_point);
	mypoint->setString(pstr);
	float per = m_point * 100 / 200;
	probar->setPercent(per);
}

void TaskLayer::loadData(int category)
{
	if (category == 2)
	{
		int m_point = DataSave::getInstance()->getMyyDailyPoint();
		cocos2d::ui::Widget* point;
		if (m_point >= 200)
		{
			point = (cocos2d::ui::Widget*)pnode->getChildByName("point_4");
			point->addTouchEventListener(CC_CALLBACK_2(TaskLayer::onPointClick, this));
			point->setTag(200);
		}
		else if (m_point >= 150)
		{
			point = (cocos2d::ui::Widget*)pnode->getChildByName("point_3");
			point->addTouchEventListener(CC_CALLBACK_2(TaskLayer::onPointClick, this));
			point->setTag(150);
		}
		else if (m_point >= 100)
		{
			point = (cocos2d::ui::Widget*)pnode->getChildByName("point_2");
			point->addTouchEventListener(CC_CALLBACK_2(TaskLayer::onPointClick, this));
			point->setTag(100);
		}
		else if (m_point >= 50)
		{
			point = (cocos2d::ui::Widget*)pnode->getChildByName("point_1");
			point->addTouchEventListener(CC_CALLBACK_2(TaskLayer::onPointClick, this));
			point->setTag(50);
		}
	}
}

void TaskLayer::onPointClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* pnode = (Node*)pSender;
		int tag = pnode->getTag();
		cocos2d::ui::ImageView* node = (cocos2d::ui::ImageView*)pSender;
		node->setTouchEnabled(false);
		switch (tag)
		{
		case 50:
			break;
		case 100:
			break;
		case 150:
			break;
		case 200:
			break;
		default:
			break;
		}
	}
}

void TaskLayer::onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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

void TaskLayer::skipContent()
{
	lastCategoryindex = 1;
	updateContent(1);
}