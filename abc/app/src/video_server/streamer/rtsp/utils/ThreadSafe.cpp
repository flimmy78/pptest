#include <nomad.h>
#include "ThreadSafe.h"

/** @brief ThreadSafe 생성자. 한개의 데이터를 여러개의 thread에서 사용할때 안전하게 관리 하기 위해 사용함.
	@return 의미 없음.
	@param 없음.
*/
ThreadSafe::ThreadSafe(void)
{
#ifdef WIN32
		InitializeCriticalSection(&csLock);
#else
		pthread_mutex_init(&mutex, NULL);
#endif

}

/** @brief ThreadSafe 소멸자.
	@return 의미 없음.
	@param 없음.
*/
ThreadSafe::~ThreadSafe(void)
{
#ifdef WIN32
		DeleteCriticalSection(&csLock);
#else
		pthread_mutex_destroy(&mutex);
#endif
}

/** @brief mutex lock을 건다.
	@return 의미 없음.
	@param 없음.
*/
void ThreadSafe::lock()
{
#ifdef WIN32
		EnterCriticalSection(&csLock);
#else
		pthread_mutex_lock(&mutex);
#endif
}

/** @brief mutex lock을 푼다.
	@return 의미 없음.
	@param 없음.
*/
void ThreadSafe::unlock()
{
#ifdef WIN32
		LeaveCriticalSection(&csLock);
#else
		pthread_mutex_unlock(&mutex);
#endif
}
