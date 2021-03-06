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

class PerspectiveRetrieveResponseBean
{
public:
    PerspectiveRetrieveResponseBean() = default;

    PerspectiveRetrieveResponseBean(const char* json)
            : PerspectiveRetrieveResponseBean(rapidjson::Document{}.Parse(json))
    {
    }

    PerspectiveRetrieveResponseBean(List<PerspectiveBean> perspectives)
            : perspectives(perspectives)
    {
    }

    PerspectiveRetrieveResponseBean(const rapidjson::Value& value)
    {
        if (value.HasMember(perspectives_label))
            this->perspectives = Typoid<List<PerspectiveBean>>::FromValue(value[perspectives_label]);
        else
            throw(missing_mandatory_field(perspectives_label));
    }

    void update(PerspectiveRetrieveResponseBean bean)
    {
        perspectives = bean.perspectives;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(perspectives_label);
        Typoid<List<PerspectiveBean>>::Serialize(perspectives, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const PerspectiveRetrieveResponseBean& bean)
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

    const char* __name__ = "PerspectiveRetrieveResponseBean";
    List<PerspectiveBean> perspectives;
    const char* perspectives_label = "perspectives";
};
}
