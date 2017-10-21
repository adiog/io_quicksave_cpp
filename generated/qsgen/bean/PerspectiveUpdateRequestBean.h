// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef PERSPECTIVEUPDATEREQUESTBEAN_H
#define PERSPECTIVEUPDATEREQUESTBEAN_H

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <qsgen/bean/PerspectiveBean.h>


class PerspectiveUpdateRequestBean
{
public:
    PerspectiveUpdateRequestBean() = default;

    PerspectiveUpdateRequestBean(const char* json)
            : PerspectiveUpdateRequestBean(rapidjson::Document{}.Parse(json))
    {
    }

    PerspectiveUpdateRequestBean(absl::optional<List<std::string>> ordering, PerspectiveBean perspective)
            : ordering(ordering)
            , perspective(perspective)
    {
    }

    PerspectiveUpdateRequestBean(const rapidjson::Value& value)
    {
        if (value.HasMember(ordering_label)) this->ordering = absl::make_optional<List<std::string>>(Typoid<List<std::string>>::FromValue(value[ordering_label]));
        if (value.HasMember(perspective_label))
            this->perspective = Typoid<PerspectiveBean>::FromValue(value[perspective_label]);
        else
            throw(missing_mandatory_field(perspective_label));
    }

    void update(PerspectiveUpdateRequestBean bean)
    {
        if (bean.ordering) ordering = bean.ordering;
        perspective = bean.perspective;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        if (ordering) writer.String(ordering_label);
        if (ordering) Typoid<List<std::string>>::Serialize(*ordering, writer);
        writer.String(perspective_label);
        Typoid<PerspectiveBean>::Serialize(perspective, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const PerspectiveUpdateRequestBean& bean)
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

    const char* __name__ = "PerspectiveUpdateRequestBean";
    absl::optional<List<std::string>> ordering;
    const char* ordering_label = "ordering";
    PerspectiveBean perspective;
    const char* perspective_label = "perspective";
};

#endif
