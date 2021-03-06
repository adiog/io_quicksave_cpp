// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <qsgen/abi/CreateRequestBean.h>
#include <qsgen/abi/KeyBean.h>


namespace qs {

class InternalCreateRequestBean
{
public:
    InternalCreateRequestBean() = default;

    InternalCreateRequestBean(const char* json)
            : InternalCreateRequestBean(rapidjson::Document{}.Parse(json))
    {
    }

    InternalCreateRequestBean(CreateRequestBean createRequest, std::string databaseConnectionString, List<KeyBean> keys, std::string storageConnectionString)
            : createRequest(createRequest)
            , databaseConnectionString(databaseConnectionString)
            , keys(keys)
            , storageConnectionString(storageConnectionString)
    {
    }

    InternalCreateRequestBean(const rapidjson::Value& value)
    {
        if (value.HasMember(createRequest_label))
            this->createRequest = Typoid<CreateRequestBean>::FromValue(value[createRequest_label]);
        else
            throw(missing_mandatory_field(createRequest_label));
        if (value.HasMember(databaseConnectionString_label))
            this->databaseConnectionString = Typoid<std::string>::FromValue(value[databaseConnectionString_label]);
        else
            throw(missing_mandatory_field(databaseConnectionString_label));
        if (value.HasMember(keys_label))
            this->keys = Typoid<List<KeyBean>>::FromValue(value[keys_label]);
        else
            throw(missing_mandatory_field(keys_label));
        if (value.HasMember(storageConnectionString_label))
            this->storageConnectionString = Typoid<std::string>::FromValue(value[storageConnectionString_label]);
        else
            throw(missing_mandatory_field(storageConnectionString_label));
    }

    void update(InternalCreateRequestBean bean)
    {
        createRequest = bean.createRequest;
        databaseConnectionString = bean.databaseConnectionString;
        keys = bean.keys;
        storageConnectionString = bean.storageConnectionString;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(createRequest_label);
        Typoid<CreateRequestBean>::Serialize(createRequest, writer);
        writer.String(databaseConnectionString_label);
        Typoid<std::string>::Serialize(databaseConnectionString, writer);
        writer.String(keys_label);
        Typoid<List<KeyBean>>::Serialize(keys, writer);
        writer.String(storageConnectionString_label);
        Typoid<std::string>::Serialize(storageConnectionString, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const InternalCreateRequestBean& bean)
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

    const char* __name__ = "InternalCreateRequestBean";
    CreateRequestBean createRequest;
    const char* createRequest_label = "createRequest";
    std::string databaseConnectionString;
    const char* databaseConnectionString_label = "databaseConnectionString";
    List<KeyBean> keys;
    const char* keys_label = "keys";
    std::string storageConnectionString;
    const char* storageConnectionString_label = "storageConnectionString";
};
}
