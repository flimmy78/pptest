#ifdef LINUX
#include <string.h>
#endif
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/** @brief printf������ ��Ÿ���� �ʴ� ����(white space)���� Ȯ���Ѵ�.
	@return white space �̸� true, �ƴϸ� false
	@param ch white space���� �Ǵ��ؾ� �� ����
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

/** @brief ���° ���ڰ� white space�� �ƴ����� �˾ƿ´�.(�տ��� ����)
	@return white space�� char�� index
	@param text ���° ���ڰ� white space�� �ƴ��� ã�ƾ� �� ���ڿ�
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

/** @brief ���° ���ڰ� white space�� �ƴ����� �˾ƿ´�.(�ڿ��� ����)
	@return white space�� char�� index
	@param text ���° ���ڰ� white space�� �ƴ��� ã�ƾ� �� ���ڿ�
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

/** @brief ���ڿ� ���� white space�� �����Ѵ�.
	@return ���ڿ� ���� white space�� ���ŵ� ���ڿ�
	@param text ���ڿ� ���� white space�� ������ ���ڿ�
*/
string ltrim(const string &text)
{
	int begin = ltrimIndex(text);
	return text.substr(begin);
}

/** @brief ���ڿ� ���� white space�� �����Ѵ�.
	@return ���ڿ� ���� white space�� ���ŵ� ���ڿ�
	@param text ���ڿ� ���� white space�� ������ ���ڿ�
*/
string rtrim(const string &text)
{
	int end = rtrimIndex(text);
	return text.substr(0, end);
}

/** @brief ���ڿ� ��, ���� white space�� �����Ѵ�.
	@return ���ڿ� ��, ���� white space�� ���ŵ� ���ڿ�
	@param text ���ڿ� ��, ���� white space�� ������ ���ڿ�
*/
string trim(const string &text)
{
	int begin = ltrimIndex(text);
	int end = rtrimIndex(text);

	return text.substr(begin, end);
}

/** @brief ���ڿ��� Ư�� char�� �и��Ͽ� ������.
	@return ����.
	@param text ���� ���ڿ�
	@param sep �и��� char
	@param tokens �и��� ���ڿ��� �� vector
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

/** @brief ���ڿ��� Ư�� string�� �и��Ͽ� ������.
	@return ����.
	@param text ���� ���ڿ�
	@param sep �и��� string
	@param tokens �и��� ���ڿ��� �� vector
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

/** @brief ���ڿ��� Ư�� �����ڷ� ������ key �� value������ �����.
	@return ���������� ������ ���� key, value���� ���� �Ǹ� 0, ������ ���� -1
	@param szLine key�� value�� ���� ���ڿ�
	@param szSeparator key�� value�� ������ ������
	@param pszKey ���еǾ� �� key
	@param pszValue ���еǾ� �� value
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

/** @brief hex binary�� string(�빮��)���� ��ȯ�Ѵ�.
	@return ��ȯ�� string�� ����
	@param pBuf ������ hex binary
	@param nDataSize ��ȯ�� string�� ����
	@param szHex ��ȯ�� string�� ��.
	@param nBufSize ������ hex binary�� ����
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

/** @brief hex binary�� string(�ҹ���)���� ��ȯ�Ѵ�.
	@return ��ȯ�� string�� ����
	@param pBuf ������ hex binary
	@param nDataSize ��ȯ�� string�� ����
	@param szHex ��ȯ�� string�� ��.
	@param nBufSize ������ hex binary�� ����
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

/** @brief hex char�� hex binary�� ��ȯ�Ѵ�.
	@return ��ȯ�� hex binary
	@param chHex ��ȯ�� hex char
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

/** @brief hex string�� hex binary�� ��ȯ�Ѵ�.
	@return ��ȯ�� string�� ����
	@param szHex ������ hex string
	@param nBufSize ��ȯ�� hex binary�� ����
	@param pBuf ��ȯ�� hex binary�� ��
	@param nDataSize ������ hex string�� ����
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

/** @brief hex string�� int�� ��ȯ�Ѵ�.
	@return ����� ��
	@param szHex ������ hex string
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

/** @brief string�� int�� ��ȯ�Ѵ�.
	@return ����� ��
	@param szText ������ string
	@param nLength ������ string�� ����
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

/** @brief int�� string���� ��ȯ�Ѵ�.
	@return ����� ��
	@param i ������ int
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

/** @brief ���ڿ��� Ư�� ���ڿ��� ���������� Ȯ���Ѵ�.
	@return Ư�� ���ڿ��� ������� true, �ƴ� ��� false
	@param source Ȯ���� ���ڿ�
	@param target ���� Ư�� ���ڿ�
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

/** @brief ���ڿ��� Ư�� ���ڿ��� �����ϴ��� Ȯ���Ѵ�.
	@return Ư�� ���ڿ��� ������ ��� true, �ƴ� ��� false
	@param source Ȯ���� ���ڿ�
	@param target ���� Ư�� ���ڿ�
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

