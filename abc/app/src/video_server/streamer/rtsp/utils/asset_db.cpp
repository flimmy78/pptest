
#include <stdio.h>
#include "asset_db.h"
// -----------------------------------------------------------------------------
EXTERN_C_BEGIN

void setDBPath(std::string path)
{
	kDbName = path;
}

CommonDB& getCommonDB()
{
	static CommonDB commonDB;
	return commonDB;
}

DBOpenState checkDB()
{
	static AssetDB assetDB;
	return assetDB.checkDB();
}

AssetDB& getAssetDB()
{
	static AssetDB assetDB;
	assetDB.tableCheck();
	return assetDB;
}

VidInfoDB& getVidInfoDB()
{
	static VidInfoDB vidDB;
	vidDB.dbCheck();
	return vidDB;
}

TokenDB& getTokenDB()
{
	static TokenDB tokenDB;
	tokenDB.dbCheck();
	return tokenDB;
}

RtspSystemDB& getRtspSystemDB()
{
	static RtspSystemDB rtspSystemDB;
	rtspSystemDB.dbCheck();
	return rtspSystemDB;
}

RtspInfoDB& getRtspInfoDB()
{
	static RtspInfoDB rtspInfoDB;
	rtspInfoDB.dbCheck();
	return rtspInfoDB;
}

EXTERN_C_END
// --------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// ConmmonDB

DBOpenState CommonDB::open()
{
    int rc = sqlite3_open_v2(kDbName.c_str(), &db , SQLITE_OPEN_READWRITE, NULL);
    if (rc == SQLITE_OK)
        return kJustOpenOk;

    rc = sqlite3_open(kDbName.c_str(), &db);

    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return kOpenFail;
    }

    return kCreateAndOpen;
}

bool CommonDB::close()
{
    return (sqlite3_close(db) == SQLITE_OK);
}

