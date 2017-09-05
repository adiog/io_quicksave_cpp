// This file is a part of cpp-fuse project.
// Copyright (c) 2016 Aleksander Gajewski <adiog@brainfuck.pl>.

// Usage:
// $ mkdir mntpoint
// $ ./cpp-fuse mntpoint
// $ fusermount -u mntpoint

#define FUSE_USE_VERSION 30


#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "fuse/fuse.h"

#include <stdlib.h>
#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "FS.h"
#include "util/debug"
#include "StaticReadOnly.h"

static FS fs;
static fuse_operations cpp_fuse;

static int cpp_getattr(const char *path, struct stat *stbuf) {
    DEBUG(path);

    std::memset(stbuf, 0, sizeof(struct stat));

    if (fs.isDir(path)) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2 + fs.getDirSize(path);
    } else if (fs.isLink(path)) {
        stbuf->st_mode = S_IFLNK | 0777;
        stbuf->st_nlink = 1;
        stbuf->st_size = fs.getLinkSize(path);
    } else if (fs.isFile(path)) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = fs.getFileSize(path);
    } else {
        return -ENOENT;
    }

    return 0;
}

static int cpp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi) {
    DEBUG(path);

    (void)offset;
    (void)fi;
//    (void)flags;

    if (!fs.isDir(path)) {
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    auto dir_list = fs.getDir(path);
    for (auto &dir : dir_list) {
        dir.push_back('\0');
        filler(buf, &dir[0], NULL, 0);
    }

    return 0;
}

static int cpp_open(const char *path, struct fuse_file_info *fi) {
    DEBUG(path);

    if (!fs.isNode(path)) {
        return -ENOENT;
    }

    if ((fi->flags & 3) != O_RDONLY) {
        return -EACCES;
    }

    return 0;
}

static int cpp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    DEBUG(path);

    (void)fi;

    if (!fs.isFile(path)) {
        return -ENOENT;
    }

    size = fs.readFile(path, buf, size, offset);

    return static_cast<int>(size);
}

int cpp_readlink(const char *path, char *buf, size_t size) {
    DEBUG(path);

    if (!fs.isLink(path)) {
        return -ENOENT;
    }

    size = fs.readLink(path, buf, size);

    if (!size) {
        return -ENOENT;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cout << "Usage: ./qsfuse /path/to/mount username password \"query\"" << std::endl;
        return 1;
    }
    //////// CUT HERE
    ///////////////////////////////////////////////////////////////
    fs.init();

    std::string username = argv[2];
    std::string password = argv[3];
    std::string query = argv[4];

    StaticReadOnly::build(fs, username, password, query);
    //////// CUT HERE
    ///////////////////////////////////////////////////////////////

    cpp_fuse.getattr = cpp_getattr;
    cpp_fuse.readdir = cpp_readdir;
    cpp_fuse.readlink = cpp_readlink;
    cpp_fuse.open = cpp_open;
    cpp_fuse.read = cpp_read;

    return fuse_main(argc-3, argv, &cpp_fuse, NULL);
}
