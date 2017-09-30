// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <uuid>
#include <qs/oauth/OAuthHelper.h>
#include <qs/oauth/OAuthMemcached.h>
#include <qsgen/bean/SessionBean.h>
#include <qsgen/bean/UserBean.h>
#include <util/format.h>


class OAuthAPI
{
public:
    using Token = std::string;

    static Token create_session(UserBean userBean, int expireTime)
    {
        SessionBean sessionBean;
        sessionBean.token = UUID::get();
        sessionBean.user = userBean;
        sessionBean.user.password = "";

        if (!OAuthMemcached::set(sessionBean.token, sessionBean.to_string(), expireTime))
        {
            throw std::runtime_error("");
        }

        LOG(INFO) << Format::format("Session                      : CREATED (%s,%s)", userBean.username.c_str(), sessionBean.token.c_str());
        return sessionBean.token;
    }

    static bool check_session(Token token)
    {
        return ((OAuthMemcached::get(token) != "") && OAuthMemcached::touch(token));
    }

    static SessionBean get_session(Token token)
    {
        return SessionBean(OAuthMemcached::get(token).c_str());
    }

    static bool destroy_session(Token token)
    {
        return OAuthMemcached::remove(token);
    }
};
