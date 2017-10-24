// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <timer>

#include <PythonBeanAPI.h>

#include <qsgen/abi/MetaBean.h>
#include <qsgen/abi/ItemBean.h>
#include <qsgen/abi/InternalCreateRequestBean.h>


TEST(PyEngineTestSuite, PureEmbedding)
{
    MEASURE;
    qs::MetaBean metaBean;
    metaBean.meta_hash = "sha";
    qs::InternalCreateRequestBean internalCreateRequestBean;
    qs::ItemBean itemBean = PythonBeanAPI::call<qs::InternalCreateRequestBean, qs::ItemBean>("process", internalCreateRequestBean);
    std::cout << itemBean.to_string() << std::endl;
}