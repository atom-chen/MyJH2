﻿/****************************************************************
静态数据操作类
****************************************************************/
#ifndef _STATIC_DATA_H_
#define _STATIC_DATA_H_
#include "cocos2d.h"
#include "Hero.h"
#include "ResCreator.h"
#include "ResBase.h"
USING_NS_CC;

typedef struct
{
	std::string id;
	std::vector<int> vec_exp;//升级所需经验值
	std::vector<float> vec_atk;//攻击
	std::vector<float> vec_df;//防御
	std::vector<float> vec_maxhp;//最大血量
	std::vector<float> vec_avoid;//闪避
	std::vector<float> vec_crit;//暴击
	std::vector<float> vec_atkspeed;//攻击速度
	std::vector<int> vec_attrbias;//属性偏向
}S_HeroAttr;

//小地图数据
typedef struct
{
	std::string id;
	int ph;//小地图消耗的精力，体力
	int pf;//没走一次消耗的食物
	int bgtype;//背景图片类型
	std::vector<std::string> vec_awd;//可能获得物品
	std::vector<std::string> vec_starc;//3星条件
	std::vector<std::string> vec_f3starawds;//首次获得3星奖励
}S_SubMap;

//大地图数据
typedef struct
{
	std::string id;
	std::map<std::string, S_SubMap> map_sublist;//小地图list
}S_MainMap;

typedef struct
{
	std::string id;
	std::string name;
	std::string desc;
	std::vector<std::map<std::string, int>> vec_needres;//合成需要的资源
	int silverval;//银子价格
	int coinval;//元宝价格
	int friendly;//物品赠送好友度
}AllResources;

typedef struct
{
	std::string id;
	std::vector<float> vec_bns;//额外加成
	int maxhp;
	int atk;
	int df;
	float avoid;//闪避率
	float crit;//暴击率
	float speed;//攻击速度
	int type;//类型；0--刀，剑...
}EquipData;

typedef struct
{
	std::string id;
	std::vector<std::string> vec_suit;
	std::vector<float> vec_bns;
}EquipSuit;

typedef struct
{
	std::string id;
	std::vector<float> vec_herobns;//对应英雄职业加成
	std::vector<int> vec_skillbns;//技能加成
	int skillrnd;
	float skilleff1;
	int skilleff2;
	int skill;
	int qu;
	std::vector<int> vec_hp;
	std::vector<int> vec_atk;
	std::vector<int> vec_df;
	std::vector<float> vec_avoid;//闪避率
	std::vector<float> vec_crit;//暴击率
	std::vector<float> vec_speed;//攻击速度
	std::vector<int> vec_exp;//等级经验
}GFData;

typedef struct
{
	std::string id;
	int pr;
	int max;
	int min;
	int maxqu;
}EventData;

typedef enum
{
	//是否完成此任务，0已接受未完成，1未接受任务，2已完成未领取，3已领取奖励
	QUEST_ACC = 0,
	QUEST_TASK,
	QUEST_FINISH,
	QUEST_GET
}QUESTSTATE;

typedef enum
{
	QUEST_GIVE = 1,//1表示给东西
	QUEST_FIGHT,//战斗
	QUEST_NOTFIGHT,//不战斗
	QUEST_TAKEMY,//拿东西走人
	QUEST_OTHER
}QUESTTYPE;

typedef enum
{
	//0未完成,1已完成，2已领取
	DAILY_UNFINISHED = 0,
	DAILY_FINISHED,
	DAILY_RECEIVE,
}DAILYSTATE;

