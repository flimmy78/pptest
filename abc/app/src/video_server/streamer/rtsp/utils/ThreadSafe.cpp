#include <nomad.h>
#include "ThreadSafe.h"

/** @brief ThreadSafe ������. �Ѱ��� �����͸� �������� thread���� ����Ҷ� �����ϰ� ���� �ϱ� ���� �����.
	@return �ǹ� ����.
	@param ����.
*/
ThreadSafe::ThreadSafe(void)
{
#ifdef WIN32
		InitializeCriticalSection(&csLock);
#else
		pthread_mutex_init(&mutex, NULL);
#endif

}

/** @brief ThreadSafe �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
ThreadSafe::~ThreadSafe(void)
{
#ifdef WIN32
		DeleteCriticalSection(&csLock);
#else
		pthread_mutex_destroy(&mutex);
#endif
}

/** @brief mutex lock�� �Ǵ�.
	@return �ǹ� ����.
	@param ����.
*/
void ThreadSafe::lock()
{
#ifdef WIN32
		EnterCriticalSection(&csLock);
#else
		pthread_mutex_lock(&mutex);
#endif
}

/** @brief mutex lock�� Ǭ��.
	@return �ǹ� ����.
	@param ����.
*/
void ThreadSafe::unlock()
{
#ifdef WIN32
		LeaveCriticalSection(&csLock);
#else
		pthread_mutex_unlock(&mutex);
#endif
}
