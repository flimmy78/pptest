
#include <stdio.h>
#include "asset_db.h"
#include "StringUtil.h"
#include "TimeUtil.h"

int main(int argc, char* argv[])
{
	checkDB();
	
	int count = 10000;
	unsigned long long __time = GW::getTimeMS();
	getCommonDB().begin();
		
	for(int i=0; i<count; i++)
	{
		//printf("count : %d\n", i);
		if(false == getSystemDB().insertOrUpdateRecord("1.111.111.1." + intToString(i), 1))
		{
			printf("system insert fail\n");
			break;
			return EXIT_FAILURE;
		}
		
//		if(false == getSystemDB().deleteRecordWithKey("1.111.111.1.19990"))
//		{
//			printf("system delete fail\n");
//			break;
//			return EXIT_FAILURE;
//		}
	}
	
	getCommonDB().commit();

	__time = GW::getTimeMS() - __time;
	printf("count(%d), time(%llu)\n", count, __time);

	
//	for(int i=0; i<1000; i++)
//	{
//		printf("count : %d\n", i);
//		//SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0002222222222 RTSP/1.0
//		// asset 추가
//		if(false == asset_db.insertOrUpdateRecord("ASSETID0002222222222" + intToString(i)))
//		{
//			printf("asset insert fail\n");
//			return EXIT_FAILURE;
//		}
//
//		if(false == asset_db.deleteRecordWithKey("ASSETID0002222222222"))
//		{
//			printf("asset delete fail\n");
//			return EXIT_FAILURE;
//		}
//	}

//    //SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0002222222222/system/10.20.0.132 RTSP/1.0
//    // system file  추가.
//    if (false == asset_db.insertOrUpdateRecord("ASSETID0002222222222",
//            "/home/alticast/test",
//            "test.ts",
//            "10.20.0.132:18000"))
//        return EXIT_FAILURE;
//
//
//    //system file 추가후 index file 생성.
//    //얻어온 duration 추가
//    if (false == asset_db.insertOrUpdateRecord("ASSETID0002222222222",
//            "/home/alticast/test",
//            "test.ts",
//            "10.20.0.132:18000",
//            199.456))
//
//        return EXIT_FAILURE;
//
//
//    // virtual path 추가.
//    // SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0002222222222/vid//test/1111/test.ts RTSP/1.0
//    // validTime: 0
//    VidInfoDB vid_db;
//    vid_db.dbCheck();
//    if (false == vid_db.insertOrUpdateRecord("/test/1111",
//            "test.ts",
//            "ASSETID0002222222222",
//            0.0) )
//    {
//        return EXIT_FAILURE;
//    }
//
//    // virtual path 추가.
//    // SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0002222222222/vid//test/2222/test.ts RTSP/1.0
//    // validTime: 0
//    if (false == vid_db.insertOrUpdateRecord("/test/2222",
//            "test.ts",
//            "ASSETID0002222222222",
//            0.0) )
//    {
//        return EXIT_FAILURE;
//    }
//
//    // token 추가.
//    // SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0002222222222/token/ABCDEFG RTSP/1.0
//    // contentId: CONTENTID222222222222
//    // validTime: 0
//    TokenDB token_db;
//    token_db.dbCheck();
//    if (false == token_db.insertOrUpdateRecord("ABCDEFG",
//            "ASSETID0002222222222",
//            "CONTENTID222222222222",
//            0.0) )
//    {
//        return EXIT_FAILURE;
//    }
//
//    // asset 목록
//    // GET_PARAMETER rtsp://10.20.0.132:18001/api0/assets RTSP/1.0
//    std::vector<std::string> assets = asset_db.getAllRecords();
//
//
//    //asset 추가 , 삭제..
//    //SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0001111111111 RTSP/1.0
//    if (false == asset_db.insertOrUpdateRecord("ASSETID0001111111111"))
//        return EXIT_FAILURE;
//
//    //useFlag: 0
//    asset_db.deleteRecordWithKey("ASSETID0001111111111");
//
//
//    //system file 추가,삭제
//    //SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0001111111111/system/10.20.0.132 RTSP/1.0
//    //filePath: /test/test
//    //fileName: test.ts
//
//    if (false == asset_db.insertOrUpdateRecord("ASSETID0001111111111",
//            "/test/test",
//            "test.ts",
//            "10.20.0.132:18000"))
//        return EXIT_FAILURE;
//
//    //SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0001111111111/system/10.20.0.132 RTSP/1.0
//    //useFlag: 0
//    asset_db.deleteRecordSystemFile("ASSETID0001111111111");
//
//    //VID 추가 삭제
//    //SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0001111111111/vid//test/test/test.ts RTSP/1.0
//    if (false == vid_db.insertOrUpdateRecord("/test/test",
//            "test.ts",
//            "ASSETID0001111111111",
//            0.0) )
//    {
//        return EXIT_FAILURE;
//    }
//
//    //useFlag: 0
//    if (false == vid_db.deleteRecord("/test/test",
//                                     "test.ts",
//                                     "ASSETID0001111111111") )
//    {
//        return EXIT_FAILURE;
//    }
//
//
//    //token 추가 , 삭제
//    //SET_PARAMETER rtsp://10.20.0.132:18001/api0/asset/ASSETID0001111111111/token/ABCDEFG RTSP/1.0
//    //contentId: CONTENTID11111111111
//    //validTime: 0
//    if (false == token_db.insertOrUpdateRecord("ABCDEFG",
//            "ASSETID0001111111111",
//            "CONTENTID222222222222",
//            0.0) )
//    {
//        return EXIT_FAILURE;
//    }
//
//    //useFlag: 0
//    if (false == token_db.deleteRecordWithKey("ABCDEFG") )
//        return EXIT_FAILURE;

	printf("asset db test success\n");
    return EXIT_SUCCESS;
}