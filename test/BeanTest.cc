#include <bean/ItemBean.h>
#include <databaseBean/DatabaseBeanItem.h>
#include <bean/ContentBean.h>
#include <bean/RetrieveResponseBean.h>
#include <gtest/gtest.h>

TEST(BeanTestSuite, BeanTestCase)
{
/*    const char *json = "{\"item_id\": 123, \"freetext\": \"blabla\", \"user_id\": 541}";
    auto bean = ItemBean(json);
    ASSERT_EQ(*bean.item_id, 123);
    ASSERT_EQ(*bean.freetext, "blabla");
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    bean.Serialize(writer);

    const char *json_re = "{\"items\": [{\"item_id\": 123, \"freetext\": \"blabla\", \"user_id\": 541}], \"total\": 55}";
    auto bb = RetrieveResponseBean(json_re);
    std::cout << *((*bb.items)[0].item->item_id) << std::endl;
    std::cout << s.GetString() << std::endl;
    std::cout << std::string(serialize(bb)) << std::endl;
*/
 }