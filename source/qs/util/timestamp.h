// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>

#include <absl/time/clock.h>
#include <absl/time/time.h>


namespace qs {
namespace util {

struct Timestamp
{
    static std::string timestamp()
    {
        absl::TimeZone timeZone = absl::UTCTimeZone();
        absl::Time time = absl::Now();

        return absl::FormatTime("%Y/%m/%d %H:%M:%E3S", time, timeZone);
    }
};
}
}
