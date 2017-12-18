#ifndef __CONFIG_MANAGER_H__
#define __CONFIG_MANAGER_H__

#include <map>
#include <string>

class ConfigManager
{
public:
	ConfigManager(void);
	virtual ~ConfigManager(void);

	HRESULT load(const char* szPathName);
	void clear();

	const char* getValue(const char* szKey);
#if 0
	bool contains(const char* szKey);
#endif

private:
	typedef std::map<std::string, std::string> configMap;
	std::map<std::string, std::string> configs;
};

#endif