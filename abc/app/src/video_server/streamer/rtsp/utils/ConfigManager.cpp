#include <nomad.h>
#include "ConfigManager.h"
#include <sys/stat.h>
#include "StringUtil.h"
#include "gwport.h"

/** @brief ConfigManager ������.
	@return �ǹ� ����.
	@param ����.
*/
ConfigManager::ConfigManager(void)
{
}

/** @brief ConfigManager �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
ConfigManager::~ConfigManager(void)
{
	clear();
}

/** @brief ������ ���δ����� �о ':'�� �����ڷ� �Ͽ� key�� value���� �����Ѵ�.
	@return ������ �о� key, value������ ������ ����� �Ǹ� S_OK, ������ ���ų� ���� ũ�Ⱑ 0�̸� E_FAIL;
	@param szPathName ���� ������ ���� ���
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

/** @brief ����� key, value ���� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
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

/** @brief �־��� key�� ������ value ���� return �Ѵ�.
	@return ��û�� key�� ���� value
	@param szKey ������ value�� key
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
