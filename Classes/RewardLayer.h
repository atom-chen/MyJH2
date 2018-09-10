#ifndef __REWARD_LAYER_H__
#define __REWARD_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GlobalInstance.h"

USING_NS_CC;

class RewardLayer : public cocos2d::Layer
{
public:
	RewardLayer();
	~RewardLayer();

	static RewardLayer* create(std::vector<MSGAWDSDATA> vec_rewards);
	bool init(std::vector<MSGAWDSDATA> vec_rewards);
private:
	void onBtnClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void removeSelf();

private:

};

#endif
