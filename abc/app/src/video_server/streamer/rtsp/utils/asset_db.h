

#ifndef _ASSET_DB_H_
#define _ASSET_DB_H_

#include <iostream>
#include <vector>
#include <map>

#include <nomad.h>

#include "sqlite3.h"

typedef struct _AssetInfo
{
	char assetId[128];
	char filePath[128];
	char fileName[128];
	char slave_ip[128];
	char duration[128];
	
}AssetInfo;


static std::string kDbName = "asset.s3db";

enum DBOpenState
{
    kCreateAndOpen = 0,
    kJustOpenOk,
    kOpenFail,
};

class CommonDB
{
public:
    CommonDB()
    {
        db = nullptr;
    }

    ~CommonDB()
    {
        close();
        db = nullptr;
    }

    DBOpenState open();
    bool close();
	
	bool foreign_keys_on();
    //transaction
    bool begin();
    bool commit();
    bool rollback();

    sqlite3* getDB()
    {
        return db;
    };

private:
    sqlite3* db;

};


class RtspInfoDB
{
public:
    bool dbCheck();
    bool insertOrUpdateRecord(std::string rtsp_ip, std::string asset_id, std::string file_path, std::string file_name, std::string file_size, std::string protection, std::string duration = "0");
    bool updateRecordDuration(std::string rtsp_ip, std::string duration);
    bool deleteRecordWithRtspIpAndAssetId(std::string rtsp_ip, std::string asset_id);
    std::vector<AssetInfo> getAllItemFromRtspWhereInitFindDuration();
	bool checkRtspInfoWithRtspIPAndAssetId(std::string rtsp_ip, std::string asset_id);

private:
    bool createTable();
    bool isTableExist();
};

class RtspSystemDB
{
public:
    bool dbCheck();
    bool insertOrUpdateRecord(std::string system_ip, int active_flag);
    bool deleteRecordWithKey(std::string system_ip);
    std::map<std::string, int> getAllItemFromSystem();
	bool querySystemInfo(std::string system_ip);

private:
    bool createTable();
    bool isTableExist();
};


class TokenDB
{
public:
    bool dbCheck();
    bool insertOrUpdateRecord(std::string token, std::string asset_id,
                              std::string content_id,
                              std::string valid_time);

    bool deleteRecordWithKey(std::string token_id);
    //bool getAseetIdFromToken(std::string token);
	bool queryTokenInfo(std::string token);

private:
    bool createTable();
    bool isTableExist();
};

class VidInfoDB
{
public:
    bool dbCheck();
    bool insertOrUpdateRecord(std::string logical_path,
                              std::string logical_name,
                              std::string asset_id,
                              std::string valid_time);

    bool deleteRecord(std::string logical_path,
                      std::string logical_name,
                      std::string asset_id);

	bool queryVidInfo(std::string vid);

private:
    bool createTable();
    bool isTableExist();

private:
};

class AssetDB
{

public:
	DBOpenState checkDB();
    bool tableCheck();
    bool insertOrUpdateRecord(std::string asset_id, std::string asset_type_id="");

    bool deleteRecordWithKey(std::string asset_id);
    bool deleteRecordSystemFile(std::string asset_id)
    {
        return deleteRecordWithKey(asset_id);
    };

    bool getAssetInfoFromToken(std::string token, std::vector<AssetInfo>& vecAssetInfo);
    bool getAssetInfoFromFile(std::string filePath, std::string fileName, std::vector<AssetInfo>& vecAssetInfo);
    bool getAssetInfoFromVid(std::string vid, std::vector<AssetInfo>& vecAssetInfo);
    bool getAssetInfoFromVidAndToken(std::string vid, std::string token, std::vector<AssetInfo>& vecAssetInfo);
	bool getAssetInfoFromFileAndToken(std::string filepath, std::string filename, std::string token, std::vector<AssetInfo>& vecAssetInfo);

    std::vector<std::string> getAllRecords();

	
	bool queryAssetId(std::string asset_id);
	bool querySystemInfo(std::string asset_id, std::string rtspIp);

private:
    bool createTable();
    bool isTableExist();

private:
};

EXTERN_C_BEGIN
	
	void setDBPath(std::string path);
	CommonDB& getCommonDB();
	DBOpenState checkDB();

	AssetDB& getAssetDB();
	VidInfoDB& getVidInfoDB();
	TokenDB& getTokenDB();
	RtspSystemDB& getRtspSystemDB();
	RtspInfoDB& getRtspInfoDB();

EXTERN_C_END


#endif //_ASSET_DB_H_
