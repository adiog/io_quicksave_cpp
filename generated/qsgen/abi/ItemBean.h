// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <qsgen/abi/ActionBean.h>
#include <qsgen/abi/FileBean.h>
#include <qsgen/abi/MetaBean.h>
#include <qsgen/abi/TagBean.h>


namespace qs {

class ItemBean
{
public:
    ItemBean() = default;

    ItemBean(const char* json)
            : ItemBean(rapidjson::Document{}.Parse(json))
    {
    }

    ItemBean(List<ActionBean> actions, List<FileBean> files, MetaBean meta, List<TagBean> tags)
            : actions(actions)
            , files(files)
            , meta(meta)
            , tags(tags)
    {
    }

    ItemBean(const rapidjson::Value& value)
    {
        if (value.HasMember(actions_label))
            this->actions = Typoid<List<ActionBean>>::FromValue(value[actions_label]);
        else
            throw(missing_mandatory_field(actions_label));
        if (value.HasMember(files_label))
            this->files = Typoid<List<FileBean>>::FromValue(value[files_label]);
        else
            throw(missing_mandatory_field(files_label));
        if (value.HasMember(meta_label))
            this->meta = Typoid<MetaBean>::FromValue(value[meta_label]);
        else
            throw(missing_mandatory_field(meta_label));
        if (value.HasMember(tags_label))
            this->tags = Typoid<List<TagBean>>::FromValue(value[tags_label]);
        else
            throw(missing_mandatory_field(tags_label));
    }

    void update(ItemBean bean)
    {
        actions = bean.actions;
        files = bean.files;
        meta = bean.meta;
        tags = bean.tags;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        writer.String(actions_label);
        Typoid<List<ActionBean>>::Serialize(actions, writer);
        writer.String(files_label);
        Typoid<List<FileBean>>::Serialize(files, writer);
        writer.String(meta_label);
        Typoid<MetaBean>::Serialize(meta, writer);
        writer.String(tags_label);
        Typoid<List<TagBean>>::Serialize(tags, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const ItemBean& bean)
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

    const char* __name__ = "ItemBean";
    List<ActionBean> actions;
    const char* actions_label = "actions";
    List<FileBean> files;
    const char* files_label = "files";
    MetaBean meta;
    const char* meta_label = "meta";
    List<TagBean> tags;
    const char* tags_label = "tags";
};
}