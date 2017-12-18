#include "RefCount.h"
#include <gw.h>

/** @brief RefCount�� ������. refcount�� ������ ���ҽ�Ű�ٰ� 0�� �Ǹ� �޸𸮸� �����Ѵ�.
	@return ����.
	@param ����.
*/
RefCount::RefCount() :
count(0)
{
}

/** @brief RefCount�� �Ҹ���
	@return ����.
	@param ����.
*/
RefCount::~RefCount()
{
}

/** @brief ���� ���ڸ� �ϳ� ������Ų��.
	@return �߰��� ���� ����
	@param ����.
*/
int RefCount::addRef()
{
	count++;
	return count;
}

/** @brief ���� ���� ���ڸ� �����´�.
	@return ���� ���� ����
	@param ����.
*/
int RefCount::getRef() const
{
	return count;
}

/** @brief ���� ���� �ϳ� ���ҽ�Ų��.
	@return ������ ���� ����
	@param ����.
*/
int RefCount::release()
{
	count--;
	
	if(count < 0)
	{
		count = 0;
	}

	return count;
}

/** @brief ���� ������ �����Ѵ�.
	@return ����.
	@param refCount ������ ���� ����
*/
void RefCount::setRef(const int refCount)
{
	count = refCount;
}