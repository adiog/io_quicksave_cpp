# -*- coding: utf-8 -*-

"""
This file is a part of quicksave project.
Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.
"""

import os
import queue
from queue import Queue

from locust import HttpLocust, TaskSet
from locust import task
import json
import random

import env
import requests

user_min = env.IO_QUICKSAVE_LOCUST_USER_MIN
user_max = env.IO_QUICKSAVE_LOCUST_USER_MAX


def random_user():
    u_int = random.randint(user_min, user_max)
    username = 'u' + str(u_int)
    password = 'p' + str(u_int)
    return username, password


tags = ['white', 'blue', 'black', 'red', 'green']

authors = ['adiog', 'locust', 'unknown', 'rogers', 'williams']

meta_type = ['image', 'selection', 'page', 'text', 'screenshot']

source_title = ['random', 'blabla', 'locusttitle']

source_url = ['https://www.youtube.com/watch?v=I0AxrOUJ62E', 'https://en.wikipedia.org/wiki/Main_Page', 'localhost', 'https://github.com/adiog/nihcl', 'unknown', '']

class UserBehavior(TaskSet):
    def do_login(self):
        username, password = random_user()
        response = requests.post(env.HTTPS_OAUTH_QUICKSAVE_IO + '/token/get',
                                 data=json.dumps({'expireTime': 300}),
                                 auth=(username, password),
                                 verify=False)
        token = json.loads(response.content.decode())['token']
        self.client.post("/token/put", data=json.dumps({'token': token}))

    def on_start(self):
        self.client.verify = False
        self.tags = Queue()
        self.metas = Queue()
        self.meta_hashes = set()
        self.do_login()

    @task(5)
    def check(self):
        self.client.get("/session/check")

    @task(3)
    def retrieve_all(self):
        self.client.post("/retrieve", data=json.dumps({'query': "WHERE freetext match ''"}))

    @task(3)
    def retrieve_by_tag(self):
        self.client.post("/retrieve", data=json.dumps({'query': "WHERE has '%s'" % tags[random.randint(0,4)]}))

    @task(1)
    def create(self):
        meta = {
            "author": authors[random.randint(0,4)],
            "text": 'any text is ok',
            "meta_type": meta_type[random.randint(0,4)],
            "source_title": source_title[random.randint(0,2)],
            "name": source_title[random.randint(0,2)],
            "source_url": source_url[random.randint(0,4)],
        }
        meta = json.loads(self.client.post('/create', data=json.dumps({'meta': meta})).content.decode())['item']['meta$']
        self.metas.put(meta)
        self.meta_hashes.add(meta['meta_hash'])

    @task(3)
    def create_tag(self):
        try:
            meta = self.metas.get(block=False)
            tag = {
                "meta_hash": meta['meta_hash'],
                "name": tags[random.randint(0,4)]
            }
            message_with_hash = json.loads(self.client.post('/tag/create', data=json.dumps({'tag': tag})).content.decode())
            self.tags.put((message_with_hash['hash'], meta['meta_hash']))
            self.metas.put(meta)
        except queue.Empty:
            pass


    @task(3)
    def update_tag(self):
        try:
            tag_meta = self.tags.get(block=False)
            if tag_meta[1] in self.meta_hashes:
                tag = {
                    "meta_hash": tag_meta[1],
                    "tag_hash": tag_meta[0],
                    "name": tags[random.randint(0,4)]
                }
                self.client.post('/tag/update', data=json.dumps({'tag': tag}))
                self.tags.put(tag_meta)
        except queue.Empty:
            pass

    @task(1)
    def delete_tag(self):
        try:
            tag_meta = self.tags.get(block=False)
            if tag_meta[1] in self.meta_hashes:
                self.client.post('/tag/delete', data=json.dumps({'tag_hash': tag_meta[0]}))
        except queue.Empty:
            pass

    @task(3)
    def update_meta(self):
        try:
            meta = self.metas.get(block=False)
            meta['author'] = authors[random.randint(0,4)]
            meta['text'] = 'any text is ok'
            meta['meta_type'] = meta_type[random.randint(0,4)]
            meta['source_title'] = source_title[random.randint(0,2)]
            meta['name'] = source_title[random.randint(0,2)]
            meta['source_url'] = source_url[random.randint(0,4)]
            self.client.post('/meta/update', data=json.dumps({'meta': meta}))
            self.metas.put(meta)
        except queue.Empty:
            pass

    @task(1)
    def delete_meta(self):
        try:
            meta = self.metas.get(block=False)
            self.client.post('/meta/delete', data=json.dumps({'meta_hash': meta['meta_hash']}))
        except queue.Empty:
            pass



class WebsiteUser(HttpLocust):
    task_set = UserBehavior
    min_wait = 100
    max_wait = 1000
