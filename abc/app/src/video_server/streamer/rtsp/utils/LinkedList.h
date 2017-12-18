#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "nomad_windef.h"

typedef struct _SNode
{
	void* pObject;

	struct _SNode* pPrev;
	struct _SNode* pNext;
} SNode;

typedef void (*pfnDeleteObject)(void* pObject);

class ThreadSafe;

class LinkedList
{
protected:
	BOOL isThreadSafe;
	ThreadSafe* threadSafe;

	int size;
	SNode* head;
	SNode* tail;

	pfnDeleteObject DeleteObject;

	// For enumeration
	SNode* node;
	BOOL reverse;

protected:
	HRESULT getNode(const void* pObject, int* pIndex, SNode** ppNode);
	HRESULT getNode(int nIndex, SNode** ppNode);
	HRESULT removeHeadThreadUnsafe(void** ppObject);
	HRESULT removeTailThreadUnsafe(void** ppObject);
	HRESULT addHeadThreadUnsafe(SNode* pNode);
	HRESULT addTailThreadUnsafe(SNode* pNode);

public:
	LinkedList(BOOL bThreadSafe = FALSE);
	~LinkedList(void);

	HRESULT lock();
	HRESULT unlock();

	HRESULT getHead(void** ppObject);
	HRESULT getTail(void** ppObject);
	HRESULT get(int nIndex, void** ppObject);

	HRESULT removeHead(void** ppObject);
	HRESULT removeTail(void** ppObject);
	HRESULT remove(int nIndex, void** ppObject);
	HRESULT remove(const void* pObject);

	HRESULT addTail(void* pObject);
	HRESULT addHead(void* pObject);
	HRESULT insert(int nIndex, void* pObject);

	HRESULT getCount(int* pCount);
	int getCount();

	HRESULT clear();
	HRESULT setClearCallback(pfnDeleteObject DeleteObject);

	HRESULT beginEnumeration(BOOL bReverse);
	HRESULT next(void** ppObject);
	HRESULT endEnumeration();
};

#endif
