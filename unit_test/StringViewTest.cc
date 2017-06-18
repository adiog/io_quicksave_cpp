#include <bean/MetaBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <bean/UserBean.h>
#include <databaseBean/DatabaseBeanUser.h>
#include <string_view>

TEST(BeanTestSuite, BeanTestCase)
{
    std::string conStr = "file://path";
    std::string_view view = {&conStr[0], 157};
    std::string proto = "file://";
    ASSERT_EQ(view, proto);

}