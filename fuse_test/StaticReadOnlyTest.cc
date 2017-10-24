#include <StaticReadOnly.h>
#include <timer>
#include <gtest/gtest.h>
#include <qsgen/abi/InternalCreateRequestBean.h>
#include <qsgen/abi/ItemBean.h>
#include <qsgen/abi/MetaBean.h>


TEST(StaticReadOnlyTestSuite, PureEmbedding)
{
    MEASURE;
    FS fs;
    fs.init();

    std::string username = "adiog";
    std::string password = "pass";
    StaticReadOnly::build(fs, username, password, "WHERE name ~ ''");
}