
#pragma once

#define DEBUG 1

/*
    1    set program
    1    set loop
    1    set mode

    2    rotate value
    2    toggle loop (on_down)

    3    set_dirty

    4    modi_cmd read byte
    4    modi_cmd write byte
    4    on_down
    4    on_hold
    4    on_rotate (encoder)

    4    snake start
    4    snake loose x2

    5    on_down (release key)
    5    snake turn left
    5    snake turn right

    6    loop_state read prog
    6    loop_state write prog

    7    in: piso value
    7    out: mute
    7    out: unmute
    7    out: commit sipo
    7    out: commit_led only
    7    program_title::setup x3
    7    program_title: ee_find (program)
    7    program_title: write prog
 */


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
