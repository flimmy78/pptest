#ifdef LINUX
#include <string.h>
#endif
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/** @brief printf등으로 나타나지 않는 문자(white space)인지 확인한다.
	@return white space 이면 true, 아니면 false
	@param ch white space인지 판단해야 할 문자
*/
bool isWhitespace(const char ch)
{
	const char whitespace[] = { ' ', '\t', '\r', '\n' };
	const int size = sizeof(whitespace) / sizeof(whitespace[0]);

	for(int i=0; i<size; i++)
	{
		if(ch == whitespace[i])
		{
			return true;
		}
	}

	return false;
}

/** @brief 몇번째 문자가 white space가 아닌지를 알아온다.(앞에서 부터)
	@return white space인 char의 index
	@param text 몇번째 문자가 white space가 아닌지 찾아야 할 문자열
*/
int ltrimIndex(const string &text)
{
	int i;
	int length = text.size();

	for(i=0; i<length; i++)
	{
		if( !isWhitespace(text[i]) )
		{
			break;
		}
	}

	return i;
}

/** @brief 몇번째 문자가 white space가 아닌지를 알아온다.(뒤에서 부터)
	@return white space인 char의 index
	@param text 몇번째 문자가 white space가 아닌지 찾아야 할 문자열
*/
int rtrimIndex(const string &text)
{
	int i;
	int length = text.size();

	for(i=length-1; i>=0; i--)
	{
		if( !isWhitespace(text[i]) )
		{
			break;
		}
	}

	return i + 1;
}

/** @brief 문자열 앞의 white space를 제거한다.
	@return 문자열 앞의 white space가 제거된 문자열
	@param text 문자열 앞의 white space를 제거할 문자열
*/
string ltrim(const string &text)
{
	int begin = ltrimIndex(text);
	return text.substr(begin);
}

/** @brief 문자열 뒤의 white space를 제거한다.
	@return 문자열 뒤의 white space가 제거된 문자열
	@param text 문자열 뒤의 white space를 제거할 문자열
*/
string rtrim(const string &text)
{
	int end = rtrimIndex(text);
	return text.substr(0, end);
}

/** @brief 문자열 앞, 뒤의 white space를 제거한다.
	@return 문자열 앞, 뒤의 white space가 제거된 문자열
	@param text 문자열 앞, 뒤의 white space를 제거할 문자열
*/
string trim(const string &text)
{
	int begin = ltrimIndex(text);
	int end = rtrimIndex(text);

	return text.substr(begin, end);
}

