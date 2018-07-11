#pragma once
#ifndef __QUEST_H_
#define __QUEST_H_

#include "cocos2d.h"
#include "GlobalInstance.h"
class Quest
{
public:

	/*************�������������߼�**************/
	//��ʼ������ɵ�����
	static bool initFinishTaskData();

	//��ʼ����ǰ��������ɵ���Ʒ
	static void initCurNeedData();

	//�������������
	static void AddFinishQuest(TaskMainData data);

	//�ж϶�ѡһ�������Ƿ��л���,���з���true
	static bool getMutexMainQuestType(int id, int type);

	//�ж��Ƿ���ܴ�npc����
	static bool getMainQuestNpc(std::string npcid);

	//�ж�����1��������ĵ���Ʒid������
	static void setResQuestData(std::string resid, int count, std::string npcid);

	//�ж����ĵ���Ʒ�����Ƿ���
	static bool getResCountFinish();

	static void saveMainData();

	static void finishQuest();

	/*************֧�����������߼�**************/

	//�ж��Ƿ���ܴ�npc����
	static bool getBranchQuestNpc(std::string npcid);

	//�ж�������Ʒ��������ĵ���Ʒid������
	static void setResBranchQuestData(std::string resid, int count, std::string npcid);

	//�ж����ĵ���Ʒ�����Ƿ���
	static bool getResBranchFinish();

	static void saveBranchData();

	static void finishBranchQuest();

	/*************ÿ�������߼�**************/
	//ÿ������
	static void initDailyTypeCount(std::string str);
	//ÿ������
	static void setDailyTask(int type, int count);

public:
	static std::vector<TaskMainData> myFinishMainQuest;

	static std::map<std::string, int> map_NpcQuestRes;

	//֧������npc��Ʒ
	static std::map<std::string, int> map_NpcBranchQuestRes;

	//ÿ������type-count
	static std::map<int, int> map_DailyTypeCount;

};

#endif