typedef enum
{
	FINISH_BRANCH = 0,//完成x个支线任务
	FRESH_PUBENLIST,//刷新x次酒馆招募
	UPGRADE_HERO,//升级x次英雄角色
	UPGRADE_BUILDING,//升级x次建筑物
	STRENG_EQUIP,//强化x次装备
	STRENG_WG,//升级x次武功
	DECOMPOSE_EQUIP,//分解x次装备
	SET_GEM, //镶嵌x次宝石
	ACHIEVE_FIGHT,//打败XXnpcX次
	ACHIEVE_GOODS,//获得XX物品XX个
	ACHIEVE_GOLD,//消耗XX个元宝
	ACHIEVE_SLIVER,//消耗XX个银两
	HERO_LEVEL,//角色等级到X级
	SAFE_WOMAN,//救民女
	SAFE_OLDER,//救老人
	GET_STARS,//获得评星
	FINISH_MAIN//完成主线任务
}QUESTDAYTYPE;

typedef struct
{
	int id;
	std::string name;
	std::vector<int> type;//条件二选一，1表示给东西，2表示战斗,二选一即可完成任务(只有1和2两种类型)
	std::string desc;
	std::string place;
	std::string npcid;
	std::string bossword;
	std::vector<std::map<std::string, int>> need1;//条件二选一，物品，r001-10(id-count)
	std::string need1desc;
	std::vector<std::map<std::string, int>> need2;//条件二选一，战斗，填写type
	std::string need2desc;
	std::vector<int> mutex1;//互斥1，1-1,id为1的type为1的互斥
	std::vector<int> mutex2;//互斥2
	std::vector<std::vector<std::string>> reward1;//条件1的奖励
	std::vector<std::vector<std::string>> reward2;//条件2的奖励
	std::string bossword1;
	std::string bossword2;
	int isFight1;
	int isFight2;
	int isfinish;//是否完成此任务，0已接受未完成，1未接受任务，2已完成未领取，3已领取奖励
	int finishtype;//完成任务类型，1表示条件1完成，2表示条件2完成
}TaskData;

typedef struct
{
	std::string id;
	int type;
	int count;
	std::vector<std::vector<std::string>> goods;//物品，r001-10(id-count)
	int points;
	int state;//当前状态
}DailyTaskData;

typedef struct
{
	int vocation;
	std::vector<int> vec_maxhp;
	std::vector<int> vec_atk;
	std::vector<int> vec_df;
	std::vector<float> vec_avoid;//闪避率
	std::vector<float> vec_crit;//暴击率
	std::vector<float> vec_speed;//攻击速度
	std::vector<int> vec_bnsexp;//战斗获得经验值
}NPCAttrData;

typedef struct
{
	std::string id;
	int vocation;
	std::string icon;
}NPCData;

typedef enum
{
	SKILL_1 = 1,//技能
	SKILL_2,
	SKILL_3,
	SKILL_4,
	SKILL_5,
	SKILL_6,
	SKILL_7,
	SKILL_8,
	SKILL_9,
	SKILL_10,
	SKILL_11,
	SKILL_12,
	SKILL_13,
	SKILL_14,
	SKILL_15,
	SKILL_16,
	SKILL_17,
	SKILL_18,
	SKILL_19,
	SKILL_20
}SKILLTYPE;

typedef struct
{
	std::string id;
	std::vector<std::string> vec_awds;
	std::vector<int> vec_rnd;
}TBoxData;

typedef struct
{
	std::string id;
	std::string title;
	std::string content;
	int status;
	int type;//0--公告；1--奖励，2--补道具，3-活动
	int subtype;//0--纯公告；1--强制更新；2--不强制
}MessageData;

typedef struct
{
	std::string id;
	int type;
	std::string achid;
	int count;
	std::vector<std::vector<std::string>> rewards;//奖励id-count-qu
	int state;
}AchieveData;

typedef enum
{
	COIN = 0,//元宝
	GIFT,
	VIP,
	OTHER
}SHOPTYPE;

typedef struct
{
	std::string icon;
	int type;
	std::string name;
	int price;
	std::vector<std::vector<std::string>> res;//奖励id-count-qu
	std::string desc;
	int count;
	bool show;
	std::string paycode;
}ShopData;

typedef struct
{
	std::string rid;
	int count;
	int qu;
}MSGAWDSDATA;

