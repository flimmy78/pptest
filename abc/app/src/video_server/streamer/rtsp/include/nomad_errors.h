#ifndef __NOMAD_ERRORS_H__
#define __NOMAD_ERRORS_H__

#if !defined(WIN32)

#define SUCCEEDED(hr) (((int)(hr)) >= 0)

//
// and the inverse
//

#define FAILED(hr) (((int)(hr)) < 0)

//
// Return the code
//

#define HRESULT_CODE(hr)    ((hr) & 0xFFFF)
#define SCODE_CODE(sc)      ((sc) & 0xFFFF)

//
//  Return the facility
//

#define HRESULT_FACILITY(hr)  (((hr) >> 16) & 0x1fff)
#define SCODE_FACILITY(sc)    (((sc) >> 16) & 0x1fff)

//
//  Return the severity
//

#define HRESULT_SEVERITY(hr)  (((hr) >> 31) & 0x1)
#define SCODE_SEVERITY(sc)    (((sc) >> 31) & 0x1)

#define MAKE_HRESULT(sev,fac,code) \
    ((int) (((unsigned int)(sev)<<31) | ((unsigned int)(fac)<<16) | ((unsigned int)(code))) )

#define _HRESULT_TYPEDEF_(_sc) ((int)_sc)

#define S_OK			0
#define S_FALSE			1
#define S_STOP			2
#define S_PAUSE			3
#define S_END_OF_STREAM 4
#define S_END_OF_SEGMENT 5
#define S_NEW_SEGMENT 6

#define E_FAIL                           _HRESULT_TYPEDEF_(0x80000000)
#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80000001)
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x80000002)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80000003)
#define E_ABORT                          _HRESULT_TYPEDEF_(0x80000004)
#define E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x80000005)
#define E_TIMEOUT						 _HRESULT_TYPEDEF_(0x80001000)

#endif // #if !defined(WIN32)

// ----------------------------------------------
#define FAC_COMMON 0
#define S_MA_OK 0x00000000
#define S_MA_FALSE 0x00000001
#define E_MA_FAIL 0x80000000
#define E_MA_NOTIMPL 0x80000001
#define E_MA_OUTOFMEMORY 0x80000002
#define E_MA_INVALIDARG 0x80000003
#define E_MA_ABORT 0x80000004
#define E_MA_UNEXPECTED 0x80000005
#define E_MA_UNKNOWN_CONTINER_TYPE 0x80000006
#define E_MA_WRONG_STATE 0x80000007
#define E_MA_UNKNOWN 0x80000008
#define E_MA_CANCEL 0x80000009
#define E_MA_INVALID_SESSION 0x80000010

// ----------------------------------------------
#define FAC_NET 1
#define E_MA_TIMEOUT 0x80010000
#define E_MA_CONNECTION_FAIL 0x80010001
#define E_MA_SEND_FAIL 0x80010002
#define E_MA_RECV_FAIL 0x80010003
#define E_MA_FAILOVER 0x80010004
#define E_MA_FAILED_TO_RUN_HTTP 0x80010005
#define E_MA_FAILED_TO_RUN_RTSP 0x80010006

// ----------------------------------------------
#define FAC_PLAYER 2
#define S_MA_PAUSE 0x00020000
#define S_MA_STOP 0x00020001
#define S_MA_END_OF_STREAM 0x00020002
#define S_MA_END_OF_SEGMENT 0x00020003
#define S_MA_NEW_SEGMENT 0x00020004
#define S_MA_NEW_STREAM 0x00020005
#define E_MA_BUFFER_UNDERFLOW 0x80020000

// ----------------------------------------------
#define FAC_M3U8 3
#define S_MA_M3U8_END_OF_STREAM 0x00030000
#define E_MA_M3U8_GET_STREAM_LIST_FAILURE 0x80030000
#define E_MA_M3U8_GET_PLAY_LIST_FAILURE 0x80030001
#define E_MA_M3U8_GET_SEGMENT_FAILURE 0x80030002
#define E_MA_M3U8_FAILED_TO_FIND_AV_PROP 0x80030003

// ----------------------------------------------
#define FAC_DRM 4
#define E_MA_DRM_INVALID_USER_INFO 0x80040000

// -------------------------------------------------
#define FAC_MP4 5
#define E_MA_MP4_FILE_OPEN_FAIL 0x80050000
#define E_MA_MP4_DOWNLOAD_IS_SLOW 0x80050001
#define E_MA_MP4_HEADER_NOT_FOUND 0x80050002
#define E_MA_MP4_SEEK_OUT_OF_RANGE 0x80050003
#define E_MA_MP4_FILE_READ_FAIL 0x80050004
#define E_MA_MP4_OUT_OF_DATA 0x80050005

#endif
