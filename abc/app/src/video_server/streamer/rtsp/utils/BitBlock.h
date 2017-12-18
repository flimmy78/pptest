#ifndef __BITBLOCK_H__
#define __BITBLOCK_H__

#ifdef WIN32
#include <Windows.h>
#else
#include <limits.h>
#endif

/**
 * From a buffer we can get any values indicated by the bit position.
 */
class CBitBlock
{
protected:
	/**
	 * Buffer which includes data.
	 * BitBlock don't allocate or deallocate this buffer memory.
	 */
	BYTE* m_pcbBuffer;

	DWORD m_nBufSize;

	UINT m_nMaxBitCount;

	/**
	 * The current bit position
	 */
	UINT m_nPos;

protected:
	//HRESULT GetBit(UINT nIndex, UINT* pBit);
	//HRESULT GetNextBit(UINT* pBit);

public:
	CBitBlock(void);
	~CBitBlock(void);

public:
	HRESULT SetBuffer(void* pBuffer, DWORD nBufSize = ULONG_MAX);

	HRESULT Offset(UINT nOffset);
	HRESULT Reset(UINT nPos = 0u);
	HRESULT GetValue(UINT nBitCount, UINT* pValue);
	HRESULT GetBit(UINT nIndex, UINT* pBit);
	HRESULT GetNextBit(UINT* pBit);
};

#endif
