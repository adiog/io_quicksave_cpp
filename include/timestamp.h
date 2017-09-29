// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>
#include <absl/time/time.h>
#include <absl/time/clock.h>

std::string timestamp()
{
    absl::TimeZone timeZone = absl::UTCTimeZone();
    absl::Time time = absl::Now();

    return absl::FormatTime("%Y/%m/%d %H:%M:%E3S", time, timeZone);
}