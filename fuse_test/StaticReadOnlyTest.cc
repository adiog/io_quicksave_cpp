#include <StaticReadOnly.h>
#include <timer>
#include <gtest/gtest.h>
#include <qsgen/bean/InternalCreateRequestBean.h>
#include <qsgen/bean/ItemBean.h>
#include <qsgen/bean/MetaBean.h>


TEST(StaticReadOnlyTestSuite, PureEmbedding)
{
    MEASURE;
    FS fs;
    fs.init();

    std::string username = "adiog";
    std::string password = "pass";
    StaticReadOnly::build(fs, username, password, "WHERE name ~ ''");
}