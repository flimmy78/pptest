
#include "demuxer/ts/ts_index_generate.h"

using namespace nml_demuxer::ts;

#if defined(OS_WIN)
#pragma comment(lib, "nml.lib")
#endif


void displayUsage(void)
{
    puts("Usage:\t ts_indexer <input filepath> ");

    puts("\nExample: ts_indexer /home/alticast_media/kt_ad.ts");
    puts("\t will generate file /home/alticast_media/kt_ad.ts.index\n");
}


int main(int argc, char* argv[])
{
    TsIndexGenerate ts_indexer;

    puts  ("TS Indexer : ts_indexer");
    printf("NonmadConnection Version : %s \n", "0.92");
    puts  ("====================================\n");


    if (argc == 1)
    {
        displayUsage();
        return EXIT_SUCCESS;
    }


    std::string input_file(argv[1]);
    if (true == ts_indexer.tsIndexer(input_file, true) )
    {
        float duration = ts_indexer.getDurationOfMovie();
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}