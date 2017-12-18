
//=======================================================================
// 
// [11/13/2013 by kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _NML_STRUCTURE_H_
#define _NML_STRUCTURE_H_

#include "base/basic_types.h"

#define BSWAP16(x) \
    (uint16) ((x) >> 8 | (x) << 8)

#define BSWAP32(x) \
    (uint32)(((x) << 24) + \
    (((x)&0xff00) << 8) + \
    (((x) >> 8)&0xff00) + \
    ((uint32)(x) >> 24))

#define BSWAP64(x) \
    (uint64)(((x) << 56) + \
    (((x)&0xff00) << 40) + \
    (((x)&0xff0000) << 24) + \
    (((x)&0xff000000) << 8) + \
    (((x) >> 8)&0xff000000) + \
    (((x) >> 24)&0xff0000) + \
    (((x) >> 40)&0xff00) + \
    ((x) >> 56))

#ifdef _BIG_ENDIAN_
#   define BIG_ENDIAN_SWAP16(x) BSWAP16(x)
#   define BIG_ENDIAN_SWAP32(x) BSWAP32(x)
#   define BIG_ENDIAN_SWAP64(x) BSWAP64(x)
#   define LITTLE_ENDIAN_SWAP16(x) (x)
#   define LITTLE_ENDIAN_SWAP32(x) (x)
#   define LITTLE_ENDIAN_SWAP64(x) (x)
#else // _BIG_ENDIAN_
#   define BIG_ENDIAN_SWAP16(x) (x)
#   define BIG_ENDIAN_SWAP32(x) (x)
#   define BIG_ENDIAN_SWAP64(x) (x)
#   define LITTLE_ENDIAN_SWAP16(x) BSWAP16(x)
#   define LITTLE_ENDIAN_SWAP32(x) BSWAP32(x)
#   define LITTLE_ENDIAN_SWAP64(x) BSWAP64(x)
#endif // _BIG_ENDIAN_

enum NCMFStatus
{
    NML_OK                          = 0,    // no error
    NML_ERROR_FAILED                = -999, //-999,
    NML_ERROR_NOT_INITIALIZED       = -998,
    NML_ERROR_NOT_ENOUGH_DATA       = -996, // not enough input data
    NML_ERROR_NULL_PTR              = -995, // null pointer in input parameters
    NML_ERR_NOT_ENOUGH_BUFFER       = -896,  // not enough buffer to put output data
    NML_ERROR_END_OF_STREAM         = -895,
    NML_ERROR_BEGIN_OF_STREAM       = -897,
    NML_ERROR_ALLOC                 = -883,  // failed to allocate memory
    NML_ERROR_INVALID_STREAM        = -882,
};

typedef int32 Status;


//////////////////////////////////////////////////////////////////////////
//


enum SystemStreamType
{
    UNDEF_STREAM            = 0x00000000, //unsupported stream type
    AVI_STREAM              = 0x00000001, //AVI RIFF
    MP4_ATOM_STREAM         = 0x00000010, //ISO/IEC 14496-14 stream
    ASF_STREAM              = 0x00000100, //ASF stream

    H26x_PURE_VIDEO_STREAM  = 0x00100000,
    H261_PURE_VIDEO_STREAM  = H26x_PURE_VIDEO_STREAM|0x00010000,
    H263_PURE_VIDEO_STREAM  = H26x_PURE_VIDEO_STREAM|0x00020000,
    H264_PURE_VIDEO_STREAM  = H26x_PURE_VIDEO_STREAM|0x00040000,

    MPEGx_SYSTEM_STREAM     = 0x00001000,                    //MPEG 1,2 - like system

    MPEG1_SYSTEM_STREAM     = MPEGx_SYSTEM_STREAM|0x00000100,//MPEG 1 system
    MPEG2_SYSTEM_STREAM     = MPEGx_SYSTEM_STREAM|0x00000200,//MPEG 2 system
    MPEG4_SYSTEM_STREAM     = MPEGx_SYSTEM_STREAM|0x00000400,//MPEG 4 system

