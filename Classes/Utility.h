﻿
#ifndef _H_UTILITY_H_
#define _H_UTILITY_H_

#include "cocos2d.h"
#include "DataBase64.h"
#include <string>

using namespace std;

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define encrypt_data 1
#endif

// 加/解密函数；bEncrypt = true 加密；bEncrypt = false 解密
inline char* Encrypt(char* pMsg, bool bEncrypt)
{
	int i, j, nMsgLen, nKeyLen;

	char szKey1[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.', ',', ';', '*', '#', '?', ':', '=', '+' };

	char szKey2[] = { 'd', 'A', 'e', 'Q', 'i', 'R', 'I', 'j', '8', 'l', 'M', '6', '-', 'n', 'B', 'G', 'H', 'V', '3', 'v', 'o', 'w', 'x', 'J', '0', 'U',
		'c', 'E', 'S', '9', 'X', 'g', 'T', 'p', 'C', 'D', 'f', 'h', 'N', 'z', 'q', 's', '4', 't', '5', 'm', 'y', 'K', 'L', '1', 'O', 'u',
		'Y', '2', 'P', 'Z', 'a', 'r', 'F', 'W', 'b', '.', 'k', '7', '#', '*', '?', ':', ',', ';', '+', '='};

	if (NULL == pMsg)
	{
		return NULL;
	}

	nMsgLen = strlen(pMsg);
	nKeyLen = strlen(szKey1);

	for (i = 0; i < nMsgLen; i++)
	{
		if (bEncrypt)
		{
			for (j = 0; j < nKeyLen; j++)
			{
				if (pMsg[i] == szKey1[j])
				{
					pMsg[i] = szKey2[j]; break;
				}
			}
		}
		else
		{
			for (j = 0; j < nKeyLen; j++)
			{
				if (pMsg[i] == szKey2[j])
				{
					pMsg[i] = szKey1[j]; break;
				}
			}
		}

	}
	return pMsg;
}

inline std::string loadStringDataByKey(string key, std::string defaultValue = ""){
	char buffer[32];
	sprintf(buffer, "jh%s", key.c_str());

	string s = UserDefault::getInstance()->getStringForKey(buffer);
	if (s == "") {
		return defaultValue;
	}
#if encrypt_data
	string valuestr = s;
	Encrypt((char*)valuestr.c_str(), false);

	string parseKey = parseData(valuestr);
	return parseKey;
#else
	return s;
#endif
}

inline void saveStringDataByKey(string key, string sValue = ""){ 
	char keybuffer[32];
	sprintf(keybuffer, "jh%s", key.c_str());
#if encrypt_data
	string encryptvalue = encryptData(reinterpret_cast<const unsigned char*>(sValue.c_str()), sValue.length());
	string valuestr = encryptvalue;
	Encrypt((char*)valuestr.c_str(), true);
	encryptvalue = valuestr;
#else
	string encryptvalue = sValue;
#endif
	UserDefault::getInstance()->setStringForKey(keybuffer, encryptvalue);
	UserDefault::getInstance()->flush();
}

inline void saveIntDataByKey(string key, int nVal){
	char buffer[32];
	sprintf(buffer, "%d", nVal);
	saveStringDataByKey(key, buffer);
}

inline void saveFloatDataByKey(string key, float nVal){
	char buffer[32];
	sprintf(buffer, "%.2f", nVal);
	saveStringDataByKey(key, buffer);
}

inline int loadIntDataByKey(string key, int defaultValue = 0){
	char buffer[32];
	sprintf(buffer, "%s", key.c_str());

	string s = loadStringDataByKey(buffer);
	if(s == "") {
		return defaultValue;
	}
	return atoi(s.c_str());
}

inline float loadFloatDataByKey(string key, float defaultValue = 0.0f){
	char buffer[32];
	sprintf(buffer, "%s", key.c_str());

	string s = loadStringDataByKey(buffer);
	if (s == "") {
		return defaultValue;
	}
	return atof(s.c_str());
}

inline void removeEleByKey(string key)
{
	char keybuffer[32];
	sprintf(keybuffer, "jh%s", key.c_str());
	UserDefault::getInstance()->deleteValueForKey(keybuffer);
	UserDefault::getInstance()->flush();
}

#endif // _H_UTILITY_H_
