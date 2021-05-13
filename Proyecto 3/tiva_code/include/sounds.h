#include <Arduino.h>

#ifndef __SOUNDS__
#define __SOUNDS__

#define C4 262
#define C4s 277
#define D4 294
#define D4s 311
#define E4 330
#define F4 349
#define F4s 370
#define G4 392
#define G4s 415
#define A4 440
#define A4s 466
#define B4 494
#define C5 523
#define C5s 554
#define D5 587
#define D5s 622
#define E5 659
#define F5 698
#define F5s 740
#define G5 784
#define G5s 831
#define A5 880
#define A5s 932
#define B5 988
#define C6 1109

const int buzzer = PD_7;

int intro_notes[] =
{
    B4, 0, B5, 0, F5s, 0, D5s, 0, B5, F5s, 0, D5s, 0,
    C5, 0, C6, 0, G5, 0, E5, 0, C6, G5, 0, E5, 0,
    B4, 0, B5, 0, F5s, 0, D5s, 0, B5, F5s, 0, D5s, 0,
    D5s, E5, F5, 0, F5, F5s, G5, 0, G5, G5s, A5, 0, B5, 0,
};

int intro_durations[] =
{
    70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 210, 70,
    70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 210, 70,
    70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 210, 70,
    70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 140,
};

        
int paco_notes[] = { E4, A4s, D5s, F5s, A5, 0, B5, G5s, F5, C5s, G4s};
int paco_durations[] = {18, 18, 18, 18, 18, 53, 18, 18, 18, 18, 18};


int death_notes[] =
{
    E5, F5, F5s, D5s, E5, D5, D5s, C5, C5s, B4, C5, 0,
    G4s, 0, F4s, 0, F4s
};

int death_durations[] =
{
    80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 125, 
    80, 125, 125, 125, 125
};

void play_note(int notes[], int durations[], unsigned int index, float speed)
{
    float wait = durations[index] / speed;

    if (notes[index] != 0)
    {
        tone(buzzer, notes[index], wait); 
    }
    else
    {
        noTone(buzzer);
    }
    
    delay(wait);
}

void play_sound (int notes[], int durations[], int sizenotes)
{
    const int totalNotes = sizenotes / sizeof(int);

    for (int i = 0; i < totalNotes; i++)
    {
        play_note(notes, durations, i, 1);
    }
}

// void loop()
// {
//     play_sound(intro_notes, intro_durations, sizeof(intro_notes));
//     play_sound(paco_notes, paco_durations, sizeof(paco_notes));
//     play_sound(death_notes, death_durations, sizeof(death_notes));
//}

#endif // __SOUNDS__