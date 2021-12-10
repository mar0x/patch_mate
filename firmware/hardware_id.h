
#pragma once

namespace patch_mate {

struct hardware_id_t {
    char buf[10];

    operator char*() { return buf; }
    operator const char*() const { return buf; }
};

}
