#include <Python.h>
#include <gtest/gtest.h>
#include "PythonBeanAPI.h"
#include <bean/MetaBean.h>
#include <bean/ItemBean.h>
#include <timer>
#include <bean/InternalCreateRequestBean.h>


TEST(PyEngineTestSuite, PureEmbedding)
{
    MEASURE;
    MetaBean metaBean;
    metaBean.meta_hash = "sha";
    InternalCreateRequestBean internalCreateRequestBean;
    ItemBean itemBean = PythonBeanAPI::call<InternalCreateRequestBean, ItemBean>("process", internalCreateRequestBean);
    std::cout << itemBean.to_string() << std::endl;
}