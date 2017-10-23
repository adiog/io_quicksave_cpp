// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <qs/audit/Audit.h>


TEST(AuditTestSuite, TestCase) {
    AUDIT_FATAL << "audit unit test";
    EXPECT_FALSE(false);
}
