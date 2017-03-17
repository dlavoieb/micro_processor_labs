#ifndef MAIN_H__
#define MAIN_H__

#include "display.h"
#include "keypad.h"

typedef enum
{
    TEMPERATURE = 0,
    ANGLE
}DisplayMode_T;

struct AppState{
    DisplayMode_T display_state;
    DisplayUnits temp_unit;
		PitchOrRoll pitch_or_roll;
    uint16_t target_pitch;
    uint16_t target_roll;
		uint16_t temp_pitch;
    uint16_t temp_roll;
};


#endif //!MAIN_H__
