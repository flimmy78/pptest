#include "nomad.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "Flags.h"

/** @brief Flags ������
	@return �ǹ̾���.
	@param ����.
*/
Flags::Flags(void)
{
	flags = 0u;
}

/** @brief Flags ������
	@return �ǹ̾���.
	@param uFlags ����� uFlags ����
*/
Flags::Flags(UINT uFlags)
{
	flags = uFlags;
}

/** @brief Flags �Ҹ���
	@return �ǹ̾���.
	@param ����.
*/
Flags::~Flags(void)
{
}

/** @brief uflags�� �Ѱų� ����.
	@return �ǹ̾���.
	@param uFlags �Ѱų� �� flags
	@param bValue true �ѱ�, false ����
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

/** @brief flags�� �����Ѵ�.
	@return �ǹ̾���.
	@param uFlags ������ flags
*/
void Flags::setFlags(UINT uFlags)
{
	flags = uFlags;
}

/** @brief ���� ������ flags ���� �����´�.
	@return ������ flags
	@param uMask ������ ��ġ�� flags�� ���� 1�� ���� mask
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