//npc关系
typedef enum
{
	NPC_NORMOL = 0,//无关系
	NPC_MASTER,
	NPC_FRIEND,
	NPC_COUPEL
}NPCRELATION;

//npc加成属性
typedef enum
{
	HP = 0,
	ATTACK,//攻击
	DEFENCE,//防御
	AVOID,//闪避
	CRIT,//暴击
	SPEED//速度
}NPCQUA;

typedef struct
{
	std::string npcid;
	std::vector<std::string> talk;
	std::vector<std::string> mytalk;
	std::string desc;
	std::vector<std::vector<std::string>> reward;//获得物品id-count-qu
	std::vector<std::string> res;
	std::vector<std::string> enemynpc;
	int friendmax;//好友度上限
	int friendneed;//可结交好友度
	int masterneed;//可拜师好友度
	int marryneed;//可结亲好友度
	std::string friendword;//结交成功对话
	std::string masterword;
	std::string marryword;
	std::vector<float> masterratio;
	std::vector<float> conpelratio;
	std::vector<float> friendratio;
	int fightcost;
}NpcRelationData;

typedef struct
{
	std::string npcid;
	int friendly;//好友度
	std::vector<int> relation;//关系状态
}NpcFriendly;

typedef struct
{
	bool isopen;
	int lefttime;
	int turn;
}S_TimeGift;

typedef struct
{
	int getrewardstate;//0未到时间，1到时间可领取，2已领取
	std::string endtime;
	std::string rewardstr;
	std::string rewardstr1;
	std::string rewardinfo;
	std::string rewardrank;
	int matchscore;
	int awardindex;
	std::string awardindex1;
	std::map<std::string, std::string> map_myheros;
	std::map<std::string, std::string> map_pairheros;
	std::string pairplayerid;
	std::string pairnickname;
	std::map<std::string, NpcFriendly> map_pairfriendly;
	int pairscore;
	int wincount;
	int lostcount;
	int matchcount;
	int specifiedcount;
}MyMatchInfo;

typedef struct
{
	int rank;
	std::string playerid;
	std::string nickname;
	int matchscore;
	std::map<std::string, std::string> map_otherheros;
	std::string friendly;
	int wincount;
	int lostcount;
	int formation;
}MyRankData;

typedef struct
{
	int myrank;
	std::vector<MyRankData> vec_rankData;
}MyRankInfo;

typedef struct
{
	int logindays;
	bool isGeted;
	std::string rwds;
}LoginData;

typedef enum
{
	COSTCOIN_BUILDINGLVUP = 1,
	COSTCOIN_RANDHEROS,
	COSTCOIN_MARKETBUYRES,
	COSTCOIN_STRENTH,
	COSTCOIN_MAKERES,
	COSTCOIN_HILL,
	COSTCOIN_EVENTBUISNESS
}COSTCOIN_ANALYSIS;

typedef enum
{
	COSTPROP_TORCH = 0,//火把
	COSTPROP_VISION,//视野
	COSTPROP_GOHOME,//回城
	COSTPROP_COMPASS,//罗盘
	COSTPROP_MAPALLOPEN,//地图全开
	COSTPROP_TRANSER,//传送
	COSTPROP_GAMES,//赌博
	COSTPROP_END
}COSTPROPS_ANALYSIS;

typedef struct
{
	std::string rid;
	int buycount;
	int totalcount;
}TimeMartData;

typedef struct
{
	int sid;
	int needcount;
	int finishcount;
}S_StarData;

typedef struct
{
	std::vector<int> vec_starnum;
	std::vector<std::vector<std::string>> vec_adws;
	std::vector<int> vec_getstate;
}S_ChapterStarAwd;

typedef enum
{
	SA_FINISH_MAINTASK = 1,
	SA_FINISH_BRANCHTASK,
	SA_GETALLBOX,
	SA_FIGHTSUCC,
	SA_EVENT,
	SA_BUSIBUY,
	SA_ENTERMAZE,
	SA_NODEATH,
	SA_GOSTEP,
	SA_BEFRIEND,
	SA_BEMASTER,
	SA_BECOMPLE,
	SA_USEALLOPEN,
	SA_USETORCH
}STAR_TYPE;

