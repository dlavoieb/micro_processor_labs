#include "display.h"
#ifndef MAIN_H__
#define MAIN_H__

typedef enum DisplayMode_T
{
    TEMPERATURE,
    ANGLE
};

typedef struct AppState{
    DisplayMode_T display_state;
    DisplayUnits temp_unit;
    float target_pitch;
    float target_roll;
};


#endif //!MAIN_H__