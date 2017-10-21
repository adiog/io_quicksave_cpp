// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef BACKGROUNDTASKBEAN_H
#define BACKGROUNDTASKBEAN_H

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <qsgen/bean/InternalCreateRequestBean.h>


class BackgroundTaskBean
{
public:
    BackgroundTaskBean() = default;

    BackgroundTaskBean(const char* json)
            : BackgroundTaskBean(rapidjson::Document{}.Parse(json))
    {
    }

    BackgroundTaskBean(InternalCreateRequestBean internalCreateRequest, SerializedDict kwargs, std::string name)
            : internalCreateRequest(internalCreateRequest)
            , kwargs(kwargs)
            , name(name)
    {
    }

    BackgroundTaskBean(const rapidjson::Value& value)
    {
        if (value.HasMember(internalCreateRequest_label))
            this->internalCreateRequest = Typoid<InternalCreateRequestBean>::FromValue(value[internalCreateRequest_label]);
        else
            throw(missing_mandatory_field(internalCreateRequest_label));
        if (value.HasMember(kwargs_label))
            this->kwargs = Typoid<SerializedDict>::FromValue(value[kwargs_label]);
        else
            throw(missing_mandatory_field(kwargs_label));
        if (value.HasMember(name_label))
            this->name = Typoid<std::string>::FromValue(value[name_label]);
        else
            throw(missing_mandatory_field(name_label));
    }

    void update(BackgroundTaskBean bean)
    {
        internalCreateRequest = bean.internalCreateRequest;
        kwargs = bean.kwargs;
        name = bean.name;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(internalCreateRequest_label);
        Typoid<InternalCreateRequestBean>::Serialize(internalCreateRequest, writer);
        writer.String(kwargs_label);
        Typoid<SerializedDict>::Serialize(kwargs, writer);
        writer.String(name_label);
        Typoid<std::string>::Serialize(name, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const BackgroundTaskBean& bean)
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

    const char* __name__ = "BackgroundTaskBean";
    InternalCreateRequestBean internalCreateRequest;
    const char* internalCreateRequest_label = "internalCreateRequest";
    SerializedDict kwargs;
    const char* kwargs_label = "kwargs";
    std::string name;
    const char* name_label = "name";
};

#endif