typedef struct
{
	std::vector<std::string> vec_repairres;
	std::vector<std::string> vec_adws;
	std::string name;
	int state;//0--没有破损，1--破损提示，2--破损没有修复， 3--修复中
	int repairtime;
}S_BUILDINREPAIR;

typedef struct
{
	std::string id;
	std::vector<int> vec_formation;
	std::vector<float> vec_addattr;
	std::vector<float> vec_baseupattr;
	int lv;
}S_FORMATION;

typedef struct
{
	std::string playerid;
	std::string nickname;
	int weekcount;
	int lefttime;
}S_PAISEDATA;

typedef struct
{
	std::string mapid;
	std::vector<std::string> vec_rwdstr;
	std::vector<int> vec_rnd;
	int bnsexp;
	int costfood;
	int needherocount;
	int leftcount;
	int resetcount;
}S_MOPUPRWDDATA;

typedef struct
{
	int leftfreecount;
	int leftcoincount;
	std::string starendtime;
	std::string bossdata;
	std::string awdinfo;
	int bosslv;
	int bossmaxhp;
	float bossdodge;
	int bosslefthp;
	int starthour;
	int endhour;
	int curhurt;
	int totalhurt;
}S_SUPPERBOSSINFO;

typedef struct
{
	std::string playerid;
	std::string nickname;
	int hurt;
}S_SUPPERBOSSRANDATA;

typedef enum
{
	CH_NORMAL,
	CH_SUPERBOSS
}CHALLANGE_TYPE;

class GlobalInstance
{
public:
	GlobalInstance();
	~GlobalInstance();
	/****************************
	类实例，单例模式
	****************************/
	static GlobalInstance* getInstance();

	/****************************
	获取设备唯一ID
	*****************************/
	static std::string UUID();

	/****************************
	版本code
	*****************************/
	std::string getVersionCode();

	/****************************
	包名com.csfb.jh
	*****************************/
	std::string getPackageName();

	/****************************
	渠道号
	*****************************/
	std::string getChannelId();

	/****************************
	平台
	*****************************/
	std::string getPlatForm();

	/****************************
	idfa
	*****************************/
	std::string getIDFA();

	/****************************
	MD5
	*****************************/
	std::string getAppMD5Sign();

	int getSysSecTime();

	int createRandomNum(int val);
public:

	//加载初始化数据

	void loadInitData();
	//语言
	CC_SYNTHESIZE(int, m_lang, Lang);

	//保存英雄数据到本地
	void saveMyHeros();

	void saveHero(Hero* hero);

	//从本地读取英雄数据
	void loadMyHeros();

	//加载英雄属性数据
	void loadHerosAttrData();

	//保存随机生成的3个英雄数据到本地
	void saveRand3Heros();

	//从本地读取随机生成的3个英雄数据
	void loadRand3Heros();

	//保存刷新英雄时间
	void saveRefreshHeroTime(int time);

	//设置刷新英雄时间
	int getRefreshHeroTime();

	//检查是否重名
	bool checkifSameName(std::string nickname);


	//保存刷新资源时间
	void saveRefreshResTime(int time);

	//设置刷新资源时间
	int getRefreshResTime();

	//解析生产资源数据
	void loadResCreatorData();

	//保存生产资源数据
	void saveResCreatorData();

	//加载资源名称
	void loadAllResourcesData();

	//加载我的资源
	void loadMyResData();

	//加载装备资源
	void loadEquipData();

	//加载功法资源
	void loadGFData();

	//加载主线任务
	void loadTaskMainData();

	//加载已完成主线任务
	void loadMyTaskMainData();

	//保存已完成主线任务
	void saveMyTaskMainData();

	//加载支线任务
	void loadTaskBranchData();

	//加载已完成支线任务
	void loadMyTaskBranchData();

