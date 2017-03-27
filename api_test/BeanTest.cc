#include <bean/MetaBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <gtest/gtest.h>
#include <SQLiteCpp/Database.h>
#include <bean/UserBean.h>
#include <databaseBean/DatabaseBeanUser.h>

TEST(BeanTestSuite, BeanTestCase)
{
    auto db = std::make_unique<SQLite::Database>("/io.quicksave.db/unittest.sqlite3", SQLite::OPEN_READWRITE);

    auto userBean = DatabaseBean<UserBean>::get_by(db.get(), "username", "adiog");

    MetaBean metaBean;
    metaBean.user_hash = userBean[0].user_hash;
    metaBean.name = "name";
    metaBean.text = "text";
    std::string meta_hash = DatabaseBean<MetaBean>::insert(db.get(), metaBean);

    MetaBean updateMetaBean;
    updateMetaBean.meta_hash = meta_hash;
    updateMetaBean.name = "new";
    DatabaseBean<MetaBean>::update(db.get(), updateMetaBean);

    auto readBean = DatabaseBean<MetaBean>::get(db.get(), meta_hash);
    std::cout << readBean->to_string() << std::endl;

    MetaBean overrideMetaBean;
    overrideMetaBean.meta_hash = meta_hash;
    overrideMetaBean.name = "new";
    overrideMetaBean.user_hash = userBean[0].user_hash;

    DatabaseBean<MetaBean>::override(db.get(), overrideMetaBean);

    auto readBean2 = DatabaseBean<MetaBean>::get(db.get(), meta_hash);
    std::cout << readBean2->to_string() << std::endl;
    if (!readBean->text)
        std::cout << "Success!!" << std::endl;

}