
#if defined(ARDUINO)

#include "controller.h"

void setup(void) {
    patch_mate::controller_.setup();
}

void loop(void) {
    patch_mate::controller_.loop();
}

#endif
