// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <PythonBeanAPI.h>
#include <qsgen/abi/InternalCreateRequestBean.h>
#include <qsgen/abi/ItemBean.h>

namespace qs {

class PluginEngine
{
public:
    static ItemBean process(InternalCreateRequestBean bean)
    {
        return PythonBeanAPI::call<InternalCreateRequestBean, ItemBean>("process", bean);
    }
};
}
