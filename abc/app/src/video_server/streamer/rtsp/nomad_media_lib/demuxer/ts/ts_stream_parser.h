
#ifndef _TS_STREAM_PARSER_H_
#define _TS_STREAM_PARSER_H_

#include <map>
//#include <unordered_map>


#include "base/nml_defines.h"
#include "base/byte_queue.h"
#include "reader/file_reader.h"

#include "demuxer/ts/ts_packet_parser.h"
#include "demuxer/ts/stream_parser.h"
#include "demuxer/ts/pid_state.h"
#include "demuxer/ts/register_interface.h"
#include "demuxer/ts/index_file_loader.h"

namespace nml_demuxer
{
namespace ts
{

class TsStreamParser : public StreamParser ,
                       public RegisterInterface
{
public:
    typedef std::map<int, PidState> PidMap;
	typedef std::map<uint64,  int64> IndexMap;
	typedef std::map<float,  uint64> IndexBiMap;

    TsStreamParser();
    virtual ~TsStreamParser();

    double getDuration();


    void init(const std::string& file_name);
    bool initIndexFile(const std::string& file_name);
    //////////////////////////////////////////////////////////////////////////
    // StreamParser implementaion
    Status parse(SendEvent e = nullptr);
    virtual Status parse(const uint8* buffer, int size, SendEvent e = nullptr) OVERRIDE;
    virtual void flush() OVERRIDE;

    Status seekStream(const float seek_npt_time);

    //////////////////////////////////////////////////////////////////////////
    // RegisterInterface
    virtual void registerPmtCB(int program_number, int pmt_pid) OVERRIDE;
    virtual void registerPesCB(int pes_pid, int stream_type) OVERRIDE;

    //////////////////////////////////////////////////////////////////////////
    // callback
    void onVideoPts(const int64  pts);


	void  resetTimestamp()
	{
        firstPts = kNoTimestamp;
	}

    int64 getPts() {return pts; }
    int64 getPosition() { return position;}

    void setScaleFactor(int trick_scale);
    void setFileName(std::string file_name) { fileName = file_name;}
private:

    Status trickModeFilter(int current_pid);
    base::ByteQueue tsByteQueue;
    ts::TsPacket tsPacket;
    PidMap pidMaps;
    IndexMap indexHash;
    IndexBiMap indexBiHash;


    int64  pts;
    int64  firstPts;
    int64  previousPts;
    int64  position;

    int    scaleFactor;
    uint64    packetNumber;
    uint64    prevKeyFramePacketNumber;

    DropFrameType frameType;
    std::string fileName;
    IndexFileLoader indexFile;

    nml_reader::FileReader fileReader;
};

} //namespace ts
} //namespace nml_demuxer

#endif //_TS_STREAM_PARSER_H_



