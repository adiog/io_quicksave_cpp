#include <Python.h>
#include <gtest/gtest.h>
#include "PythonBeanAPI.h"
#include <qsgen/bean/MetaBean.h>
#include <qsgen/bean/ItemBean.h>
#include <timer>
#include <qsgen/bean/InternalCreateRequestBean.h>


TEST(PyEngineTestSuite, PureEmbedding)
{
    MEASURE;
    MetaBean metaBean;
    metaBean.meta_hash = "sha";
    InternalCreateRequestBean internalCreateRequestBean;
    ItemBean itemBean = PythonBeanAPI::call<InternalCreateRequestBean, ItemBean>("process", internalCreateRequestBean);
    std::cout << itemBean.to_string() << std::endl;
}