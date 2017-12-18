#include "nomad.h"
#include "LinkedList.h"
#include "ThreadSafe.h"

/** @brief LinkedList 생성자.
	@return 의미 없음.
	@param bThreadSafe mutex를 사용할지 여부 true : 사용, false : 사용안함.
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

/** @brief LinkedList 생성자.
	@return 의미 없음.
	@param 없음.
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

/** @brief pthread mutex lock 여러개의 쓰레드에서 동시에 쓸경우 안전하게 데이터를 사용하기 위함.
	@return 의미 없음.
	@param 없음.
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

/** @brief pthread mutex lock 여러개의 쓰레드에서 동시에 쓸경우 안전하게 데이터를 사용하기 위함.
	@return 의미 없음.
	@param 없음.
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

/** @brief linkedlist에 보관 중인 데이터중 맨 앞의 데이터를 가져온다.
	@return linkedlist에 데어터가 없을경우 E_FAIL, 데이터가 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 맨앞의 데이터를 넣어줌.
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

/** @brief linkedlist에 보관 중인 데이터중 맨 뒤의 데이터를 가져온다.
	@return linkedlist에 데어터가 없을경우 E_FAIL, 데이터가 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 맨뒤의 데이터를 넣어줌.
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

/** @brief linkedlist에 보관 중인 데이터중 맨 nIndex 번째 데이터를 가져온다.
	@return linkedlist에 nIndex번째 데이터가 없을 경우 E_FAIL, 있을 경우 S_OK
	@param nIndex linkedlist에 보관중인 데이터중 가져올 index
	@param ppObject linkedlist에 보관중인 데이터중 맨 nIndex번째의 데이터를 넣어줌.
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

/** @brief linkedlist에 보관 중인 데이터중 맨앞의 데이터를 가져오고 linkedlist에서 삭제한다. mutex lock을 사용하지 않음.
	@return linkedlist에 데이터가 없을 경우 E_FAIL, 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 맨 앞의 데이터를 넣어줌.
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

/** @brief linkedlist에 보관 중인 데이터중 맨앞의 데이터를 가져오고 linkedlist에서 삭제한다.
	@return linkedlist에 데이터가 없을 경우 E_FAIL, 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 맨 앞의 데이터를 넣어줌.
*/
HRESULT LinkedList::removeHead(void** ppObject)
{
	HRESULT hr;

	lock();
	hr = removeHeadThreadUnsafe(ppObject);
	unlock();

	return hr;
}

/** @brief linkedlist에 보관 중인 데이터중 맨뒤의 데이터를 가져오고 linkedlist에서 삭제한다. mutex lock을 사용하지 않음.
	@return linkedlist에 데이터가 없을 경우 E_FAIL, 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 맨 뒤의 데이터를 넣어줌.
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

/** @brief linkedlist에 보관 중인 데이터중 맨뒤의 데이터를 가져오고 linkedlist에서 삭제한다.
	@return linkedlist에 데이터가 없을 경우 E_FAIL, 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 맨 뒤의 데이터를 넣어줌.
*/
HRESULT LinkedList::removeTail(void** ppObject)
{
	HRESULT hr;

	lock();
	hr = removeTailThreadUnsafe(ppObject);
	unlock();

	return hr;
}

/** @brief linkedlist에 보관 중인 데이터중 nIndex번째의 데이터를 가져오고 linkedlist에서 삭제한다. mutex lock을 사용하지 않음.
	@return linkedlist에 nIndex번재의 데이터가 없을 경우 E_FAIL, 있을 경우 S_OK
	@param ppObject linkedlist에 보관중인 데이터중 nIndex 번째의 데이터를 넣어줌.
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

/** @brief linkedlist에 보관 중인 데이터중 pObject 데이터를 삭제
	@return linkedlist에 pObjcet 데이터가 있으면 S_OK, 없으면 E_FAIL
	@param ppObject linkedlist에서 삭제할 데이터
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

/** @brief linkedlist의 맨끝에 데이터를 추가 한다. mutex lock을 사용하지 않음.
	@return S_OK
	@param pNode linkedlist에 추가할 데이터
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

/** @brief linkedlist의 맨끝에 데이터를 추가 한다.
	@return S_OK
	@param pObject linkedlist에 추가할 데이터
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

/** @brief linkedlist의 맨앞에 데이터를 추가 한다. mutex lock을 사용하지 않음.
	@return S_OK
	@param pNode linkedlist에 추가할 데이터
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

/** @brief linkedlist의 맨앞에 데이터를 추가 한다.
	@return S_OK
	@param pObject linkedlist에 추가할 데이터
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

/** @brief linkedlist의 nIndex 번째에 데이터를 추가 한다.
	@return nIndex < 0 E_FAIL 아니면 S_OK
	@param pObject linkedlist에 추가할 데이터
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

/** @brief linkedlist에 현재 보관중인 데이터 갯수를 가져온다.
	@return S_OK
	@param pCount linkedlist에 현재 보관중인 데이터의 갯수
*/
HRESULT LinkedList::getCount(int* pCount)
{
	lock();
	*pCount = size;
	unlock();

	return S_OK;
}

/** @brief linkedlist에 현재 보관중인 데이터 갯수를 가져온다.
	@return linkedlist에 현재 보관중인 데이터의 갯수
	@param 없음.
*/
int LinkedList::getCount()
{
	int nCount; 
	lock();
	nCount = size;
	unlock();

	return nCount;
}

/** @brief linkedlist에 현재 보관중인 데이터를 삭제할 callback 함수를 등록한다.
	@return S_OK
	@param linkedlist에 들어있는 데이터를 삭제할 callback 함수
*/
HRESULT LinkedList::setClearCallback(pfnDeleteObject DeleteObject)
{
	this->DeleteObject = DeleteObject;
	return S_OK;
}

/** @brief 등록된 callback 함수를 이용하여 linkedlist를 초기화한다.
	@return S_OK
	@param 없음.
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

/** @brief linkedlist에 있는 데이터중 pObjcet의 index와 node를 알아온다.
	@return pObject 데이터가 있으면 S_OK, 없으면 E_FAIL
	@param pObject 찾을 데이터, pIndex pObject가 있는 index, ppNode pObject의 node
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

/** @brief linkedlist에 있는 데이터중 nIndex번째의 데이터를 가져온다.
	@return nIndex가 < 0 이거나 linkedlist의 size보다 크면 E_FAIL, 아니면 S_OK
	@param ppNode nIndex 번째의 데이터
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

/** @brief linkedlist에서 데이터 서치시 앞에서 부터 찾을건지 뒤에서 부터 찾을건지를 설정한다.
	@return linkedlist에 데이터가 없으면 E_FAIL, 있으면 S_OK
	@param bReverse true면 뒤에서부터 서치, false면 앞에서부터 서치
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

/** @brief linkedlist에서 현재 node의 다음값을 가져온다.
	@return data가 없으면 E_FAIL, 있으면 S_OK
	@param ppObject에 다음 데이터를 넣어준다.
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

/** @brief linkedlist에서 데이터 서치를 종료한다.
	@return S_OK
	@param 없음.
*/
HRESULT LinkedList::endEnumeration()
{
	node = NULL;
	unlock();

	return S_OK;
}
