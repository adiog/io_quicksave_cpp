// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <qsgen/abi/PerspectiveBean.h>


namespace qs {

class PerspectiveCreateRequestBean
{
public:
    PerspectiveCreateRequestBean() = default;

    PerspectiveCreateRequestBean(const char* json)
            : PerspectiveCreateRequestBean(rapidjson::Document{}.Parse(json))
    {
    }

    PerspectiveCreateRequestBean(PerspectiveBean perspective)
            : perspective(perspective)
    {
    }

    PerspectiveCreateRequestBean(const rapidjson::Value& value)
    {
        if (value.HasMember(perspective_label))
            this->perspective = Typoid<PerspectiveBean>::FromValue(value[perspective_label]);
        else
            throw(missing_mandatory_field(perspective_label));
    }

    void update(PerspectiveCreateRequestBean bean)
    {
        perspective = bean.perspective;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(perspective_label);
        Typoid<PerspectiveBean>::Serialize(perspective, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const PerspectiveCreateRequestBean& bean)
    {
        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        bean.Serialize(writer);
        os << s.GetString();
        return os;
    }

    std::string to_string() const
    {
        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        Serialize(writer);
        return s.GetString();
    }

    operator std::unique_ptr<folly::IOBuf>() const { return folly::IOBuf::copyBuffer(::serialize(*this)); }

    const char* __name__ = "PerspectiveCreateRequestBean";
    PerspectiveBean perspective;
    const char* perspective_label = "perspective";
};
}