bool CommonDB::foreign_keys_on()
{
    sqlite3_stmt* stmt;
    const char* query = "PRAGMA foreign_keys = ON;";
    bool result = true;

    sqlite3_prepare_v2(this->db, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(this->db));
        result = false;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

bool CommonDB::begin()
{
    sqlite3_stmt* stmt;
    const char* query = "BEGIN;";
    bool result = true;

    sqlite3_prepare_v2(this->db, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(this->db));
        result = false;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}


bool CommonDB::commit()
{
    sqlite3_stmt* stmt;
    const char* query = "COMMIT;";
    bool result = true;

    sqlite3_prepare_v2(this->db, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(this->db));
        result = false;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}


bool CommonDB::rollback()
{
    sqlite3_stmt* stmt;
    const char* query = "ROLLBACK;";
    bool result = true;

    sqlite3_prepare_v2(this->db, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(this->db));
        result = false;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

//////////////////////////////////////////////////////////////////////////
// VidInfoDB

bool VidInfoDB::dbCheck()
{
    if(this->isTableExist() == false)
    {
        this->createTable();
    }

    return true;
}

bool VidInfoDB::insertOrUpdateRecord(std::string logical_path,
                                     std::string logical_name,
                                     std::string asset_id,
                                     std::string valid_time)
{
    sqlite3_stmt* stmt;
    const char* query = "INSERT OR REPLACE INTO vid_info VALUES(?, ?, ?, ?);";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, logical_path.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, asset_id.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, logical_name.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, valid_time.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}


bool VidInfoDB::deleteRecord(std::string logical_path,
                             std::string logical_name,
                             std::string asset_id)
{
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM vid_info WHERE logical_path=? AND logical_name=? AND asset_id=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, logical_path.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, logical_name.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, asset_id.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}


bool VidInfoDB::createTable()
{
    sqlite3_stmt* stmt;

    std::string QUERY_TABLE_CREATE = " CREATE TABLE [vid_info] ("
                                     "[logical_path] TEXT UNIQUE NOT NULL PRIMARY KEY,"
									 "[asset_id] TEXT not null, "
                                     "[logical_name] TEXT NOT NULL,"
                                     "[valid_time] TEXT  NOT NULL, "
									 "foreign key ( asset_id ) references asset_info( asset_id ) on delete cascade)" ;

    const char* query = QUERY_TABLE_CREATE.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}



bool VidInfoDB::isTableExist()
{
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='vid_info';";
    bool result = false;

    sqlite3* sdb = getCommonDB().getDB();
    sqlite3_prepare_v2(sdb, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = true;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

bool VidInfoDB::queryVidInfo(std::string vid)
{
	sqlite3_stmt* stmt;

    std::string QUERY_SQL = "select logical_path from vid_info where logical_path = ?";

    const char* query = QUERY_SQL.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, vid.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}




//////////////////////////////////////////////////////////////////////////
// token db

bool TokenDB::dbCheck()
{
    if(this->isTableExist() == false)
    {
        this->createTable();
    }

    return true;
}

bool TokenDB::insertOrUpdateRecord(std::string token,
                                   std::string asset_id,
                                   std::string content_id,
                                   std::string valid_time)
{
    sqlite3_stmt* stmt;
    const char* query = "INSERT OR REPLACE INTO token_info VALUES(?, ?, ?, ?);";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, token.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, asset_id.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, content_id.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, valid_time.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}


bool TokenDB::deleteRecordWithKey(std::string token_id)
{
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM token_info WHERE token_id=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, token_id.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool TokenDB::createTable()
{
    sqlite3_stmt* stmt;

    std::string QUERY_TABLE_CREATE = "CREATE TABLE [token_info] ("
                                     "[token_id] TEXT  UNIQUE NOT NULL PRIMARY KEY,"
                                     "[asset_id] TEXT not null,"
                                     "[content_id] TEXT  NOT NULL,"
                                     "[valid_time] integer  NULL, "
									 "foreign key ( asset_id ) references asset_info( asset_id ) on delete cascade)" ;

    const char* query = QUERY_TABLE_CREATE.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}



bool TokenDB::isTableExist()
{
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='token_info';";
    bool result = false;

    sqlite3* sdb = getCommonDB().getDB();
    sqlite3_prepare_v2(sdb, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = true;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}


bool TokenDB::queryTokenInfo(std::string token)
{
	sqlite3_stmt* stmt;

    std::string QUERY_SQL = "select token_id from token_info where token_id = ?";

    const char* query = QUERY_SQL.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, token.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}




//////////////////////////////////////////////////////////////////////////
// rtsp system db

bool RtspSystemDB::dbCheck()
{
    if(this->isTableExist() == false)
    {
        this->createTable();
    }

    return true;
}

bool RtspSystemDB::insertOrUpdateRecord(std::string system_ip, int active_flag)
{
    sqlite3_stmt* stmt;
    const char* query = "INSERT OR REPLACE INTO rtsp_system VALUES(?, ?);";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, system_ip.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, active_flag);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}


bool RtspSystemDB::deleteRecordWithKey(std::string system_ip)
{
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM rtsp_system WHERE system_ip=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, system_ip.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

std::map<std::string, int> RtspSystemDB::getAllItemFromSystem()
{
	std::map<std::string, int> mapServer;
	sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM rtsp_system;";
    

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		mapServer.insert(std::map<std::string, int>::value_type(std::string((char*)sqlite3_column_text(stmt, 0)), sqlite3_column_int(stmt, 1)));
    };
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
	return mapServer;
}

bool RtspSystemDB::createTable()
{
    sqlite3_stmt* stmt;

    std::string QUERY_TABLE_CREATE = "CREATE TABLE [rtsp_system] ("
                                     "[system_ip] TEXT UNIQUE NOT NULL PRIMARY KEY,"
                                     "[active_flag] integer default(1))";

    const char* query = QUERY_TABLE_CREATE.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}



bool RtspSystemDB::isTableExist()
{
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='rtsp_system';";
    bool result = false;

    sqlite3* sdb = getCommonDB().getDB();
    sqlite3_prepare_v2(sdb, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = true;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

bool RtspSystemDB::querySystemInfo(std::string system_ip)
{
	sqlite3_stmt* stmt;

    std::string QUERY_SQL = "select * from rtsp_system where system_ip = ?";

    const char* query = QUERY_SQL.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, system_ip.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

//////////////////////////////////////////////////////////////////////////
// rtsp info db

bool RtspInfoDB::dbCheck()
{
    if(this->isTableExist() == false)
    {
        this->createTable();
    }

    return true;
}

bool RtspInfoDB::updateRecordDuration(std::string rtsp_ip, std::string duration)
{
	sqlite3_stmt* stmt;
    const char* query = "update rtsp_info set duration = ? where rtsp_ip=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, duration.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, rtsp_ip.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool RtspInfoDB::insertOrUpdateRecord(std::string rtsp_ip, std::string asset_id, std::string file_path, std::string file_name, std::string file_size, std::string protection, std::string duration)
{
    sqlite3_stmt* stmt;
    const char* query = "INSERT INTO rtsp_info(rtsp_ip, asset_id, file_path, file_name, file_size, protection_type, duration) VALUES(?, ?, ?, ?, ?, ?, ?);";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, rtsp_ip.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, asset_id.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, file_path.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, file_name.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, file_size.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 6, protection.c_str(), -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 7, duration.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}


bool RtspInfoDB::deleteRecordWithRtspIpAndAssetId(std::string rtsp_ip, std::string asset_id)
{
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM rtsp_info WHERE rtsp_ip=? and asset_id=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, rtsp_ip.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, asset_id.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

std::vector<AssetInfo> RtspInfoDB::getAllItemFromRtspWhereInitFindDuration()
{
	std::vector<AssetInfo> vecAssetInfo;
	sqlite3_stmt* stmt;
    const char* query = "SELECT file_path, file_name, rtsp_ip, duration FROM rtsp_info where duration='0';";    

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		AssetInfo assetInfo;
		memset(&assetInfo, 0x00, sizeof(assetInfo));
		strncpy(assetInfo.filePath, (const char*)sqlite3_column_text(stmt, 0), sizeof(assetInfo.filePath) -1);
		strncpy(assetInfo.fileName, (const char*)sqlite3_column_text(stmt, 1), sizeof(assetInfo.fileName) -1);
		strncpy(assetInfo.slave_ip, (const char*)sqlite3_column_text(stmt, 2), sizeof(assetInfo.slave_ip) -1);
		strncpy(assetInfo.duration, (const char*)sqlite3_column_text(stmt, 3), sizeof(assetInfo.duration) -1);
		vecAssetInfo.push_back(assetInfo);
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
	return vecAssetInfo;
}

bool RtspInfoDB::createTable()
{
    sqlite3_stmt* stmt;

    std::string QUERY_TABLE_CREATE = "CREATE TABLE [rtsp_info] ("
                                     "[rtsp_ip] TEXT UNIQUE NOT NULL PRIMARY KEY, "
									 "[asset_id] TEXT not null, "
                                     "[file_path] TEXT NOT NULL, "
                                     "[file_name] TEXT NOT NULL, "
                                     "[file_size] TEXT NOT NULL, "
                                     "[protection_type] TEXT NOT NULL,"
                                     "[duration] TEXT NOT NULL DEFAULT '0', "
									 "foreign key ( asset_id ) references asset_info( asset_id ) on delete cascade)" ;

    const char* query = QUERY_TABLE_CREATE.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}



bool RtspInfoDB::isTableExist()
{
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='rtsp_info';";
    bool result = false;

    sqlite3* sdb = getCommonDB().getDB();
    sqlite3_prepare_v2(sdb, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = true;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

bool RtspInfoDB::checkRtspInfoWithRtspIPAndAssetId(std::string rtsp_ip, std::string asset_id)
{
	sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM rtsp_info WHERE rtsp_ip=? and asset_id=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, rtsp_ip.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, asset_id.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
		result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

//////////////////////////////////////////////////////////////////////////
// asset db
// 사용하는 곳에서 맨처음 반드시 한번만 호출 해야 함.
DBOpenState	AssetDB::checkDB()
{
	DBOpenState ret = getCommonDB().open();
	getCommonDB().foreign_keys_on();
	return ret;
}

bool AssetDB::tableCheck()
{
    if(this->isTableExist() == false)
    {
        this->createTable();
    }

    return true;
}


bool AssetDB::insertOrUpdateRecord(std::string asset_id, std::string asset_type_id)
{
    sqlite3_stmt* stmt;
    //const char* query = "INSERT OR REPLACE INTO asset_info VALUES(?, ?);";
	const char* query = "INSERT INTO asset_info(asset_id, asset_type_id) VALUES(?, ?);";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, asset_id.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, asset_type_id.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}



bool AssetDB::deleteRecordWithKey(std::string asset_id)
{
	sqlite3_stmt* stmt;
    const char* query = "DELETE FROM asset_info WHERE asset_id=?;";
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, asset_id.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool AssetDB::getAssetInfoFromToken(std::string token, std::vector<AssetInfo>& vecAssetInfo)
{
	sqlite3_stmt* stmt;
    const char* query = "SELECT rtsp.file_path, rtsp.file_name, rtsp.rtsp_ip, rtsp.duration "
						"FROM  token_info token, asset_info asset, rtsp_info rtsp "
						"WHERE token.token_id=? and asset.asset_id=token.asset_id and asset.asset_id=rtsp.asset_id;";
    bool result = false;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, token.c_str(), -1,SQLITE_STATIC);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		AssetInfo assetInfo;
		memset(&assetInfo, 0x00, sizeof(assetInfo));
		strncpy(assetInfo.filePath, (const char*)sqlite3_column_text(stmt, 0), sizeof(assetInfo.filePath) -1);
		strncpy(assetInfo.fileName, (const char*)sqlite3_column_text(stmt, 1), sizeof(assetInfo.fileName) -1);
		strncpy(assetInfo.slave_ip, (const char*)sqlite3_column_text(stmt, 2), sizeof(assetInfo.slave_ip) -1);
		strncpy(assetInfo.duration, (const char*)sqlite3_column_text(stmt, 3), sizeof(assetInfo.duration) -1);
		vecAssetInfo.push_back(assetInfo);
		result = true;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool AssetDB::getAssetInfoFromVid(std::string vid, std::vector<AssetInfo>& vecAssetInfo)
{
	sqlite3_stmt* stmt;
    const char* query = "SELECT rtsp.file_path, rtsp.file_name, rtsp.rtsp_ip, rtsp.duration "
						"FROM  vid_info vid, asset_info asset, rtsp_info rtsp "
						"WHERE vid.logical_path=? and asset.asset_id=vid.asset_id and asset.asset_id=rtsp.asset_id;";
    bool result = false;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, vid.c_str(), -1,SQLITE_STATIC);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		AssetInfo assetInfo;
		memset(&assetInfo, 0x00, sizeof(assetInfo));
		strncpy(assetInfo.filePath, (const char*)sqlite3_column_text(stmt, 0), sizeof(assetInfo.filePath) -1);
		strncpy(assetInfo.fileName, (const char*)sqlite3_column_text(stmt, 1), sizeof(assetInfo.fileName) -1);
		strncpy(assetInfo.slave_ip, (const char*)sqlite3_column_text(stmt, 2), sizeof(assetInfo.slave_ip) -1);
		strncpy(assetInfo.duration, (const char*)sqlite3_column_text(stmt, 3), sizeof(assetInfo.duration) -1);
		vecAssetInfo.push_back(assetInfo);
		result = true;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool AssetDB::getAssetInfoFromFile(std::string filePath, std::string fileName, std::vector<AssetInfo>& vecAssetInfo)
{
	sqlite3_stmt* stmt;
    const char* query = "SELECT file_path, file_name, rtsp_ip, duration FROM rtsp_info WHERE file_path=? and file_name=?;";
    bool result = false;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, filePath.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, fileName.c_str(), -1,SQLITE_STATIC);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		AssetInfo assetInfo;
		memset(&assetInfo, 0x00, sizeof(assetInfo));
		strncpy(assetInfo.filePath, (const char*)sqlite3_column_text(stmt, 0), sizeof(assetInfo.filePath) -1);
		strncpy(assetInfo.fileName, (const char*)sqlite3_column_text(stmt, 1), sizeof(assetInfo.fileName) -1);
		strncpy(assetInfo.slave_ip, (const char*)sqlite3_column_text(stmt, 2), sizeof(assetInfo.slave_ip) -1);
		strncpy(assetInfo.duration, (const char*)sqlite3_column_text(stmt, 3), sizeof(assetInfo.duration) -1);
		vecAssetInfo.push_back(assetInfo);
		result = true;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool AssetDB::getAssetInfoFromVidAndToken(std::string vid, std::string token, std::vector<AssetInfo>& vecAssetInfo)
{
	sqlite3_stmt* stmt;
    const char* query = "SELECT rtsp.file_path, rtsp.file_name, rtsp.rtsp_ip, rtsp.duration "
						"FROM  vid_info vid, asset_info asset, token_info token, rtsp_info rtsp "
						"WHERE (vid.logical_path=? and vid.asset_id=asset.asset_id and rtsp.asset_id=asset.asset_id) and "
						"(token.token_id=? and asset.asset_id=token.asset_id and rtsp.asset_id=asset.asset_id);";
    bool result = false;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, vid.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, token.c_str(), -1,SQLITE_STATIC);

	while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		AssetInfo assetInfo;
		memset(&assetInfo, 0x00, sizeof(assetInfo));
		strncpy(assetInfo.filePath, (const char*)sqlite3_column_text(stmt, 0), sizeof(assetInfo.filePath) -1);
		strncpy(assetInfo.fileName, (const char*)sqlite3_column_text(stmt, 1), sizeof(assetInfo.fileName) -1);
		strncpy(assetInfo.slave_ip, (const char*)sqlite3_column_text(stmt, 2), sizeof(assetInfo.slave_ip) -1);
		strncpy(assetInfo.duration, (const char*)sqlite3_column_text(stmt, 3), sizeof(assetInfo.duration) -1);
		vecAssetInfo.push_back(assetInfo);
		result = true;
    }
	
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool AssetDB::getAssetInfoFromFileAndToken(std::string filepath, std::string filename, std::string token, std::vector<AssetInfo>& vecAssetInfo)
{
	sqlite3_stmt* stmt;
	const char* query = "SELECT rtsp.file_path, rtsp.file_name, rtsp.rtsp_ip, rtsp.duration "
						"FROM  asset_info asset, token_info token, rtsp_info rtsp "
						"WHERE (rtsp.file_path=? and rtsp.file_name=? and rtsp.asset_id=asset.asset_id) "
						"and (token.token_id=? and asset.asset_id=token.asset_id);";
	bool result = false;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, filepath.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, filename.c_str(), -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, token.c_str(), -1,SQLITE_STATIC);

	while(sqlite3_step(stmt) == SQLITE_ROW)
    {
		AssetInfo assetInfo;
		memset(&assetInfo, 0x00, sizeof(assetInfo));
		strncpy(assetInfo.filePath, (const char*)sqlite3_column_text(stmt, 0), sizeof(assetInfo.filePath) -1);
		strncpy(assetInfo.fileName, (const char*)sqlite3_column_text(stmt, 1), sizeof(assetInfo.fileName) -1);
		strncpy(assetInfo.slave_ip, (const char*)sqlite3_column_text(stmt, 2), sizeof(assetInfo.slave_ip) -1);
		strncpy(assetInfo.duration, (const char*)sqlite3_column_text(stmt, 3), sizeof(assetInfo.duration) -1);
		vecAssetInfo.push_back(assetInfo);
		result = true;
    }
	
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
	return result;
}


std::vector<std::string> AssetDB::getAllRecords()
{
    std::vector<std::string> result;
    sqlite3_stmt* stmt;
    const char* query = "SELECT asset_id FROM asset_info;";

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int type = sqlite3_column_type(stmt, 0);
        if(type != SQLITE_TEXT)
        {
            continue;
        }
        std::string fileName((char*)sqlite3_column_text(stmt, 0));
        result.push_back(fileName);
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}


bool AssetDB::createTable()
{
    sqlite3_stmt* stmt;

    std::string QUERY_TABLE_CREATE = "CREATE TABLE [asset_info] ("
                                     "[asset_id] TEXT UNIQUE NOT NULL PRIMARY KEY ,"
                                     "[asset_type_id] TEXT  NULL);";

    const char* query = QUERY_TABLE_CREATE.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}



bool AssetDB::isTableExist()
{
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='asset_info';";
    bool result = false;

    sqlite3* sdb = getCommonDB().getDB();
    sqlite3_prepare_v2(sdb, query, -1, &stmt, NULL);
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = true;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

bool AssetDB::queryAssetId(std::string assetId)
{
	sqlite3_stmt* stmt;

    std::string QUERY_SQL = "select asset_id, asset_type_id from asset_info where asset_id = ?";

    const char* query = QUERY_SQL.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, assetId.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

bool AssetDB::querySystemInfo(std::string assetId, std::string rtspIp)
{
	sqlite3_stmt* stmt;

    std::string QUERY_SQL = "select slave_ip from asset_info where asset_id = ?";

    const char* query = QUERY_SQL.c_str();
    bool result = true;

    sqlite3_prepare_v2(getCommonDB().getDB(), query, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, assetId.c_str(), -1,SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        fprintf(stderr, "line %d: %s\n", __LINE__, sqlite3_errmsg(getCommonDB().getDB()));
        result = false;
    }
	else
	{
		std::string ip((char*)sqlite3_column_text(stmt, 0));
		if(rtspIp != ip) result = false;
	}
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    return result;
}

