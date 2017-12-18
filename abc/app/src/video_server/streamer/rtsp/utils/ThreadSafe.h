#pragma once
#ifndef __THREADSAFE_H__
#define __THREADSAFE_H__

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

class ThreadSafe
{
protected:
#ifdef WIN32
	CRITICAL_SECTION csLock;
#else
	pthread_mutex_t mutex;
#endif

public:
	ThreadSafe(void);
	virtual ~ThreadSafe(void);

	void lock();
	void unlock();

#ifndef WIN32
	operator pthread_mutex_t*() { return &mutex; }
#endif
};

class AutoLock
{
public:
	AutoLock(ThreadSafe* pMutex) : mutex(pMutex) { pMutex->lock(); };
	~AutoLock(void) { mutex->unlock(); };

private:
	ThreadSafe* mutex;
};

#endif
