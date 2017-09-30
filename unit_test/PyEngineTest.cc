// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <timer>

#include <PythonBeanAPI.h>
#include <qsgen/bean/MetaBean.h>
#include <qsgen/bean/ItemBean.h>
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