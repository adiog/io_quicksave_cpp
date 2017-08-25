#include <bean/MetaBean.h>
#include <databaseBean/DatabaseBeans.h>
#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <bean/UserBean.h>
#include <string_view>

TEST(BeanTestSuite, BeanTestCase)
{
    std::string conStr = "file://path";
    std::string proto = "file://";
    std::string_view view = {&conStr[0], proto.size()};
    ASSERT_TRUE(view == proto);
}