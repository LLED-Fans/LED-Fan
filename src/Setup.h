#ifndef LED_FAN_SETUP_H
#define LED_FAN_SETUP_H

#define LED_FAN_XSTR(s) LED_FAN_STR(s)
#define LED_FAN_STR(s) #s

#define IS_DEFINE_EMPTY(DEF) (7-DEF-7 == 14)

#include "DefaultSetup.h"

#if defined(LED_FAN_SETUP_FILE) && !IS_DEFINE_EMPTY(LED_FAN_SETUP_FILE)
#include LED_FAN_XSTR(builds/LED_FAN_SETUP_FILE.h)
#endif

#endif //LED_FAN_SETUP_H
