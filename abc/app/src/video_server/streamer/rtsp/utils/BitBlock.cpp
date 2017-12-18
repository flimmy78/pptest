#include "nomad.h"
#include "BitBlock.h"

CBitBlock::CBitBlock(void)
{
	m_pcbBuffer = NULL;
	m_nBufSize = 0u;
	m_nPos = 0u;
}

CBitBlock::~CBitBlock(void)
{
}

HRESULT CBitBlock::SetBuffer(void* pBuffer, DWORD nBufSize)
{
	m_pcbBuffer = (BYTE*) pBuffer;
	m_nBufSize = nBufSize;

	m_nPos = 0u;

	m_nMaxBitCount = nBufSize * 8;

	return S_OK;
}

HRESULT CBitBlock::Reset(UINT nPos)
{
	m_nPos = nPos;
	return S_OK;
}

HRESULT CBitBlock::Offset(UINT nOffset)
{
	m_nPos += nOffset;
	return S_OK;
}

HRESULT CBitBlock::GetValue(UINT nBitCount, UINT* pValue)
{
	if(nBitCount + m_nPos > m_nMaxBitCount)
	{
		return E_INVALIDARG;
	}

	HRESULT hr;
	UINT nValue = 0;
	UINT nBit;

	for(int i=0; i<(int) nBitCount; i++)
	{
		hr = GetNextBit(&nBit);
		nValue = (nValue << 1) | nBit;
	}

	*pValue = nValue;

	return S_OK;
}

HRESULT CBitBlock::GetBit(UINT nIndex, UINT* pBit)
{
	UINT nByteIndex = nIndex / 8;

	UINT nBitIndex = 7 - (nIndex % 8);

	UINT nMask = 1u << nBitIndex;

	*pBit = (m_pcbBuffer[nByteIndex] & nMask) >> nBitIndex;

	return S_OK;
}

HRESULT CBitBlock::GetNextBit(UINT* pBit)
{
	GetBit(m_nPos, pBit);
	m_nPos++;

	return S_OK;
}
