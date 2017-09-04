#include <gtest/gtest.h>
#include <bean/MetaBean.h>
#include <bean/ItemBean.h>
#include <timer>
#include <bean/InternalCreateRequestBean.h>
#include <fuse/StaticReadOnly.h>


TEST(StaticReadOnlyTestSuite, PureEmbedding)
{
    MEASURE;
    FS fs;
    fs.init();

    std::string username = "adiog";
    std::string password = "pass";
    StaticReadOnly::build(fs, username, password, "WHERE name ~ ''");
}