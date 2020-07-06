
#pragma once

#include "../artl/button.h"

namespace patch_mate {

struct default_button_time_traits {
    static constexpr unsigned long bounce() { return 0; }
    static constexpr unsigned long hold() { return 500; }
};


template<typename HANDLERS>
struct button : public artl::button<HANDLERS, default_button_time_traits> {
    using base = artl::button<HANDLERS, default_button_time_traits>;
    using base::base;
};

}