    MPEGx_PURE_VIDEO_STREAM = MPEGx_SYSTEM_STREAM|0x00000010,//MPEG 1,2 - like pure video data
    MPEGx_PURE_AUDIO_STREAM = MPEGx_SYSTEM_STREAM|0x00000020,//MPEG 1,2 - like pure audio data
    MPEGx_PES_PACKETS_STREAM= MPEGx_SYSTEM_STREAM|0x00000040,//MPEG 1,2 - like pes packets system
    MPEGx_PROGRAMM_STREAM   = MPEGx_SYSTEM_STREAM|0x00000080,//MPEG 1,2 - like program system
    MPEGx_TRANSPORT_STREAM  = MPEGx_SYSTEM_STREAM|0x000000c0,//MPEG 1,2 - like transport system


    MPEG1_PURE_VIDEO_STREAM = MPEG1_SYSTEM_STREAM|MPEGx_PURE_VIDEO_STREAM, //MPEG1 pure video stream
    MPEG1_PURE_AUDIO_STREAM = MPEG1_SYSTEM_STREAM|MPEGx_PURE_AUDIO_STREAM, //MPEG1 pure video stream
    MPEG1_PES_PACKETS_STREAM= MPEG1_SYSTEM_STREAM|MPEGx_PES_PACKETS_STREAM,//MPEG1 pes packets stream
    MPEG1_PROGRAMM_STREAM   = MPEG1_SYSTEM_STREAM|MPEGx_PROGRAMM_STREAM,   //MPEG1 program stream

    MPEG2_PURE_VIDEO_STREAM = MPEG2_SYSTEM_STREAM|MPEGx_PURE_VIDEO_STREAM,//MPEG2 pure video stream
    MPEG2_PURE_AUDIO_STREAM = MPEG2_SYSTEM_STREAM|MPEGx_PURE_AUDIO_STREAM,//MPEG2 pure audio stream
    MPEG2_PES_PACKETS_STREAM= MPEG2_SYSTEM_STREAM|MPEGx_PES_PACKETS_STREAM,//MPEG2 pes packets stream
    MPEG2_PROGRAMM_STREAM   = MPEG2_SYSTEM_STREAM|MPEGx_PROGRAMM_STREAM,   //MPEG2 program stream
    MPEG2_TRANSPORT_STREAM  = MPEG2_SYSTEM_STREAM|MPEGx_TRANSPORT_STREAM,  //MPEG2 transport stream
    //MPEG2_TRANSPORT_STREAM_TTS  = MPEG2_SYSTEM_STREAM|MPEGx_TRANSPORT_STREAM | 1,  //MPEG2 transport stream with valid packet time stamps
    //MPEG2_TRANSPORT_STREAM_TTS0 = MPEG2_SYSTEM_STREAM|MPEGx_TRANSPORT_STREAM | 2,  //MPEG2 transport stream with zero packet time stamps

    MPEG4_PURE_VIDEO_STREAM = MPEG4_SYSTEM_STREAM|MPEGx_PURE_VIDEO_STREAM,//MPEG4 pure video stream

    WEB_CAM_STREAM          = 0x00100000,
    ADIF_STREAM             = 0x00200000,
    ADTS_STREAM             = 0x00400000,
    STILL_IMAGE             = 0x00800000,
    VC1_PURE_VIDEO_STREAM   = 0x01000000,
    WAVE_STREAM             = 0x02000000
};

enum AudioStreamType
{
    UNDEF_AUDIO             = 0x00000000,
    PCM_AUDIO               = 0x00000001,
    LPCM_AUDIO              = 0x00000002,
    AC3_AUDIO               = 0x00000004,
    ALAW_AUDIO              = 0x00000006,
    MULAW_AUDIO             = 0x00000007,
    TWINVQ_AUDIO            = 0x00000008,
    DTS_AUDIO               = 0x00000016,

    MPEG1_AUDIO             = 0x00000100,
    MPEG2_AUDIO             = 0x00000200,
    MPEG_AUDIO_LAYER1       = 0x00000010,
    MPEG_AUDIO_LAYER2       = 0x00000020,
    MPEG_AUDIO_LAYER3       = 0x00000040,

