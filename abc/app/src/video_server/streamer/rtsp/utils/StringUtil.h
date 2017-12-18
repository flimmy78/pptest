#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <string>
#include <vector>

bool isWhitespace(const char ch);
int ltrimIndex(const std::string &text);
int rtrimIndex(const std::string &text);
std::string ltrim(const std::string &text);
std::string rtrim(const std::string &text);
std::string trim(const std::string &text);
void split(const std::string &text, const char sep, std::vector<std::string> &tokens);
void split(const std::string &text, std::string sep, std::vector<std::string> &tokens);

int parseKeyValueLine(
	char* szLine, const char* szSeparator, char** pszKey, char** pszValue);

int hexToString(
	const unsigned char* pBuf, int nDataSize, char* szHex, int nBufSize);
int hexToLowerCaseString(
	const unsigned char* pBuf, int nDataSize, char* szHex, int nBufSize);
int stringToHex(
	const char* szHex, int nBufSize, unsigned char* pBuf, int nDataSize);
unsigned int sexStringToInt(const char* szHex);
int stringToInt(const char* szText, const int nLength);

std::string intToString(int i);
bool endsWith(std::string source, std::string target);
bool startsWith(std::string source, std::string target);

#endif
