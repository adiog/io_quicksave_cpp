// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <qs/util/uuid.h>


TEST(UuidTestSuite, TestCase) {
    auto uuid = UUID::get();
    std::cout << uuid << std::endl;
}
