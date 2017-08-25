# -*- coding: utf-8 -*-

"""
This file is a part of quicksave project.
Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.
"""

import os

from locust import HttpLocust, TaskSet
from locust import task
import json
import random

import env

user_min = env.IO_QUICKSAVE_LOCUST_USER_MIN
user_max = env.IO_QUICKSAVE_LOCUST_USER_MAX


def random_user():
    u_int = random.randint(user_min, user_max)
    username = 'u' + str(u_int)
    password = 'p' + str(u_int)
    return username, password


class UserBehavior(TaskSet):
    def on_start(self):
        self.client.verify = False

    def do_login(self, username, password):
        response = self.client.post("/token/get",
                                   data=json.dumps({'expireTime': 300}),
                                   auth=(username, password))
        return json.loads(response.content.decode())['token']

    def do_logout(self, token):
        self.client.post("/token/delete", data=json.dumps({'token': token}))

    def do_ping(self, token):
        self.client.post("/token/check", data=json.dumps({'token': token}))

    @task()
    def login_ping_logout(self):
        username, password = random_user()
        token = self.do_login(username, password)
        self.do_ping(token)
        self.do_logout(token)


class WebsiteUser(HttpLocust):
    task_set = UserBehavior
    min_wait = 1000
    max_wait = 5000
