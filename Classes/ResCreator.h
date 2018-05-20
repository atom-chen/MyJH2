﻿/********************************************************************
* 基础资源生产
*********************************************************************/
#ifndef _RESCREATOR_H_
#define _RESCREATOR_H_
#include "cocos2d.h"
#include "json.h"
#include "DynamicValue.h"
USING_NS_CC;

class ResCreator
{
public:
	ResCreator(std::string strid);
	~ResCreator();

	CC_SYNTHESIZE(DynamicValueInt, m_maxFarmersCount, MaxFarmersCount);//工人上限
	CC_SYNTHESIZE(DynamicValueInt, m_farmersCount, FarmersCount);//工人人数
	CC_SYNTHESIZE(DynamicValueInt, m_maxCap, MaxCap);//最大存量
	CC_SYNTHESIZE(DynamicValueInt, m_lv, Lv);//等级
	CC_SYNTHESIZE(std::string, m_name, Name);//等级
public:

	DynamicValueInt getMaxFarmersCount();

	DynamicValueInt getMaxCap();

	//保存当前数据
	void saveData();
private:
	DynamicValueInt m_maxFarmersCount;
	DynamicValueInt m_maxCap;
};
#endif