    MP1L1_AUDIO             = MPEG1_AUDIO|MPEG_AUDIO_LAYER1,
    MP1L2_AUDIO             = MPEG1_AUDIO|MPEG_AUDIO_LAYER2,
    MP1L3_AUDIO             = MPEG1_AUDIO|MPEG_AUDIO_LAYER3,
    MP2L1_AUDIO             = MPEG2_AUDIO|MPEG_AUDIO_LAYER1,
    MP2L2_AUDIO             = MPEG2_AUDIO|MPEG_AUDIO_LAYER2,
    MP2L3_AUDIO             = MPEG2_AUDIO|MPEG_AUDIO_LAYER3,

    VORBIS_AUDIO            = 0x00000400,
    AAC_AUDIO               = 0x00000800,
    AAC_FMT_UNDEF           = 0x00000000,   /// Undefined stream format, the decoder have to identify by bitstream
    AAC_FMT_RAW             = 0x00000001,   /// Raw input stream format, the first frame keeps init information
    AAC_FMT_EX_GA           = 0x00000010,   /// GASpecificConfig header within the first frame.
    AAC_MPEG4_STREAM        = AAC_AUDIO | AAC_FMT_RAW | AAC_FMT_EX_GA,

    AMR_NB_AUDIO            = 0x00000777,  //narrow band amr
    AMR_WB_AUDIO            = 0x00000778   //wide band amr
};

enum AudioStreamSubType
{
    UNDEF_AUDIO_SUBTYPE     = 0x00000000,
    AAC_LC_PROFILE          = 0x00000001,
    AAC_LTP_PROFILE         = 0x00000002,
    AAC_MAIN_PROFILE        = 0x00000004,
    AAC_SSR_PROFILE         = 0x00000008,
    AAC_HE_PROFILE          = 0x00000010,
    AAC_ALS_PROFILE         = 0x00000020,
    AAC_BSAC_PROFILE        = 0x00000040
};

enum VideoStreamType
{
    UNDEF_VIDEO             = 0x00000000,
    UNCOMPRESSED_VIDEO      = 0x00000001,
    MPEG1_VIDEO             = 0x00000011,
    MPEG2_VIDEO             = 0x00000012,
    MPEG4_VIDEO             = 0x00000014,
    H261_VIDEO              = 0x00000120,
    H263_VIDEO              = 0x00000140,
    H264_VIDEO              = 0x00000180,
    DIGITAL_VIDEO_SD        = 0x00001200,
    DIGITAL_VIDEO_50        = 0x00001400,
    DIGITAL_VIDEO_HD        = 0x00001800,
    DIGITAL_VIDEO_SL        = 0x00002000,
    WMV_VIDEO               = 0x00010000,
    MJPEG_VIDEO             = 0x00020000,
    YV12_VIDEO              = 0x00040000,
    VC1_VIDEO               = 0x00050000
};

enum VideoStreamSubType
{
    UNDEF_VIDEO_SUBTYPE     = 0x00000000,
    MPEG4_VIDEO_DIVX5       = 0x00000001,
    MPEG4_VIDEO_QTIME       = 0x00000002,
    DIGITAL_VIDEO_TYPE_1    = 3,
    DIGITAL_VIDEO_TYPE_2,
    MPEG4_VIDEO_DIVX3,
    MPEG4_VIDEO_DIVX4,
    MPEG4_VIDEO_XVID,
    AVC1_VIDEO,
    H263_VIDEO_SORENSON,
    VC1_VIDEO_RCV           = 0x00110000,
    VC1_VIDEO_VC1           = 0x00120000,
    WVC1_VIDEO,
    WMV3_VIDEO,
    MULTIVIEW_VIDEO         = 0x01000000,
    SCALABLE_VIDEO          = 0x02000000
};

enum Frame_Type
{
    NONE_PICTURE            = 0,
    I_PICTURE               = 1,
    P_PICTURE               = 2,
    B_PICTURE               = 3,
    D_PICTURE               = 4,
    VIDEO_FRAME             = 0x7,
    AUDIO_FRAME             = 0x8
};


#endif //_NML_STRUCTURE_H_



