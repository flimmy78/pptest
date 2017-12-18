#include <nomad.h>
#include "ConfigManager.h"
#include <sys/stat.h>
#include "StringUtil.h"
#include "gwport.h"

/** @brief ConfigManager 생성자.
	@return 의미 없음.
	@param 없음.
*/
ConfigManager::ConfigManager(void)
{
}

/** @brief ConfigManager 소멸자.
	@return 의미 없음.
	@param 없음.
*/
ConfigManager::~ConfigManager(void)
{
	clear();
}

/** @brief 파일을 라인단위로 읽어서 ':'를 구분자로 하여 key와 value값을 저장한다.
	@return 파일을 읽어 key, value값으로 저장이 제대로 되면 S_OK, 파일이 없거나 파일 크기가 0이면 E_FAIL;
	@param szPathName 읽을 파일의 절대 경로
*/
HRESULT ConfigManager::load(const char* szPathName)
{
	HRESULT hr;

	FILE* fp;
	//int nCount;
	int nRet;
	int nFileSize = 0;
	char* pBuf;

	char* szKey;
	char* szValue;
	char* szLine;
	char* szNext;
	const char* szSeparator = "\n";

	struct stat fileSt;
	nRet = stat(szPathName, &fileSt);

	if(nRet < 0)
	{
		return E_FAIL;
	}

	nFileSize = fileSt.st_size;
	if(nFileSize == 0)
	{
		return E_FAIL;
	}

	fp = fopen(szPathName, "rb");
	if(fp == NULL)
	{
		return E_FAIL;
	}

	pBuf = new char [nFileSize + 1];
	pBuf[nFileSize] = '\0';

	fread(pBuf, 1, nFileSize, fp);

	szLine = ::gw_strtok_r(pBuf, szSeparator, &szNext);

	clear();

	while(szLine)
	{
		hr = parseKeyValueLine(szLine, ":", &szKey, &szValue);

		if(SUCCEEDED(hr))
		{
			configs.insert( std::pair<std::string, std::string> (szKey, szValue) );
		}

		szLine = ::gw_strtok_r(NULL, szSeparator, &szNext);
	}

	if(pBuf)
	{
		delete[] pBuf;
	}

	if(fp)
	{
		fclose(fp);
	}

	return S_OK;
}

/** @brief 저장된 key, value 값을 삭제한다.
	@return 의미 없음.
	@param 없음.
*/
void ConfigManager::clear()
{
	configs.clear();
}

#if 0
BOOL ConfigManager::contains(const char* szKey)
{
	ConfigMap::const_iterator itr = m_Configs.find(szKey);
	if(itr == m_Configs.end())
	{
		return FALSE;
	}

	return TRUE;
}
#endif

/** @brief 주어진 key를 가지고 value 값을 return 한다.
	@return 요청한 key에 대한 value
	@param szKey 리턴할 value의 key
*/
const char* ConfigManager::getValue(const char* szKey)
{
	if(szKey == NULL)
	{
		return NULL;
	}

	configMap::const_iterator itr = configs.find(szKey);
	if(itr == configs.end())
	{
		return NULL;
	}

	return itr->second.c_str();
}
