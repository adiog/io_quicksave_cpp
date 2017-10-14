CREATE TABLE meta (
    "meta_hash" text NOT NULL PRIMARY KEY,
    "author" text NULL,
    "created_at" text NULL,
    "icon" text NULL,
    "meta_type" text NULL,
    "modified_at" text NULL,
    "name" text NULL,
    "source_title" text NULL,
    "source_url" text NULL,
    "text" text NULL,
    "user_hash" text NOT NULL
);

CREATE TABLE file (
    "file_hash" text NOT NULL PRIMARY KEY,
    "filename" text NOT NULL,
    "filesize" integer NOT NULL,
    "meta_hash" text NOT NULL REFERENCES "meta" ("meta_hash"),
    "mimetype" text NOT NULL
);

CREATE TABLE action (
    "action_hash" text NOT NULL PRIMARY KEY,
    "kwargs" text NOT NULL,
    "meta_hash" text NOT NULL REFERENCES "meta" ("meta_hash"),
    "name" text NOT NULL
);

CREATE TABLE tag (
    "tag_hash" text NOT NULL PRIMARY KEY,
    "meta_hash" text NOT NULL REFERENCES "meta" ("meta_hash"),
    "name" text NULL,
    "user_hash" text NOT NULL,
    "value" text NULL
);

CREATE TABLE key (
    "key_hash" text NOT NULL PRIMARY KEY,
    "name" text NOT NULL,
    "user_hash" text NOT NULL,
    "value" text NOT NULL
);

CREATE TABLE perspective (
    "perspective_hash" text NOT NULL PRIMARY KEY,
    "defined_query" text NOT NULL,
    "name" text NOT NULL,
    "ordering" text NULL,
    "refined_query" text NULL,
    "user_hash" text NOT NULL
);

