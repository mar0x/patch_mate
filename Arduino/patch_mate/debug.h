
#pragma once

#define DEBUG 1

#if defined(DEBUG)

#if defined(HAVE_HWSERIAL1)

uint8_t debug_level_ = 0;

template<typename T1> void debug(uint8_t l, const T1& a1) {
    if (l <= debug_level_) {
        Serial.println(a1);
    }
}

template<typename T1, typename ...Args> void debug(uint8_t l, const T1& a1, Args... args) {
    if (l <= debug_level_) {
        Serial.print(a1);
        debug(l, args...);
    }
}

#else

#undef DEBUG

#endif

#endif


#if !defined(DEBUG)

#define debug(...)

#endif
