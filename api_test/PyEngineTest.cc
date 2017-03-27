#include <Python.h>
#include <gtest/gtest.h>
#include "PythonBeanAPI.h"
#include <bean/MetaBean.h>
#include <bean/ItemBean.h>
#include <timer>
/*
TEST(PyEngineTestSuite, SimpleTestCase)
{
    const char *argv[] = {"pyenginetest", "PyEngine", "foo", "cc"};
    int argc = 3;
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  // optional but recommended
    Py_Initialize();
    PyRun_SimpleString("from time import time,ctime\n"
                               "print('Today is', ctime(time()))\n");
    Py_Finalize();
    PyMem_RawFree(program);
}

*/


TEST(PyEngineTestSuite, PureEmbedding)
{
    MEASURE;
    MetaBean metaBean;
    metaBean.meta_hash = "sha";
    ItemBean itemBean = PythonBeanAPI::call<MetaBean, ItemBean>("process", metaBean);
    std::cout << itemBean.to_string() << std::endl;
}