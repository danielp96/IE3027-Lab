#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

#define START_PUSH PD_7

uint8_t player1 = 0;
uint8_t player2 = 0;

bool game_running = false;

void printA(uint8_t num);
void printB(uint8_t num);
void semaforo(void);

void setup()
{
    pinMode(PB_5, OUTPUT);
    pinMode(PB_0, OUTPUT);
    pinMode(PB_1, OUTPUT);
    pinMode(PE_4, OUTPUT);
    pinMode(PE_5, OUTPUT);
    pinMode(PB_4, OUTPUT);
    pinMode(PA_5, OUTPUT);
    pinMode(PA_6, OUTPUT);

    pinMode(PB_3, OUTPUT);
    pinMode(PC_4, OUTPUT);
    pinMode(PC_5, OUTPUT);
    pinMode(PC_6, OUTPUT);

    pinMode(PB_6, OUTPUT);
    pinMode(PA_4, OUTPUT);
    pinMode(PA_3, OUTPUT);
    pinMode(PA_2, OUTPUT);

    pinMode(START_PUSH, INPUT_PULLDOWN);
    pinMode(PUSH1, INPUT_PULLUP);
    pinMode(PUSH2, INPUT_PULLUP);

    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
}

void loop()
{
 
    if (!game_running && digitalRead(START_PUSH))
    {
        game_running = true;
        player1 = 0;
        player2 = 0;

        printA(player1);
        printB(player2);
        semaforo();
    }

    if (game_running && !digitalRead(PUSH1))
    {
        player1 = !player1 ? 1 : player1 << 1;
    }

    if (game_running && !digitalRead(PUSH2))
    {
        player2 = !player2 ? 1 : player2 << 1;
    }


    if ((0x80 == player1) || (0x80 == player2))
    {
        game_running = false;
        
        player1 = 0x80 == player1 ? 0xFF : 0;
        player2 = 0x80 == player2 ? 0xFF : 0;
    }

    printA(player1);
    printB(player2);
    delay(175);

}

void semaforo(void)
{
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    delay(1000);

    digitalWrite(GREEN_LED, HIGH);
    delay(1000);

    digitalWrite(RED_LED, LOW);
}

void printA(uint8_t num)
{
    digitalWrite(PB_5, (num >> 7) & 1);
    digitalWrite(PB_0, (num >> 6) & 1);
    digitalWrite(PB_1, (num >> 5) & 1);
    digitalWrite(PE_4, (num >> 4) & 1);
    digitalWrite(PE_5, (num >> 3) & 1);
    digitalWrite(PB_4, (num >> 2) & 1);
    digitalWrite(PA_5, (num >> 1) & 1);
    digitalWrite(PA_6, (num >> 0) & 1);
}


void printB(uint8_t num)
{
    digitalWrite(PB_3, (num >> 7) & 1);
    digitalWrite(PC_4, (num >> 6) & 1);
    digitalWrite(PC_5, (num >> 5) & 1);
    digitalWrite(PC_6, (num >> 4) & 1);
    digitalWrite(PB_6, (num >> 3) & 1);
    digitalWrite(PA_4, (num >> 2) & 1);
    digitalWrite(PA_3, (num >> 1) & 1);
    digitalWrite(PA_2, (num >> 0) & 1);
}