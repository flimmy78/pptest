

#include "gmock/gmock.h"
#include "reader/file_reader.h"

namespace nml_reader
{


class File_Reader_Test : public testing::Test
{
protected:
    virtual void SetUp() OVERRIDE
    {
        testing::Test::SetUp();
    }

    virtual void TearDown() OVERRIDE
    {
        testing::Test::TearDown();
    }

};

TEST_F( File_Reader_Test, init_Test)
{
    File_Reader reader_;
}

} //namespace nml_reader