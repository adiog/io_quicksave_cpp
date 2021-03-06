// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace qs {

class TokenRequestBean
{
public:
    TokenRequestBean() = default;

    TokenRequestBean(const char* json)
            : TokenRequestBean(rapidjson::Document{}.Parse(json))
    {
    }

    TokenRequestBean(int expireTime)
            : expireTime(expireTime)
    {
    }

    TokenRequestBean(const rapidjson::Value& value)
    {
        if (value.HasMember(expireTime_label))
            this->expireTime = Typoid<int>::FromValue(value[expireTime_label]);
        else
            throw(missing_mandatory_field(expireTime_label));
    }

    void update(TokenRequestBean bean)
    {
        expireTime = bean.expireTime;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(expireTime_label);
        Typoid<int>::Serialize(expireTime, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const TokenRequestBean& bean)
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

    const char* __name__ = "TokenRequestBean";
    int expireTime;
    const char* expireTime_label = "expireTime";
};
}
