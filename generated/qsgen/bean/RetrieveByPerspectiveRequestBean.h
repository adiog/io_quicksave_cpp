// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef RETRIEVEBYPERSPECTIVEREQUESTBEAN_H
#define RETRIEVEBYPERSPECTIVEREQUESTBEAN_H

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


class RetrieveByPerspectiveRequestBean
{
public:
    RetrieveByPerspectiveRequestBean() = default;

    RetrieveByPerspectiveRequestBean(const char* json)
            : RetrieveByPerspectiveRequestBean(rapidjson::Document{}.Parse(json))
    {
    }

    RetrieveByPerspectiveRequestBean(absl::optional<int> limit, absl::optional<int> offset, std::string perspective_hash)
            : limit(limit)
            , offset(offset)
            , perspective_hash(perspective_hash)
    {
    }

    RetrieveByPerspectiveRequestBean(const rapidjson::Value& value)
    {
        if (value.HasMember(limit_label)) this->limit = absl::make_optional<int>(Typoid<int>::FromValue(value[limit_label]));
        if (value.HasMember(offset_label)) this->offset = absl::make_optional<int>(Typoid<int>::FromValue(value[offset_label]));
        if (value.HasMember(perspective_hash_label))
            this->perspective_hash = Typoid<std::string>::FromValue(value[perspective_hash_label]);
        else
            throw(missing_mandatory_field(perspective_hash_label));
    }

    void update(RetrieveByPerspectiveRequestBean bean)
    {
        if (bean.limit) limit = bean.limit;
        if (bean.offset) offset = bean.offset;
        perspective_hash = bean.perspective_hash;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        if (limit) writer.String(limit_label);
        if (limit) Typoid<int>::Serialize(*limit, writer);
        if (offset) writer.String(offset_label);
        if (offset) Typoid<int>::Serialize(*offset, writer);
        writer.String(perspective_hash_label);
        Typoid<std::string>::Serialize(perspective_hash, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const RetrieveByPerspectiveRequestBean& bean)
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

    const char* __name__ = "RetrieveByPerspectiveRequestBean";
    absl::optional<int> limit;
    const char* limit_label = "limit";
    absl::optional<int> offset;
    const char* offset_label = "offset";
    std::string perspective_hash;
    const char* perspective_hash_label = "perspective_hash";
};

#endif
