#include "nomad.h"
#include "LinkedList.h"
#include "ThreadSafe.h"

/** @brief LinkedList ������.
	@return �ǹ� ����.
	@param bThreadSafe mutex�� ������� ���� true : ���, false : ������.
*/
LinkedList::LinkedList(BOOL bThreadSafe) :
isThreadSafe(bThreadSafe),
threadSafe(NULL),
size(0),
head(NULL),
tail(NULL),
DeleteObject(NULL),
node(NULL),
reverse(FALSE)
{
	if(bThreadSafe)
	{
		threadSafe = new ThreadSafe();
	}
}

/** @brief LinkedList ������.
	@return �ǹ� ����.
	@param ����.
*/
LinkedList::~LinkedList(void)
{
	clear();

	if(isThreadSafe)
	{
		if(threadSafe)
		{
			delete threadSafe;
		}
	}
}

/** @brief pthread mutex lock �������� �����忡�� ���ÿ� ����� �����ϰ� �����͸� ����ϱ� ����.
	@return �ǹ� ����.
	@param ����.
*/
HRESULT LinkedList::lock()
{
	if(isThreadSafe)
	{
		if(threadSafe)
		{
			threadSafe->lock();
		}
	}

	return S_OK;
}

/** @brief pthread mutex lock �������� �����忡�� ���ÿ� ����� �����ϰ� �����͸� ����ϱ� ����.
	@return �ǹ� ����.
	@param ����.
*/
HRESULT LinkedList::unlock()
{
	if(isThreadSafe)
	{
		if(threadSafe)
		{
			threadSafe->unlock();
		}
	}

	return S_OK;
}

