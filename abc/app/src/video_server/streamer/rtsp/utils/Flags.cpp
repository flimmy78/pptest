#include "nomad.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "Flags.h"

/** @brief Flags 생성자
	@return 의미없음.
	@param 없음.
*/
Flags::Flags(void)
{
	flags = 0u;
}

/** @brief Flags 생성자
	@return 의미없음.
	@param uFlags 사용할 uFlags 지정
*/
Flags::Flags(UINT uFlags)
{
	flags = uFlags;
}

/** @brief Flags 소멸자
	@return 의미없음.
	@param 없음.
*/
Flags::~Flags(void)
{
}

/** @brief uflags를 켜거나 끈다.
	@return 의미없음.
	@param uFlags 켜거나 끌 flags
	@param bValue true 켜기, false 끄기
*/
void Flags::setFlags(UINT uFlags, bool bValue)
{
	if(bValue)
	{
		flags |= uFlags;
	}
	else
	{
		flags &= (uFlags ^ 0xFFFFFFFFu);
	}
}

/** @brief flags를 설정한다.
	@return 의미없음.
	@param uFlags 설정할 flags
*/
void Flags::setFlags(UINT uFlags)
{
	flags = uFlags;
}

/** @brief 현재 설정된 flags 값을 가져온다.
	@return 설정된 flags
	@param uMask 가져올 위치의 flags의 값만 1로 만든 mask
*/
UINT Flags::getFlags(UINT uMask)
{
	return flags & uMask;
}

void Flags::toggleFlags(UINT uFlags)
{
	UINT uValue = flags & uFlags;
	setFlags(uFlags, !uValue);
}
