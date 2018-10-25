
#pragma once

#define PATCH_MATE_LITE 1

namespace patch_mate {
    enum {
#ifdef PATCH_MATE_LITE
        MAX_LOOP = 2,
#else
        MAX_LOOP = 8,
#endif
        MAX_PROGRAMS = 128,
    };
}
