#include "RefCount.h"
#include <gw.h>

/** @brief RefCount의 생성자. refcount를 증가및 감소시키다가 0이 되면 메모리를 해제한다.
	@return 없음.
	@param 없음.
*/
RefCount::RefCount() :
count(0)
{
}

/** @brief RefCount의 소멸자
	@return 없음.
	@param 없음.
*/
RefCount::~RefCount()
{
}

/** @brief 참조 숫자를 하나 증가시킨다.
	@return 중가후 참조 숫자
	@param 없음.
*/
int RefCount::addRef()
{
	count++;
	return count;
}

/** @brief 현재 참조 숫자를 가져온다.
	@return 현재 참조 숫자
	@param 없음.
*/
int RefCount::getRef() const
{
	return count;
}

/** @brief 현재 참조 하나 감소시킨다.
	@return 감소후 참조 숫자
	@param 없음.
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

/** @brief 참조 갯수를 설정한다.
	@return 없음.
	@param refCount 설정할 참조 갯수
*/
void RefCount::setRef(const int refCount)
{
	count = refCount;
}