	//保存已完成支线任务
	void saveMyTaskBranchData();

	//总的工人数
	int getTotalFarmers();

	//总的工人数
	void saveTotalFarmers(int val);

	//正在工作的工人数
	int getWorkingFarmerCount();

	//解雇英雄
	void fireHero(int index);

	//英雄总的负重
	int getTotalCarry();

	//食物产出，工人采资源会影响食物产出
	int calcFoodMakeOut();

	void parseMapJson();

	//解析套装数据
	void parseSuitJson();

	//解析宝箱数据

	void parseTBoxJson();

	//加载事件数据
	void loadEventData();

	//加载成就数据
	void loadAchieveData();
	//读取成就数据
	void getMyAchieveData();
	void saveMyAchieveData();

	DynamicValueInt getMySoliverCount();

	void addMySoliverCount(DynamicValueInt val);

	void costMySoliverCount(DynamicValueInt val);

	DynamicValueInt getMyCoinCount();

	void addMyCoinCount(DynamicValueInt val);

	void costMyCoinCount(DynamicValueInt val);

	//根据装备品质生成镶嵌孔数
	int generateStoneCount(int qu);

	//保存刷新市场时间
	void saveRefreshMarketTime(int time);

	//设置刷新资源时间
	int getRefreshMarketTime();

	//加载NPC数据
	void loadNpcData();

	//根据客栈等级概率生成品质
	int generateHeroPotential();

	//根据客栈等级概率生成品质(元宝刷新概率)
	int generateHeroPotentialByCoin();

	//加载每日配置
	void loadDailyTaskData();
	//读取每日数据
	void getMyDailyTaskData();
	//保存已完成的每日任务数据
	void saveMyDailyTaskData();

	void loadShopData();

	//主线任务进行排序
	static bool larger_callback(TaskData a, TaskData b);
	//支线任务进行排序
	static bool larger_branchcallback(TaskData a, TaskData b);

	void heroSortByLv();

	static bool larger_heroLvCB(Hero* a, Hero* b);
	static bool sort_heroDeathCB(Hero* a, Hero* b);

	std::string getMyID();
	std::string getMyNickName();
	void setMyID(std::string val);
	void setMyNickName(std::string val);

	//生成资源ui/%s.png，字符串
	std::string getResUIFrameName(std::string resid, int qu);

	//设置章节
	int getUnlockChapter();
	void saveUnlockChapter(int val);

	//获取本地userdefault中保存的数据, para:0--需要清除的数据，1--传到服务端的数据
	std::string getUserDefaultXmlString(int type = 1);

	//清除本地userdefault中保存的数据,
	void cleanUserDefaultXmlData();

	//判断是否有死亡角色
	int getMyHerosDeadCount();

	//判断是否有达到10级角色
	int getHerosChangeLevelCount();

	//判断是否有达到某级的角色
	int getHerosLevelCount(int lv);

	//判断平均出战角色等级
	int getFightHerosLevel();

	//判断后山解锁条件
	bool getUnlockHomehillCondition();

	//判断任意角色体力小于某个值
	bool getLittleHerosPower(int comlv);

	//判断是否第一次出战六个角色
	bool getEnoughFightHeros();

	//判断可升级人参的数量
	bool getCanUpgradeCount(std::string gstr);

	/****************************
	银两刷新英雄次数
	****************************/
	void setSilverRefHeroCount(int val);

	/****************************
	获取银两刷新英雄次数
	****************************/
	int getSilverRefHeroCount();

	/****************************
	重置银两刷新英雄次数时间
	****************************/
	void setResetSilverRefHeroCountTime(int val);

	/****************************
	获取重置银两刷新英雄次数时间
	****************************/
	int getResetSilverRefHeroCountTime();

	/****************************
	恢复卡牌最大血量
	****************************/
	void recoveCardHeroMaxHp();

	/****************************
	读取npc数据
	****************************/	
	void loadNpcFriendData();

