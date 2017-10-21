// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef TOKENBEAN_H
#define TOKENBEAN_H

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


class TokenBean
{
public:
    TokenBean() = default;

    TokenBean(const char* json)
            : TokenBean(rapidjson::Document{}.Parse(json))
    {
    }

    TokenBean(std::string token)
            : token(token)
    {
    }

    TokenBean(const rapidjson::Value& value)
    {
        if (value.HasMember(token_label))
            this->token = Typoid<std::string>::FromValue(value[token_label]);
        else
            throw(missing_mandatory_field(token_label));
    }

    void update(TokenBean bean)
    {
        token = bean.token;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(token_label);
        Typoid<std::string>::Serialize(token, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const TokenBean& bean)
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

    const char* __name__ = "TokenBean";
    std::string token;
    const char* token_label = "token";
};

#endif