/** @brief linkedlist�� ���� ���� �������� �� ���� �����͸� �����´�.
	@return linkedlist�� �����Ͱ� ������� E_FAIL, �����Ͱ� ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� �Ǿ��� �����͸� �־���.
*/
HRESULT LinkedList::getHead(void** ppObject)
{
	HRESULT hr;

	lock();

	if(head == NULL)
	{
		hr = E_FAIL;
	}
	else
	{
		*ppObject = head->pObject;
		hr = S_OK;
	}

	unlock();

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� �� ���� �����͸� �����´�.
	@return linkedlist�� �����Ͱ� ������� E_FAIL, �����Ͱ� ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� �ǵ��� �����͸� �־���.
*/
HRESULT LinkedList::getTail(void** ppObject)
{
	HRESULT hr;

	lock();

	if(tail == NULL)
	{
		hr = E_FAIL;
	}
	else
	{
		*ppObject = tail->pObject;
		hr = S_OK;
	}

	unlock();

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� �� nIndex ��° �����͸� �����´�.
	@return linkedlist�� nIndex��° �����Ͱ� ���� ��� E_FAIL, ���� ��� S_OK
	@param nIndex linkedlist�� �������� �������� ������ index
	@param ppObject linkedlist�� �������� �������� �� nIndex��°�� �����͸� �־���.
*/
HRESULT LinkedList::get(int nIndex, void** ppObject)
{
	HRESULT hr;
	SNode* pNode;

	lock();
	hr = getNode(nIndex, &pNode);
	unlock();
	
	if(SUCCEEDED(hr))
	{
		*ppObject = pNode->pObject;
	}

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� �Ǿ��� �����͸� �������� linkedlist���� �����Ѵ�. mutex lock�� ������� ����.
	@return linkedlist�� �����Ͱ� ���� ��� E_FAIL, ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� �� ���� �����͸� �־���.
*/
HRESULT LinkedList::removeHeadThreadUnsafe(void** ppObject)
{
	HRESULT hr;

	if(head == NULL)
	{
		hr = E_FAIL;
	}
	else
	{
		SNode* pNode = head;

		if(ppObject)
		{
			*ppObject = pNode->pObject;
		}

		head = pNode->pNext;
		delete pNode;

		if(head == NULL)
		{
			tail = NULL;
		}
		else
		{
			head->pPrev = NULL;
		}

		size--;

		hr = S_OK;
	}

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� �Ǿ��� �����͸� �������� linkedlist���� �����Ѵ�.
	@return linkedlist�� �����Ͱ� ���� ��� E_FAIL, ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� �� ���� �����͸� �־���.
*/
HRESULT LinkedList::removeHead(void** ppObject)
{
	HRESULT hr;

	lock();
	hr = removeHeadThreadUnsafe(ppObject);
	unlock();

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� �ǵ��� �����͸� �������� linkedlist���� �����Ѵ�. mutex lock�� ������� ����.
	@return linkedlist�� �����Ͱ� ���� ��� E_FAIL, ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� �� ���� �����͸� �־���.
*/
HRESULT LinkedList::removeTailThreadUnsafe(void** ppObject)
{
	HRESULT hr;

	if(tail == NULL)
	{
		hr = E_FAIL;
	}
	else
	{
		SNode* pNode = tail;

		if(ppObject)
		{
			*ppObject = pNode->pObject;
		}
		tail = pNode->pPrev;
		delete pNode;

		if(tail == NULL)
		{
			head = NULL;
		}
		else
		{
			tail->pNext = NULL;
		}

		size--;

		hr = S_OK;
	}

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� �ǵ��� �����͸� �������� linkedlist���� �����Ѵ�.
	@return linkedlist�� �����Ͱ� ���� ��� E_FAIL, ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� �� ���� �����͸� �־���.
*/
HRESULT LinkedList::removeTail(void** ppObject)
{
	HRESULT hr;

	lock();
	hr = removeTailThreadUnsafe(ppObject);
	unlock();

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� nIndex��°�� �����͸� �������� linkedlist���� �����Ѵ�. mutex lock�� ������� ����.
	@return linkedlist�� nIndex������ �����Ͱ� ���� ��� E_FAIL, ���� ��� S_OK
	@param ppObject linkedlist�� �������� �������� nIndex ��°�� �����͸� �־���.
*/
HRESULT LinkedList::remove(int nIndex, void** ppObject)
{
	HRESULT hr;

	lock();

	if(nIndex == 0)
	{
		hr = removeHeadThreadUnsafe(ppObject);
	}
	else if(nIndex == size-1)
	{
		hr = removeTailThreadUnsafe(ppObject);
	}
	else
	{
		SNode* pNode;
		SNode* pPrevNode;
		SNode* pNextNode;

		hr = getNode(nIndex, &pNode);
		if(SUCCEEDED(hr))
		{
			pPrevNode = pNode->pPrev;
			pNextNode = pNode->pNext;

			pPrevNode->pNext = pNextNode;
			pNextNode->pPrev = pPrevNode;

			*ppObject = pNode->pObject;
			delete pNode;

			size--;
		}
	}

	unlock();

	return hr;
}

/** @brief linkedlist�� ���� ���� �������� pObject �����͸� ����
	@return linkedlist�� pObjcet �����Ͱ� ������ S_OK, ������ E_FAIL
	@param ppObject linkedlist���� ������ ������
*/
HRESULT LinkedList::remove(const void* pObject)
{
	HRESULT hr;
	SNode* pNode;
	int nIndex;

	lock();

	hr = getNode(pObject, &nIndex, &pNode);

	if(SUCCEEDED(hr))
	{
		if(nIndex == 0)
		{
			hr = removeHeadThreadUnsafe(NULL);
		}
		else if(nIndex == size-1)
		{
			hr = removeTailThreadUnsafe(NULL);
		}
		else
		{
			SNode* pPrevNode;
			SNode* pNextNode;

			pPrevNode = pNode->pPrev;
			pNextNode = pNode->pNext;

			pPrevNode->pNext = pNextNode;
			pNextNode->pPrev = pPrevNode;

			delete pNode;

			size--;
		}
	}

	unlock();

	return hr;
}

/** @brief linkedlist�� �ǳ��� �����͸� �߰� �Ѵ�. mutex lock�� ������� ����.
	@return S_OK
	@param pNode linkedlist�� �߰��� ������
*/
HRESULT LinkedList::addTailThreadUnsafe(SNode* pNode)
{
	pNode->pNext = NULL;

	if(tail == NULL)
	{
		pNode->pPrev = NULL;
		head = tail = pNode;
	}
	else
	{
		tail->pNext = pNode;
		pNode->pPrev = tail;

		tail = pNode;
	}

	size++;

	return S_OK;
}

/** @brief linkedlist�� �ǳ��� �����͸� �߰� �Ѵ�.
	@return S_OK
	@param pObject linkedlist�� �߰��� ������
*/
HRESULT LinkedList::addTail(void* pObject)
{
	//HRESULT hr;

	SNode* pNode = new SNode();
	pNode->pObject = pObject;

	lock();
	addTailThreadUnsafe(pNode);
	unlock();

	return S_OK;
}

/** @brief linkedlist�� �Ǿտ� �����͸� �߰� �Ѵ�. mutex lock�� ������� ����.
	@return S_OK
	@param pNode linkedlist�� �߰��� ������
*/
HRESULT LinkedList::addHeadThreadUnsafe(SNode* pNode)
{
	pNode->pPrev = NULL;

	if(head == NULL)
	{
		pNode->pNext = NULL;
		head = tail = pNode;
	}
	else
	{
		head->pPrev = pNode;
		pNode->pNext = head;

		head = pNode;
	}

	size++;

	return S_OK;
}

/** @brief linkedlist�� �Ǿտ� �����͸� �߰� �Ѵ�.
	@return S_OK
	@param pObject linkedlist�� �߰��� ������
*/
HRESULT LinkedList::addHead(void* pObject)
{
	HRESULT hr;

	SNode* pNode = new SNode();
	pNode->pObject = pObject;

	lock();
	hr = addHeadThreadUnsafe(pNode);
	unlock();

	return hr;
}

/** @brief linkedlist�� nIndex ��°�� �����͸� �߰� �Ѵ�.
	@return nIndex < 0 E_FAIL �ƴϸ� S_OK
	@param pObject linkedlist�� �߰��� ������
*/
HRESULT LinkedList::insert(int nIndex, void* pObject)
{
	HRESULT hr;

	if(nIndex < 0)
	{
		return E_FAIL;
	}

	lock();

	SNode* pNode = new SNode();
	pNode->pObject = pObject;

	if(nIndex == 0)
	{
		hr = addHeadThreadUnsafe(pNode);
	}
	else if(nIndex >= size)
	{
		hr = addTailThreadUnsafe(pNode);
	}
	else
	{
		SNode* pNextNode;
		SNode* pPrevNode;

		hr = getNode(nIndex, &pNextNode);

		pPrevNode = pNextNode->pPrev;

		// Connect PrevNode to NewNode.
		pPrevNode->pNext = pNode;
		pNode->pPrev = pPrevNode;

		// Connect NewNode to NextNode.
		pNode->pNext = pNextNode;
		pNextNode->pPrev = pNode;

		size++;
	}

	unlock();

	return hr;
}

/** @brief linkedlist�� ���� �������� ������ ������ �����´�.
	@return S_OK
	@param pCount linkedlist�� ���� �������� �������� ����
*/
HRESULT LinkedList::getCount(int* pCount)
{
	lock();
	*pCount = size;
	unlock();

	return S_OK;
}

/** @brief linkedlist�� ���� �������� ������ ������ �����´�.
	@return linkedlist�� ���� �������� �������� ����
	@param ����.
*/
int LinkedList::getCount()
{
	int nCount; 
	lock();
	nCount = size;
	unlock();

	return nCount;
}

/** @brief linkedlist�� ���� �������� �����͸� ������ callback �Լ��� ����Ѵ�.
	@return S_OK
	@param linkedlist�� ����ִ� �����͸� ������ callback �Լ�
*/
HRESULT LinkedList::setClearCallback(pfnDeleteObject DeleteObject)
{
	this->DeleteObject = DeleteObject;
	return S_OK;
}

/** @brief ��ϵ� callback �Լ��� �̿��Ͽ� linkedlist�� �ʱ�ȭ�Ѵ�.
	@return S_OK
	@param ����.
*/
HRESULT LinkedList::clear()
{
	lock();

	SNode* pNext;
	SNode* pNode = head;

	while(pNode != NULL)
	{
		pNext = pNode->pNext;

		if(DeleteObject)
		{
			DeleteObject(pNode->pObject);
		}
		delete pNode;

		pNode = pNext;
	}

	head = NULL;
	tail = NULL;
	size = 0;

	unlock();

	return S_OK;
}

/** @brief linkedlist�� �ִ� �������� pObjcet�� index�� node�� �˾ƿ´�.
	@return pObject �����Ͱ� ������ S_OK, ������ E_FAIL
	@param pObject ã�� ������, pIndex pObject�� �ִ� index, ppNode pObject�� node
*/
HRESULT LinkedList::getNode(const void* pObject, int* pIndex, SNode** ppNode)
{
	if(pObject == NULL)
	{
		return E_FAIL;
	}

	int nIndex = 0;
	SNode* pNode = head;

	while(pNode != NULL)
	{
		if(pObject == pNode->pObject)
		{
			break;
		}

		pNode = pNode->pNext;
		nIndex++;
	}

	if(pNode == NULL)
	{
		return E_FAIL;
	}

	if(ppNode)
	{
		*ppNode = pNode;
	}

	if(pIndex)
	{
		*pIndex = nIndex;
	}

	return S_OK;
}

/**
 * Don't make this method be thread-safe.
 * It will cause deadlock problem.
 */

/** @brief linkedlist�� �ִ� �������� nIndex��°�� �����͸� �����´�.
	@return nIndex�� < 0 �̰ų� linkedlist�� size���� ũ�� E_FAIL, �ƴϸ� S_OK
	@param ppNode nIndex ��°�� ������
*/
HRESULT LinkedList::getNode(int nIndex, SNode** ppNode)
{
	if(nIndex < 0 || nIndex >= size)
	{
		return E_FAIL;
	}

	SNode* pNode = head;

	for(int i=0; i<nIndex; i++)
	{
		if(pNode == NULL)
		{
			break;
		}

		pNode = pNode->pNext;
	}

	if(pNode == NULL)
	{
		return E_FAIL;
	}

	*ppNode = pNode;

	return S_OK;
}

/** @brief linkedlist���� ������ ��ġ�� �տ��� ���� ã������ �ڿ��� ���� ã�������� �����Ѵ�.
	@return linkedlist�� �����Ͱ� ������ E_FAIL, ������ S_OK
	@param bReverse true�� �ڿ������� ��ġ, false�� �տ������� ��ġ
*/
HRESULT LinkedList::beginEnumeration(BOOL bReverse)
{
	lock();

	reverse = bReverse;

	if(bReverse)
	{
		// Descending order
		node = tail;
	}
	else
	{
		// Ascending order
		node = head;
	}

	if(node == NULL)
	{
		//Unlock();
		return E_FAIL;
	}

	return S_OK;
}

/** @brief linkedlist���� ���� node�� �������� �����´�.
	@return data�� ������ E_FAIL, ������ S_OK
	@param ppObject�� ���� �����͸� �־��ش�.
*/
HRESULT LinkedList::next(void** ppObject)
{
	if(node == NULL)
	{
		return E_FAIL;
	}

	*ppObject = node->pObject;

	if(reverse)
	{
		node = node->pPrev;
	}
	else
	{
		node = node->pNext;
	}

	return S_OK;
}

/** @brief linkedlist���� ������ ��ġ�� �����Ѵ�.
	@return S_OK
	@param ����.
*/
HRESULT LinkedList::endEnumeration()
{
	node = NULL;
	unlock();

	return S_OK;
}
