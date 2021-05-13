#ifndef __HARDWARE_ABSTRACTION_LAYER__
#define __HARDWARE_ABSTRACTION_LAYER__

#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

enum EventType {UP_1, DOWN_1, LEFT_1, RIGHT_1, SW1, SW2, UP_2, DOWN_2, LEFT_2, RIGHT_2};

typedef struct Events
{
    bool UP_1;
    bool DOWN_1;
    bool LEFT_1;
    bool RIGHT_1;
    bool SW1;
    bool SW2;
    bool UP_2;
    bool DOWN_2;
    bool LEFT_2;
    bool RIGHT_2;

} Event_struct;

Event_struct events;

// adjust deadzone as required
const unsigned int DEAD_ZONE = 300;
const unsigned int MID_POINT = 2048;

void input_init()
{
    pinMode(PF_4, INPUT_PULLUP);
    pinMode(PF_0, INPUT_PULLUP);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
}

void read_inputs()
{
    events.SW1 = !digitalRead(PF_4);
    events.SW2 = !digitalRead(PF_0);

    unsigned int temp_y1 = analogRead(A0);
    unsigned int temp_x1 = analogRead(A1);

    unsigned int temp_y2 = analogRead(A8);
    unsigned int temp_x2 = analogRead(A9);

    events.DOWN_1 = temp_y1 > (MID_POINT + DEAD_ZONE);
    events.UP_1   = temp_y1 < (MID_POINT - DEAD_ZONE);

    events.RIGHT_1 = temp_x1 > (MID_POINT + DEAD_ZONE);
    events.LEFT_1  = temp_x1 < (MID_POINT - DEAD_ZONE);


    events.DOWN_2 = temp_y2 > (MID_POINT + DEAD_ZONE);
    events.UP_2   = temp_y2 < (MID_POINT - DEAD_ZONE);

    events.RIGHT_2 = temp_x2 > (MID_POINT + DEAD_ZONE);
    events.LEFT_2  = temp_x2 < (MID_POINT - DEAD_ZONE);

}

bool get_event(EventType e)
{
    switch (e)
    {
        case UP_1:
            return events.UP_1;

        case DOWN_1:
            return events.DOWN_1;

        case LEFT_1:
            return events.LEFT_1;

        case RIGHT_1:
            return events.RIGHT_1;

        case SW1:
            return events.SW1;

        case SW2:
            return events.SW2;

        case UP_2:
            return events.UP_2;

        case DOWN_2:
            return events.DOWN_2;

        case LEFT_2:
            return events.LEFT_2;

        case RIGHT_2:
            return events.RIGHT_2;

        default:
            return false;
    }
}

// Extra slow
void hal_debug()
{
    Serial.print(" UP_1: ");
    Serial.print(events.UP_1);
    
    Serial.print(" DOWN_1: ");
    Serial.print(events.DOWN_1);

    Serial.print(" LEFT_1: ");
    Serial.print(events.LEFT_1);

    Serial.print(" RIGHT_1: ");
    Serial.print(events.RIGHT_1);

    Serial.print(" SW1: ");
    Serial.print(events.SW1);

    Serial.print(" SW2: ");
    Serial.print(events.SW2);

    Serial.print(" UP_2: ");
    Serial.print(events.UP_2);
    
    Serial.print(" DOWN_2: ");
    Serial.print(events.DOWN_2);

    Serial.print(" LEFT_2: ");
    Serial.print(events.LEFT_2);

    Serial.print(" RIGHT_2: ");
    Serial.print(events.RIGHT_2);

    Serial.println("");
}

#endif // __HARDWARE_ABSTRACTION_LAYER__