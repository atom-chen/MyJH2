#include "CommonFuncs.h"
#include "Resource.h"
#include "GlobalInstance.h"
#include "MyRes.h"
#include "MovingLabel.h"
#include "TaskDailyNode.h"
#include "Const.h"
#include "DataSave.h"
#include "TaskLayer.h"
#include "Building.h"
#include "MainScene.h"
#include "InnRoomLayer.h"
#include "SmithyLayer.h"
#include "StoreHouseLayer.h"
#include "Quest.h"
#include "ResCreator.h"
#include "AnimationEffect.h"

TaskDailyNode::TaskDailyNode()
{

}


TaskDailyNode::~TaskDailyNode()
{

}

TaskDailyNode* TaskDailyNode::create(DailyTaskData* data)
{
	TaskDailyNode *pRet = new(std::nothrow)TaskDailyNode();
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

bool TaskDailyNode::init(DailyTaskData* data)
{
	m_Data = data;

	Node* csbnode = CSLoader::createNode(ResourcePath::makePath("taskDayNode.csb"));
	this->addChild(csbnode);

	int langtype = GlobalInstance::getInstance()->getLang();

	cocos2d::ui::ImageView* resitem = (cocos2d::ui::ImageView*)csbnode->getChildByName("resitem");
	resitem->setSwallowTouches(false);

	cocos2d::ui::ImageView* res = (cocos2d::ui::ImageView*)csbnode->getChildByName("res");
	std::string str = StringUtils::format("ui/dailytask_%d.png", data->type);
	res->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);

	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(ResourceLang::map_lang["dailyrlabel"]);

	//名字
	cocos2d::ui::Text* name = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	str = StringUtils::format("dailytype_%d", data->type);
	std::string s = StringUtils::format(ResourceLang::map_lang[str].c_str(), data->count);
	name->setString(s);

	cocos2d::ui::Text* integral = (cocos2d::ui::Text*)csbnode->getChildByName("integral");
	str = StringUtils::format(ResourceLang::map_lang["dailypoint"].c_str(), data->points);
	integral->setString(str);

	finish = (cocos2d::ui::Widget*)csbnode->getChildByName("finish");
	actionbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	actionbtn->addTouchEventListener(CC_CALLBACK_2(TaskDailyNode::onbtnClick, this));
	actionbtn->setTag(data->type);
	actionbtn->setSwallowTouches(false);

	getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(TaskDailyNode::onbtnClick, this));
	getbtn->setTag(1000);
	getbtn->setSwallowTouches(false);

	taskprobar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("taskprobar");
	taskprotext = (cocos2d::ui::Text*)csbnode->getChildByName("taskprotext");

	//奖励的物品
	for (unsigned int i = 0; i < data->goods.size(); i++)
	{
		std::vector<std::string> vec = data->goods[i];
		int count = atoi(vec[1].c_str());
		std::string str = GlobalInstance::getInstance()->getResUIFrameName(vec[0], atoi(vec[2].c_str()));
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(0 + i * 70, 0));
		this->addChild(res);
		res->setScale(0.4f);

		str = StringUtils::format("x%d", count);
		Label *namelbl = Label::createWithTTF(str, FONT_NAME, 16);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setAnchorPoint(Vec2(0, 0.5));
		namelbl->setPosition(Vec2(18 + i * 70, -7));
		this->addChild(namelbl);
	}

	updateData(0);

	this->schedule(schedule_selector(TaskDailyNode::updateData), 1.0f);
	return true;
}

void TaskDailyNode::updateData(float dt)
{
	if (m_Data->state == DAILY_UNFINISHED)
	{
		finish->setVisible(false);
		getbtn->setVisible(false);
		actionbtn->setVisible(true);
	}
	else if (m_Data->state == DAILY_FINISHED)
	{
		finish->setVisible(false);
		getbtn->setVisible(true);
		actionbtn->setVisible(false);
	}
	else
	{
		finish->setVisible(true);
		getbtn->setVisible(false);
		actionbtn->setVisible(false);
	}

	int c = Quest::map_DailyTypeCount[m_Data->type];
	if (c >= m_Data->count)
	{
		c = m_Data->count;
	}
	float h = c * 100 / m_Data->count;
	taskprobar->setPercent(h);
	std::string str = StringUtils::format("%d/%d", c, m_Data->count);
	taskprotext->setString(str);
}

