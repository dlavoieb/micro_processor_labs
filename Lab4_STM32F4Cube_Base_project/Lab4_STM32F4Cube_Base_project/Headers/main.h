#include "display.h"
#ifndef MAIN_H__
#define MAIN_H__

typedef enum DisplayMode_T
{
    TEMPERATURE,
    ANGLE
};

struct AppState{
    DisplayMode_T display_state;
    DisplayUnits temp_unit;
    uint16_t target_pitch;
    uint16_t target_roll;
};


#endif //!MAIN_H__