	/****************************
	读取npc好友
	****************************/
	void loadNpcFriendly();

	/****************************
	保存npc好友
	****************************/
	void saveNpcFriendly();

	/****************************
	读取每章节星级奖励
	****************************/
	void loadChapterStarAwds();

	/****************************
	装备武功比较是否有更高级
	****************************/

	bool compareHighEquip(int type, Hero* herodata);

	/****************************
	判断哪个更适合当前装备
	****************************/
	Equipable* compareFitEquip(int type, Hero* herodata);

	/****************************
	比较装备之间数值
	****************************/
	float compareOtherEquip(ResBase* m_res, Hero* herodata, int attr);

	/****************************
	是否有强化材料
	****************************/
	bool strengthMaterial(Equipable* m_res);

	/****************************
	议事厅任务红点显示
	****************************/
	bool checkNewQuest();

	std::vector<std::vector<std::string>> getMatchRewardByLv(int lv);

	int getMatchLvByScroe(int m_lv);

	/****************************
	竞技场解析匹配到的对手好友度
	****************************/
	void parsePairFriendly(std::string fstr);

	//判断是否有可升级建筑
	bool isCanUpgradeBuilding();

	//判断是否完成所有支线任务
	bool isAllFinishBranch();

	//重置数据
	void resetData();

	//upgrade
	void upgradeApp(std::string url);

	/****************************
	修改昵称次数是否到上限
	****************************/
	void setIsNichnameCountMax(bool val);

	/****************************
	修改昵称次数是否到上限
	****************************/
	bool getIsNichnameCountMax();

	/****************************
	获取建筑物等级
	****************************/
	void loadBuildingsLv();

	/****************************
	保存新手引导数据
	****************************/
	void saveNewerGuide(int index, bool val);

	void setPosCostCoin(int forwhere, int addval);

	void usePropsCount(int idindex, int useval);

	void setMazeEventData(int entercount, int useexitcount, int usecoin = 0);

	bool getIsGetRebateAwds(int index);

	void setIsGetRebateAwds(int index, bool val);

	void parseBuildingRepairData();

	void setBuildingBroken();

	void parseFormationData();

	void saveMyFormation(int mycurformation);

	float getFormationBns(int whichattr, int mycurformation, int lv);

	//扫荡数据
	void parseMopupData();

	void loadMopUpCountData();
	void saveMopupLeftData(std::string mapid);

	void resetMopupLeftData(std::string mapid);

private:
	static GlobalInstance* _Context;//类实例

	void saveHeroByIndex(int index);
public:
	static std::vector<Hero*> vec_myHeros;//招募的英雄
	static std::vector<Hero*> vec_rand3Heros;//待招募的3个英雄

	static std::vector<S_HeroAttr> vec_herosAttr;//英雄基础属性

	static std::vector<ResCreator*> vec_resCreators;//资源生产

	static std::map<std::string, AllResources> map_AllResources;//资源名字

	static std::map<std::string, EquipSuit> map_EquipSuit;//套装

	static std::map<std::string, EquipData> map_Equip;//装备名字

	static std::map<std::string, GFData> map_GF;

	static std::map<int, NPCAttrData> map_NpcAttrData;

	static std::map<std::string, NPCData> map_Npcs;

	static std::vector<TaskData> vec_TaskMain;//主线任务

	static TaskData myCurMainData;//当前主线任务

	static std::vector<TaskData> vec_TaskBranch;//支线任务

	static TaskData myCurBranchData;//当前支线任务

	static std::map<std::string, DailyTaskData> map_DTdata;//每日任务

	static int servertime;//服务器时间

	static Hero* myCardHeros[6];//出城选择的6个英雄

	static std::map<std::string, S_MainMap> map_mapsdata;//地图数据

	static std::map<std::string, TBoxData> map_TBoxs;//宝箱数据

	static std::vector<MessageData> vec_messsages;//邮箱数据

	static std::vector<MessageData> vec_notice;//消息数据