void TaskDailyNode::onbtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);

	Node* clicknode = (Node*)pSender;
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

		Node* node = (Node*)pSender;
		int tag = node->getTag();

		if (tag == 1000)
		{
			//物品
			for (unsigned int i = 0; i < m_Data->goods.size(); i++)
			{
				std::vector<std::string> one_res = m_Data->goods[i];
				std::string resid = one_res[0];
				int count = atoi(one_res[1].c_str());
				int qu = 0;
				int stc = 0;
				if (one_res.size()>2 && one_res[2].length()>0)
				{
					qu = atoi(one_res[2].c_str());
					stc = GlobalInstance::getInstance()->generateStoneCount(qu);
				}

				for (unsigned int i = 0; i < GlobalInstance::vec_resCreators.size(); i++)
				{
					ResCreator* rescreator = GlobalInstance::vec_resCreators[i];
					if (rescreator->getName().compare(resid) == 0)
					{
						int maxcount = rescreator->getMaxCap(rescreator->getLv().getValue()).getValue();
						if (MyRes::getMyResCount(resid)+count>maxcount)
						{
							MovingLabel::show(ResourceLang::map_lang["nomoreres"]);
							return;
						}
					}
				}
				MyRes::Add(resid, count, MYSTORAGE, qu, stc);
			}

			cocos2d::ui::Button* btn = (cocos2d::ui::Button*)pSender;
			btn->setTouchEnabled(false);

			m_Data->state = DAILY_RECEIVE;
			GlobalInstance::getInstance()->saveMyDailyTaskData();
			//积分
			int m_point = DataSave::getInstance()->getMyyDailyPoint();
			m_point += m_Data->points;
			DataSave::getInstance()->setMyDailyPoint(m_point);
		}
		else
		{
			switch (tag)
			{
			case FINISH_BRANCH:
			{
				TaskLayer* layer = (TaskLayer*)this->getParent()->getParent()->getParent()->getParent();
				if (layer != NULL)
				{
					layer->skipContent();
				}
			}
			break;
			case FRESH_PUBENLIST:
			{
				InnRoomLayer* layer = InnRoomLayer::create(Building::map_buildingDatas["6innroom"]);
				g_mainScene->addChild(layer, 0, "6innroom");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			case UPGRADE_HERO:
			{
				InnRoomLayer* layer = InnRoomLayer::create(Building::map_buildingDatas["6innroom"]);
				g_mainScene->addChild(layer, 0, "6innroom");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			case UPGRADE_BUILDING:
			{
				SmithyLayer* layer = SmithyLayer::create(Building::map_buildingDatas["2smithy"]);
				g_mainScene->addChild(layer, 0, "2smithy");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			case STRENG_EQUIP:
			{
				StoreHouseLayer* layer = StoreHouseLayer::create();
				g_mainScene->addChild(layer, 0, "3storehouse");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			case STRENG_WG:
			{
				StoreHouseLayer* layer = StoreHouseLayer::create();
				g_mainScene->addChild(layer, 0, "3storehouse");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			case DECOMPOSE_EQUIP:
			{
				StoreHouseLayer* layer = StoreHouseLayer::create();
				g_mainScene->addChild(layer, 0, "3storehouse");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			case SET_GEM:
			{
				InnRoomLayer* layer = InnRoomLayer::create(Building::map_buildingDatas["6innroom"]);
				g_mainScene->addChild(layer, 0, "6innroom");
				AnimationEffect::openAniEffect((Layer*)layer);
			}
			break;
			default:
				break;
			}
		}
	}
}