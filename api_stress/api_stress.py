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

item_type = ['image', 'selection', 'page', 'text', 'screenshot']

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
        self.items = Queue()
        self.item_ids = set()
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
        item = {
            "author": authors[random.randint(0,4)],
            "freetext": 'any text is ok',
            "item_type": item_type[random.randint(0,4)],
            "source_title": source_title[random.randint(0,2)],
            "title": source_title[random.randint(0,2)],
            "source_url": source_url[random.randint(0,4)],
            "url": source_url[random.randint(0,4)]
        }
        item = json.loads(self.client.post('/create', data=json.dumps({'item': item})).content.decode())['item']
        self.items.put(item)
        self.item_ids.add(item['item_id'])

    @task(3)
    def create_tag(self):
        try:
            item = self.items.get(block=False)
            tag = {
                "item_id": item['item_id'],
                "name": tags[random.randint(0,4)]
            }
            message_with_id = json.loads(self.client.post('/tag/create', data=json.dumps({'tag': tag})).content.decode())
            self.tags.put((message_with_id['id'], item['item_id']))
            self.items.put(item)
        except queue.Empty:
            pass


    @task(3)
    def update_tag(self):
        try:
            tag_item = self.tags.get(block=False)
            if tag_item[1] in self.item_ids:
                tag = {
                    "item_id": tag_item[1],
                    "tag_id": tag_item[0],
                    "name": tags[random.randint(0,4)]
                }
                self.client.post('/tag/update', data=json.dumps({'tag': tag}))
                self.tags.put(tag_item)
        except queue.Empty:
            pass

    @task(1)
    def delete_tag(self):
        try:
            tag_item = self.tags.get(block=False)
            if tag_item[1] in self.item_ids:
                self.client.post('/tag/delete', data=json.dumps({'tag_id': tag_item[0]}))
        except queue.Empty:
            pass

    @task(3)
    def update_item(self):
        try:
            item = self.items.get(block=False)
            item['author'] = authors[random.randint(0,4)]
            item['freetext'] = 'any text is ok'
            item['item_type'] = item_type[random.randint(0,4)]
            item['source_title'] = source_title[random.randint(0,2)]
            item['title'] = source_title[random.randint(0,2)]
            item['source_url'] = source_url[random.randint(0,4)]
            item['url'] = source_url[random.randint(0,4)]
            self.client.post('/item/update', data=json.dumps({'item': item}))
            self.items.put(item)
        except queue.Empty:
            pass

    @task(1)
    def delete_item(self):
        try:
            item = self.items.get(block=False)
            self.client.post('/item/delete', data=json.dumps({'item_id': item['item_id']}))
        except queue.Empty:
            pass



class WebsiteUser(HttpLocust):
    task_set = UserBehavior
    min_wait = 100
    max_wait = 1000