/** @brief 문자열을 특장 char로 분리하여 나눈다.
	@return 없음.
	@param text 원본 문자열
	@param sep 분리할 char
	@param tokens 분리된 문자열이 들어갈 vector
*/
void split(const std::string &text, const char sep, std::vector<std::string> &tokens)
{
	int start = 0, end = 0;

	while ((end = text.find(sep, start)) != (int)(string::npos))
	{
		if(end != start)
		{
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + 1;
	}

	tokens.push_back(trim(text.substr(start)));
}

/** @brief 문자열을 특장 string로 분리하여 나눈다.
	@return 없음.
	@param text 원본 문자열
	@param sep 분리할 string
	@param tokens 분리된 문자열이 들어갈 vector
*/
void split(const std::string &text, std::string sep, std::vector<std::string> &tokens)
{
	int start = 0, end = 0;

	while ((end = text.find(sep, start)) != (int)(string::npos))
	{
		if(end != start)
		{
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + sep.size();
	}

	tokens.push_back(trim(text.substr(start)));
}

/** @brief 문자열을 특정 구분자로 나누어 key 와 value값으로 만든다.
	@return 정상적으로 나누어 져서 key, value값이 추출 되면 0, 나머지 경우는 -1
	@param szLine key와 value로 나눌 문자열
	@param szSeparator key와 value를 구분할 구분자
	@param pszKey 구분되어 들어갈 key
	@param pszValue 구분되어 들어갈 value
*/
int parseKeyValueLine(
	char* szLine, const char* szSeparator, char** pszKey, char** pszValue)
{
	char* szText;

	if(szLine[0] == '#')
	{
		return -1;
	}

	szText = strchr(szLine, szSeparator[0]);
	if(szText == NULL)
	{
		return -1;
	}

	szText[0] = '\0';
	*pszKey = szLine;

	szText++;

	while(*szText == ' ' || *szText == '\t')
	{
		szText++;
	}

	*pszValue = szText;

	return 0;
}

/** @brief hex binary를 string(대문자)으로 변환한다.
	@return 변환한 string의 길이
	@param pBuf 변경할 hex binary
	@param nDataSize 변환한 string의 길이
	@param szHex 변환한 string이 들어감.
	@param nBufSize 변경할 hex binary의 길이
*/
int hexToString(
	const unsigned char* pBuf, int nDataSize, char* szHex, int nBufSize)
{
    int i;
    int j;
    int nCount = nBufSize / 2;
   
    if(nDataSize < nCount)
    {  
        return -1;
    }
   
    const char octet[] =
    {  
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };
   
    for(i=0, j=0; i<nCount; i++, j+=2)
    {  
        szHex[j]   = octet[ (pBuf[i] & 0xF0) >> 4 ];
        szHex[j+1] = octet[ pBuf[i] & 0x0F ];
    }
   
    return j;
}

/**
 * Hex binary -> lower case string.
 * @return string's length
 */

/** @brief hex binary를 string(소문자)으로 변환한다.
	@return 변환한 string의 길이
	@param pBuf 변경할 hex binary
	@param nDataSize 변환한 string의 길이
	@param szHex 변환한 string이 들어감.
	@param nBufSize 변경할 hex binary의 길이
*/
int hexToLowerCaseString(
	const unsigned char* pBuf, int nDataSize, char* szHex, int nBufSize)
{
	int i;
	int j;
	int nCount = nBufSize / 2;
   
	if(nDataSize < nCount)
	{  
		return -1;
	}
   
	const char octet[] =
	{  
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f'
	};
   
	for(i=0, j=0; i<nCount; i++, j+=2)
	{  
		szHex[j]   = octet[ (pBuf[i] & 0xF0) >> 4 ];
		szHex[j+1] = octet[ pBuf[i] & 0x0F ];
	}

	return j;
}

/** @brief hex char를 hex binary로 변환한다.
	@return 변환한 hex binary
	@param chHex 변환할 hex char
*/
unsigned char GetNibble(char chHex)
{
	unsigned char byteNibble = 0;

	if(chHex >= '0' && chHex <= '9')
	{
		byteNibble = chHex - '0';
	}
	else if(chHex >= 'A' && chHex <= 'F')
	{
		byteNibble = chHex - 'A' + 10;
	}
	else if(chHex >= 'a' && chHex <= 'f')
	{
		byteNibble = chHex - 'a' + 10;
	}

	return byteNibble;
}

/**
 * Little endian only
 */

/** @brief hex string을 hex binary로 변환한다.
	@return 변환한 string의 길이
	@param szHex 변경할 hex string
	@param nBufSize 변환한 hex binary의 길이
	@param pBuf 변환한 hex binary가 들어감
	@param nDataSize 변경할 hex string의 길이
*/
int stringToHex(
	const char* szHex, int nBufSize, unsigned char* pBuf, int nDataSize)
{
	int i;
	int j;
	int nIndex;
	int nCount = nBufSize / 2;

	if(nDataSize < nCount)
	{
		return -1;
	}

	unsigned char uchNibble;

	j = 0;
	nIndex = nBufSize - 1;
	memset(pBuf, 0, nDataSize);

    for(i=0; i<nBufSize; i++, nIndex--)
    {
		uchNibble = GetNibble(szHex[nIndex]);

		if(i % 2)
		{
			pBuf[j] |= uchNibble << 4;
			j++;
		}
		else
		{
			pBuf[j] = uchNibble;
		}
    }

	return j;
}

/** @brief hex string을 int로 변환한다.
	@return 변경된 값
	@param szHex 변경할 hex string
*/
unsigned int sexStringToInt(const char* szHex)
{
	unsigned int nValue = 0;

	for(int i=0; szHex[i] != '\0'; i++)
	{
		nValue <<= 4;
		nValue += GetNibble( szHex[i] );
	}

	return nValue;
}

/** @brief string을 int로 변환한다.
	@return 변경된 값
	@param szText 변경할 string
	@param nLength 변경할 string의 길이
*/
int stringToInt(const char* szText, const int nLength)
{
	int nValue = 0;

	for(int i=0; i<nLength; i++)
	{
		const char ch = szText[i];

		if(ch < '0' && ch > '9')
		{
			return 0;
		}

		nValue *= 10;
		nValue += ch - '0';
	}

	return nValue;
}

/** @brief int를 string으로 변환한다.
	@return 변경된 값
	@param i 변경할 int
*/
std::string intToString(int i)
{	
	char *intChar;
	intChar = (char*)malloc(sizeof(char)*33);
	memset(intChar, 0, 33);
	
	sprintf(intChar, "%d", i);
	std::string tmpStr(intChar);
	if (intChar) free(intChar);
	return tmpStr;
}

/** @brief 문자열이 특정 문자열로 끝나는지를 확인한다.
	@return 특정 문자열로 끝날경우 true, 아닐 경우 false
	@param source 확인할 문자열
	@param target 비교할 특정 문자열
*/
bool endsWith(string source, string target)
{
	if(source.size() >= target.size())
	{
		string _tSrc = source.substr(source.size() - target.size());
		if (_tSrc == target) return true;
		else return false;
	}
	else return false;
}

/** @brief 문자열이 특정 문자열로 시작하는지 확인한다.
	@return 특정 문자열로 시작할 경우 true, 아닐 경우 false
	@param source 확인할 문자열
	@param target 비교할 특정 문자열
*/
bool startsWith(string source, string target)
{
	if(source.size() >= target.size())
	{
		string _tSrc = source.substr(0, target.size());
		if (_tSrc == target) return true;
		else return false;
	}
	else return false;
}

