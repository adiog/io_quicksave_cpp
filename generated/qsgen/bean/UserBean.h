// This file is an AUTOGENERATED part of beans project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef USERBEAN_H
#define USERBEAN_H

#include <CppBeans.h>

#include <memory>
#include <folly/io/IOBuf.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


class UserBean
{
public:
    UserBean() = default;

    UserBean(const char* json)
            : UserBean(rapidjson::Document{}.Parse(json))
    {
    }

    UserBean(absl::optional<std::string> user_hash, std::string databaseConnectionString, std::string password, std::string storageConnectionString, std::string username)
            : user_hash(user_hash)
            , databaseConnectionString(databaseConnectionString)
            , password(password)
            , storageConnectionString(storageConnectionString)
            , username(username)
    {
    }

    UserBean(const rapidjson::Value& value)
    {
        if (value.HasMember(user_hash_label)) this->user_hash = absl::make_optional<std::string>(Typoid<std::string>::FromValue(value[user_hash_label]));
        if (value.HasMember(databaseConnectionString_label))
            this->databaseConnectionString = Typoid<std::string>::FromValue(value[databaseConnectionString_label]);
        else
            throw(missing_mandatory_field(databaseConnectionString_label));
        if (value.HasMember(password_label))
            this->password = Typoid<std::string>::FromValue(value[password_label]);
        else
            throw(missing_mandatory_field(password_label));
        if (value.HasMember(storageConnectionString_label))
            this->storageConnectionString = Typoid<std::string>::FromValue(value[storageConnectionString_label]);
        else
            throw(missing_mandatory_field(storageConnectionString_label));
        if (value.HasMember(username_label))
            this->username = Typoid<std::string>::FromValue(value[username_label]);
        else
            throw(missing_mandatory_field(username_label));
    }

    void update(UserBean bean)
    {
        if (bean.user_hash) user_hash = bean.user_hash;
        databaseConnectionString = bean.databaseConnectionString;
        password = bean.password;
        storageConnectionString = bean.storageConnectionString;
        username = bean.username;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const
    {
        writer.StartObject();
        if (user_hash) writer.String(user_hash_label);
        if (user_hash) Typoid<std::string>::Serialize(*user_hash, writer);
        writer.String(databaseConnectionString_label);
        Typoid<std::string>::Serialize(databaseConnectionString, writer);
        writer.String(password_label);
        Typoid<std::string>::Serialize(password, writer);
        writer.String(storageConnectionString_label);
        Typoid<std::string>::Serialize(storageConnectionString, writer);
        writer.String(username_label);
        Typoid<std::string>::Serialize(username, writer);
        writer.EndObject();
    }

    friend std::ostream& operator<<(std::ostream& os, const UserBean& bean)
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

    const char* __name__ = "UserBean";
    absl::optional<std::string> user_hash;
    const char* user_hash_label = "user_hash";
    std::string databaseConnectionString;
    const char* databaseConnectionString_label = "databaseConnectionString";
    std::string password;
    const char* password_label = "password";
    std::string storageConnectionString;
    const char* storageConnectionString_label = "storageConnectionString";
    std::string username;
    const char* username_label = "username";
};

#endif
