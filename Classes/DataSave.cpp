﻿#include "DataSave.h"
#include "cocos2d.h"
#include "MD5.h"
#include "Utility.h"

DataSave* DataSave::_Context = NULL;

DataSave* DataSave::getInstance() {
	if (_Context == NULL) {
		_Context = new DataSave;
	}
	return _Context;
}

void DataSave::purgeGameSave() {
	CC_SAFE_DELETE(_Context);
}

int DataSave::getBuildLv(std::string name)
{
	return loadIntDataByKey(name, 0);
}

void DataSave::setBuildLv(std::string name, int val)
{
	saveIntDataByKey(name, val);
}

int DataSave::getLocalLang()
{
	return loadIntDataByKey("lang", 1);
}

void DataSave::setLocalLang(int val)
{
	saveIntDataByKey("lang", val);
}

std::string DataSave::getHeroData(std::string herokey)
{
	return loadStringDataByKey(herokey, "");
}

void DataSave::setHeroData(std::string herokey, std::string herodata)
{
	saveStringDataByKey(herokey, herodata);
}

std::string DataSave::getRand3HeroData()
{
	return loadStringDataByKey("randheros", "");
}

void DataSave::setRand3HeroData(std::string herodata)
{
	saveStringDataByKey("randheros", herodata);
}

int DataSave::getRefreshHeroTime()
{
	return loadIntDataByKey("refreshherotime", 0);
}

void DataSave::setRefreshHeroTime(int time)
{
	saveIntDataByKey("refreshherotime", time);
}

int DataSave::getRefreshResTime()
{
	return loadIntDataByKey("refreshrestime", 0);
}

void DataSave::setRefreshResTime(int time)
{
	saveIntDataByKey("refreshrestime", time);
}

std::string DataSave::getResCreatorData()
{
	return loadStringDataByKey("rescreator", "0-0");
}

void DataSave::setResCreatorData(std::string val)
{
	saveStringDataByKey("rescreator", val);
}

int DataSave::getFarmersCount()
{
	return loadIntDataByKey("creatorfarmers", 0);
}

void DataSave::setFarmersCount(int count)
{
	saveIntDataByKey("creatorfarmers", count);
}

void DataSave::setMyRes(std::string strval)
{
	saveStringDataByKey("myres", strval);
}

std::string DataSave::getMyRes()
{
	return loadStringDataByKey("myres", "r002-100-1");
}

int DataSave::getTotalFarmers()
{
	return loadIntDataByKey("totalfarmers", 5);
}

void DataSave::setTotalFarmers(int val)
{
	saveIntDataByKey("totalfarmers", val);
}

int DataSave::getBuildingLv(std::string buildingname)
{
	return loadIntDataByKey(buildingname, 0);
}

void DataSave::setBuildingLv(std::string buildingname, int val)
{
	saveIntDataByKey(buildingname, val);
}