	static std::map<std::string, EventData> map_eventdata;//事件宝箱概率数据

	static std::vector<AchieveData> vec_achievedata;//成就

	static std::vector<ShopData> vec_shopdata;//商城
	static std::map<std::string, int> map_buyVipDays;//购买的月卡时间
	static std::vector<std::string> vec_buyVipIds;//月卡id


	static std::map<std::string, NpcRelationData> map_npcrelation;
	static std::map<std::string, NpcFriendly> map_myfriendly;
	static int npcmasterfinish;

	static int myOutMapCarry;

	static bool isNewHeroRefresh;

	static std::string qq;

	static std::string legalcopyurl;

	static bool isCheat;

	static S_TimeGift serverTimeGiftData;

	static MyMatchInfo myMatchInfo;

	static Hero* myOnChallengeHeros[6];//竞技赛我选择的6个英雄

	static Hero* matchPairHeros[6];//匹配到的6个英雄

	static MyRankInfo myRankInfo;

	static bool isServerReceipt;//IOS下用是否服务器验证支付结果

	static bool isResetData;//充值数据

	static bool isNotSameUUID;

	static std::string upgradeurl;

	static LoginData loginData;

	static int punishment;//0--不做处理；-1--清除所有数据；-2--清除部分数据

	static std::vector<int> vec_newerguides;

	static std::vector<int> vec_costCoins;

	static std::vector<int> vec_costprops;

	static bool isBuyFirstCharge;

	static bool isBuySpeedGift;

	static bool isBuyYearCard;

	static std::vector<int> vec_mazeroute;
	static int mazerouteindex;
	static std::string eventfrommapid;
	static int eventstartmappos;

	static int map_randstartpos;

	static std::vector<std::string> vec_news;

	static int getNewsTime;

	static int mazeEventData[3];

	static std::map<std::string, TimeMartData> map_timeMartData;

	static std::string timeMarketStr;
	static bool ishasmazeentry;
	static DynamicValueInt totalPayAmout;

	static bool isHasNewhero;
	static int showz002hinttextcount;

	static std::vector<S_StarData> vec_stardata;

	static std::vector<S_ChapterStarAwd> vec_chaperstarawds;

	static std::map<std::string, S_BUILDINREPAIR> map_buildingrepairdata;

	static std::map<std::string, S_FORMATION> map_formations;
	static int curMapFinishStars;

	static int takeoutherocount;
	static int starcontinuefightsucccount;

	static int curmapcontinuefightsucccount;

	static bool ispaycheck;

	static bool ispayconfirm;

	static S_PAISEDATA myPraisedata;

	static std::vector<S_PAISEDATA> vec_PaiseRankData;

	static DynamicValueInt myj002count;
	static std::string zanrwdinfo;

	static std::vector<S_PAISEDATA> vec_ToMyPaiseData;

	static int myTakeOnFormation;

	static int myMatchPairTakeOnFormation;

	static std::map<std::string, S_MOPUPRWDDATA> map_mopuprwds;

	static int fightwinbosscount;

	static bool isNewYearCard;

	static std::string androidurl;

	static S_SUPPERBOSSINFO supperbossinfo;

	static int challangeType;

	static std::vector<S_SUPPERBOSSRANDATA> vec_supperBossRankData;
	
	static std::string oneyuanGiftStr;

	static int isHasNewmail;

	static int isHasNewactivity;

	static std::string myinvitationcode;
	static std::string myinvitationrwd;

	static bool isupspeeding;

	static bool isopenpraise;
	
private:
	static int refreshHeroTime;
	static int refreshResTime;
	static int refreshMarketTime;
	static int totalFarmercount;
	static DynamicValueInt mySoliverCount;
	static DynamicValueInt myCoinCount;


	static std::string myID;
	static std::string myNickName;
	static int unlockchapter;

	static int silverRefHeroCount;
	static int resetSilverRefHeroCountTime;
	static std::vector<bool> vec_rebateisget;

};
#endif

