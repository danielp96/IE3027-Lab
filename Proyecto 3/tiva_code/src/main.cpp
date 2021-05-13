#include <Arduino.h> // platformio specific

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <ili9341.h>
#include <SPI.h>
#include <SD.h>

#include "hal.h"
#include "game_object.h"
#include "map.h"
#include "sounds.h"

#define LCD_WIDTH  320
#define LCD_HEIGHT 240

extern tImage pacman_side_1;
extern tImage pacman_side_2;
extern tImage pacman_up_1;
extern tImage pacman_up_2;
extern tImage pacman_down_1;
extern tImage pacman_down_2;

extern tImage pacman_death_0;
extern tImage pacman_death_1;
extern tImage pacman_death_2;
extern tImage pacman_death_3;
extern tImage pacman_death_4;
extern tImage pacman_death_5;
extern tImage pacman_death_6;
extern tImage pacman_death_7;
extern tImage pacman_death_8;
extern tImage pacman_death_9;
extern tImage pacman_death_10;
extern tImage pacman_death_11;
extern tImage pacman_death_12;

extern tImage greeny_side_1;
extern tImage greeny_side_2;
extern tImage greeny_up_1;
extern tImage greeny_up_2;
extern tImage greeny_down_1;
extern tImage greeny_down_2;

extern tImage greeny_death_0;
extern tImage greeny_death_1;
extern tImage greeny_death_2;
extern tImage greeny_death_3;
extern tImage greeny_death_4;
extern tImage greeny_death_5;
extern tImage greeny_death_6;
extern tImage greeny_death_7;
extern tImage greeny_death_8;
extern tImage greeny_death_9;
extern tImage greeny_death_10;
extern tImage greeny_death_11;
extern tImage greeny_death_12;

extern tImage blinky_side_1;
extern tImage blinky_side_2;
extern tImage blinky_up_1;
extern tImage blinky_up_2;
extern tImage blinky_down_1;
extern tImage blinky_down_2;

extern tImage clyde_side_1;
extern tImage clyde_side_2;
extern tImage clyde_up_1;
extern tImage clyde_up_2;
extern tImage clyde_down_1;
extern tImage clyde_down_2;

extern tImage inky_side_1;
extern tImage inky_side_2;
extern tImage inky_up_1;
extern tImage inky_up_2;
extern tImage inky_down_1;
extern tImage inky_down_2;

extern tImage pinky_side_1;
extern tImage pinky_side_2;
extern tImage pinky_up_1;
extern tImage pinky_up_2;
extern tImage pinky_down_1;
extern tImage pinky_down_2;

extern tImage eyes_side;
extern tImage eyes_up;
extern tImage eyes_down;

extern tImage blue_1;
extern tImage blue_2;

extern tImage super_pellet;

extern tImage test;

game_object pacman;
game_object greeny;
game_object blinky;
game_object clyde;
game_object inky;
game_object pinky;

game_object pacman_live1;
game_object pacman_live2;
game_object pacman_live3;

game_object greeny_live1;
game_object greeny_live2;
game_object greeny_live3;

tImage* pacman_lives[] = {&pacman_side_1};
tImage* greeny_lives[] = {&greeny_side_1};

// list of objects for rendering
game_object* object_list[] = {&pacman, &greeny, &blinky, &clyde, &inky, &pinky};
game_object* pacman_lives_list[] = { &pacman_live1, &pacman_live2, &pacman_live3};
game_object* greeny_lives_list[] = { &greeny_live1, &greeny_live2, &greeny_live3};

tImage* pacman_sprites[] = {&pacman_side_1, &pacman_side_2, &pacman_up_1, &pacman_up_2, &pacman_down_1, &pacman_down_2};
tImage* greeny_sprites[] = {&greeny_side_1, &greeny_side_2, &greeny_up_1, &greeny_up_2, &greeny_down_1, &greeny_down_2};
tImage* blinky_sprites[] = {&blinky_side_1, &blinky_side_2, &blinky_up_1, &blinky_up_2, &blinky_down_1, &blinky_down_2, &eyes_side, &eyes_up, &eyes_down, &blue_1, &blue_2};
tImage*  clyde_sprites[] = { &clyde_side_1,  &clyde_side_2,  &clyde_up_1,  &clyde_up_2,  &clyde_down_1,  &clyde_down_2, &eyes_side, &eyes_up, &eyes_down, &blue_1, &blue_2};
tImage*   inky_sprites[] = {  &inky_side_1,   &inky_side_2,   &inky_up_1,   &inky_up_2,   &inky_down_1,   &inky_down_2, &eyes_side, &eyes_up, &eyes_down, &blue_1, &blue_2};
tImage*  pinky_sprites[] = { &pinky_side_1,  &pinky_side_2,  &pinky_up_1,  &pinky_up_2,  &pinky_down_1,  &pinky_down_2, &eyes_side, &eyes_up, &eyes_down, &blue_1, &blue_2};

tImage* pacman_death_sprites[] = {&pacman_death_0, &pacman_death_1, &pacman_death_2, &pacman_death_3, &pacman_death_4, &pacman_death_5, &pacman_death_6, &pacman_death_7, &pacman_death_8, &pacman_death_9, &pacman_death_10, &pacman_death_11, &pacman_death_12};
tImage* greeny_death_sprites[] = {&greeny_death_0, &greeny_death_1, &greeny_death_2, &greeny_death_3, &greeny_death_4, &greeny_death_5, &greeny_death_6, &greeny_death_7, &greeny_death_8, &greeny_death_9, &greeny_death_10, &greeny_death_11, &greeny_death_12};

Node* dead_node;

bool _2player = false;
bool background = true; // should we draw the background? for efficiency purposes

uint8_t state = 0;
uint8_t pellet_counter = 0;

int paco_note_index = 0;
const int paco_totalNotes = sizeof(paco_notes) / sizeof(int);

void render_game(void);
void draw_sd_img(char* filename, uint16_t x, uint8_t y);
void start_conditions(bool _2player);
void play_death_anim(game_object* self, tImage* sprite_list[]);
void render_score(game_object* self, uint16_t x, uint16_t y);
void draw_background(void);
void game_menu(void);
void game_play(void);
void game_over(void);


void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    Serial.begin(9600);
    GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    Serial.println("Inicio");

    // SD setup
    SPI.setModule(0);
    Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT);
    if(!SD.begin(32))
    {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    // random setup
    pinMode(A3, INPUT); // dont connect anything here
    randomSeed(analogRead(A3));

    pinMode(PD_7, OUTPUT);

    input_init();
    
    LCD_Init();
    LCD_Clear(0x00);
  
    map1_init();

    dead_node = node_new(300, 220, 0, false, false);

    game_object_init(&pacman, map1_nodes[37]->x, map1_nodes[37]->y, pacman_sprites);
    game_object_init(&greeny, map1_nodes[38]->x, map1_nodes[38]->y, greeny_sprites);
    game_object_init(&blinky, map1_nodes[24]->x, map1_nodes[24]->y, blinky_sprites);
    game_object_init(&clyde , map1_nodes[31]->x, map1_nodes[31]->y,  clyde_sprites);
    game_object_init(&inky  , map1_nodes[29]->x, map1_nodes[29]->y,   inky_sprites);
    game_object_init(&pinky , map1_nodes[30]->x, map1_nodes[30]->y,  pinky_sprites);

    game_object_init(&pacman_live1, 250, 10, pacman_lives);
    game_object_init(&pacman_live2, 265, 10, pacman_lives);
    game_object_init(&pacman_live3, 280, 10, pacman_lives);

    game_object_init(&greeny_live1, 250, 110, greeny_lives);
    game_object_init(&greeny_live2, 265, 110, greeny_lives);
    game_object_init(&greeny_live3, 280, 110, greeny_lives);

    game_object_set_node(&pacman, map1_nodes[37]);
    game_object_set_node(&greeny, map1_nodes[38]);
    game_object_set_node(&blinky, map1_nodes[24]);
    game_object_set_node(&clyde , map1_nodes[31]);
    game_object_set_node(&inky  , map1_nodes[29]);
    game_object_set_node(&pinky , map1_nodes[30]);
}

void loop(void)
{
    read_inputs();

    switch (state)
    {
        case 0:
            game_menu();
            break;

        case 1:
            game_play();
            break;

        case 2:
            game_over();
            break;
    }
}

void game_menu(void)
{
    draw_sd_img((char*)"clyde.txt",   0,   0);
    draw_sd_img((char*)"clyde.txt", 160,   0);
    draw_sd_img((char*)"clyde.txt", 280,  40);
    draw_sd_img((char*)"clyde.txt", 280, 200);
    draw_sd_img((char*)"clyde.txt", 120, 200);
    draw_sd_img((char*)"clyde.txt",   0, 160);

    draw_sd_img((char*)"inky.txt",  40,   0);
    draw_sd_img((char*)"inky.txt", 200,   0);
    draw_sd_img((char*)"inky.txt", 280,  80);
    draw_sd_img((char*)"inky.txt", 240, 200);
    draw_sd_img((char*)"inky.txt",  80, 200);
    draw_sd_img((char*)"inky.txt",   0, 120);

    draw_sd_img((char*)"pinky.txt",  80,   0);
    draw_sd_img((char*)"pinky.txt", 240,   0);
    draw_sd_img((char*)"pinky.txt", 280, 120);
    draw_sd_img((char*)"pinky.txt", 200, 200);
    draw_sd_img((char*)"pinky.txt",  40, 200);
    draw_sd_img((char*)"pinky.txt",   0,  80);

    draw_sd_img((char*)"blinky.txt", 120,   0);
    draw_sd_img((char*)"blinky.txt", 280,   0);
    draw_sd_img((char*)"blinky.txt", 280, 160);
    draw_sd_img((char*)"blinky.txt", 160, 200);
    draw_sd_img((char*)"blinky.txt",   0, 200);
    draw_sd_img((char*)"blinky.txt",   0,  40);

    draw_sd_img((char*)"title_1.txt",  40,  40);
    draw_sd_img((char*)"title_2.txt",  80,  40);
    draw_sd_img((char*)"title_3.txt", 120,  40);
    draw_sd_img((char*)"title_4.txt", 160,  40);
    draw_sd_img((char*)"title_5.txt", 200,  40);
    draw_sd_img((char*)"title_6.txt", 240,  40);

    draw_sd_img((char*)"paco_1_1.txt", 120,  80);
    draw_sd_img((char*)"paco_1_2.txt", 160,  80);
    draw_sd_img((char*)"paco_2_1.txt", 120, 120);
    draw_sd_img((char*)"paco_2_2.txt", 160, 120);

    LCD_Print("SW1: PLAYER 1", 114, 162, 1, 0xffff, 0x0000);
    LCD_Print("SW2: PLAYER 2", 114, 172, 1, 0xffff, 0x0000);


    int intro_note_index = 0;
    const int intro_totalNotes = sizeof(intro_notes) / sizeof(int);

    while (true)
    {
        read_inputs();

        if (get_event(SW1))
        {
            _2player = false;
            break;
        }

        if (get_event(SW2))
        {
            _2player = true;
            break;
        }

        play_note(intro_notes, intro_durations, intro_note_index % intro_totalNotes, 1);

        intro_note_index++;
    }

    LCD_Clear(0x0000);

    start_conditions(_2player);

    state = 1;
}

void game_play(void)
{
    if (background)
    {
        draw_background();
    }

    // pacman movement
    if (game_object_is_on_node(&pacman))
    {
        if (get_event(UP_1))
        {
            game_object_direction(&pacman, UP);
        }

        if (get_event(DOWN_1))
        {
            game_object_direction(&pacman, DOWN);
        }

        if (get_event(LEFT_1))
        {
            game_object_direction(&pacman, LEFT);
        }

        if (get_event(RIGHT_1))
        {
            game_object_direction(&pacman, RIGHT);
        }
    }

    // greeny movement
    if (game_object_is_on_node(&greeny))
    {
        if (get_event(UP_2))
        {
            game_object_direction(&greeny, UP);
        }

        if (get_event(DOWN_2))
        {
            game_object_direction(&greeny, DOWN);
        }

        if (get_event(LEFT_2))
        {
            game_object_direction(&greeny, LEFT);
        }

        if (get_event(RIGHT_2))
        {
            game_object_direction(&greeny, RIGHT);
        }
    }

    game_object_pacman_move(&pacman, pacman.direction);
    game_object_pacman_move(&greeny, greeny.direction);

    game_object_ghost_move(&blinky);
    game_object_ghost_move(&clyde);
    game_object_ghost_move(&inky);
    game_object_ghost_move(&pinky);

    if (pacman.powerup || greeny.powerup)
    {
        blinky.sick = true;
        clyde.sick  = true;
        inky.sick   = true;
        pinky.sick  = true;
    }
    else
    {
        blinky.sick = false;
        clyde.sick  = false;
        inky.sick   = false;
        pinky.sick  = false;
    }

    game_object_collision(&pacman, &blinky);
    game_object_collision(&pacman, &clyde);
    game_object_collision(&pacman, &inky);
    game_object_collision(&pacman, &pinky);

    if (_2player)
    {
        game_object_collision(&greeny, &blinky);
        game_object_collision(&greeny, &clyde);
        game_object_collision(&greeny, &inky);
        game_object_collision(&greeny, &pinky);
    }


    // dead logic
    if (pacman.dead || greeny.dead)
    {
        if (pacman.dead)
        {
            play_death_anim(&pacman, pacman_death_sprites);
        }

        if (greeny.dead)
        {
            play_death_anim(&greeny, greeny_death_sprites);
        }

        pacman.dead = false;
        greeny.dead = false;

        for (int i = 0; i<6; i++)
        {
            FillRect(object_list[i]->x, object_list[i]->y, object_list[i]->w, object_list[i]->h, 0x0000);
        }

        start_conditions(_2player);

        if (pacman.lives == 0)
        {
            pacman.hidden = true;
            game_object_set_node(&pacman, dead_node);
            game_object_set_pos(&pacman, dead_node->x, dead_node->y);
        }

        if (greeny.lives == 0)
        {
            greeny.hidden = true;
            game_object_set_node(&greeny, dead_node);
            game_object_set_pos(&greeny, dead_node->x, dead_node->y);
        }

        if ((pacman.lives == 0) && (greeny.lives == 0))
        {
            state = 2;
        }

        if ((pacman.lives == 0) && !_2player)
        {
            state = 2;
        }
    }

    game_object_update_index(&pacman);
    game_object_update_index(&greeny);
    game_object_update_index(&blinky);
    game_object_update_index(&clyde);
    game_object_update_index(&inky);
    game_object_update_index(&pinky);

    render_game();

    if (pellet_counter == 68)
    {
        state = 2;
    }

    pellet_counter = 0;

    play_note(paco_notes, paco_durations, paco_note_index % paco_totalNotes, 4);

    paco_note_index++;
}

void game_over(void)
{
    LCD_Clear(0x00);

    if (_2player)
    {
        if ((pacman.lives == 0) && (greeny.lives == 0))
        {
            LCD_Print("YOU DIED", 126, 100, 1, 0xffff, 0x0000);

        }
        else
        {
            LCD_Print("YOU WON", 130, 40, 1, 0xffff, 0x0000); 

            LCD_Print("P1: ", 120, 100, 1, 0xffff, 0x0000);
            LCD_Print("P2: ", 120, 120, 1, 0xffff, 0x0000);

            pacman.old_score = 0; // hack, do not touch
            greeny.old_score = 0; // hack, do not touch
            render_score(&pacman, 146, 100);
            render_score(&greeny, 146, 120);
        }
    }
    else
    {
        if (pacman.lives == 0)
        {
            LCD_Print("YOU DIED", 126, 100, 1, 0xffff, 0x0000);

        }
        else
        {
            LCD_Print("YOU WON", 130, 90, 1, 0xffff, 0x0000); 

        }

        pacman.old_score = 0; // hack, do not touch
        render_score(&pacman, 134, 120);
    }

    while (true);
}

void render_game_object(game_object* self)
{
    if (self->hidden)
    {
        FillRect(self->x, self->y, self->w, self->h, 0x0000);
        return;
    }

    tImage* temp_sprite = self->sprite_list[self->sprite_index];

    LCD_Bitmap( self->x, self->y,
                temp_sprite->w, temp_sprite->h,
                (uint8_t*)temp_sprite->data,
                self->x_flip, self->y_flip);
}

void render_pellet(Node* n)
{
    if (!n->pellet)
    {
        FillRect(n->x+7, n->y+7, 2, 2, 0x0000);

        pellet_counter++;
        return;
    }

    if (n->super)
    {
        LCD_Bitmap(n->x, n->y, super_pellet.w, super_pellet.h, super_pellet.data, false, false);
        return;
    }

    FillRect(n->x+7, n->y+7, 2, 2, 0xFFFF);
}

void render_lives(game_object* self, game_object* list[])
{
    if (self->old_lives == self->lives)
    {
        return;
    }

    self->old_lives = self->lives;

    if (self->lives < 3)
    {
        list[2]->hidden = true;
    }

    if (self->lives < 2)
    {
        list[1]->hidden = true;
    }

    if (self->lives < 1)
    {
        list[0]->hidden = true;
    }

    for (int i = 0; i<3; i++)
    {
        render_game_object(list[i]);
    }
}

void render_score(game_object* self, uint16_t x, uint16_t y)
{
    if (self->old_score == self->score)
    {
        return;
    }

    self->old_score = self->score;

    char text[] = "00000";
    sprintf(text, "%06i", self->score);

    LCD_Print(text, x, y, 1, 0xffff, 0x0000);
}

void render_game(void)
{
    for (int i =0; i<68; i++)
    {
        render_pellet(map1_nodes[i]);
    }

    for (int i = 0; i<6; i++)
    {
        render_game_object(object_list[i]);
    }

    render_score(&pacman, 250,  30);
    render_lives(&pacman, pacman_lives_list);

    if (_2player)
    {
        render_score(&greeny, 250, 130);
        render_lives(&greeny, greeny_lives_list);
    }
}

void draw_sd_img(char* filename, uint16_t x, uint8_t y)
{
    Serial.print(filename);
    Serial.print(": ");
    Serial.println(SD.exists(filename));

    File file = SD.open(filename);

    // read file contents
    char data[file.size()];
    file.read(data, file.size());
    file.close();

    // delimiters
    const char s1[2] = ",";
    const char s2[2] = ";";

    // get width and heigth
    uint8_t width = strtoul(strtok(data, s1), NULL, 10);
    uint8_t height = strtoul(strtok(NULL, s2), NULL, 10);

    // reserve space for parsed data
    uint8_t* temp_data = (uint8_t*)malloc(width*height*2*sizeof(uint8_t));

    // parse data
    for (int i = 0; i<(width*height*2); i++)
    {
        temp_data[i] = strtoul(strtok(NULL, s1), NULL, 16);
    }

    // render
    LCD_Bitmap(x, y, width, height, temp_data, false, false);

    // dont forget to free allocated memory
    free(temp_data);
}

void start_conditions(bool _2player)
{
    game_object_set_node(&pacman, map1_nodes[37]);
    game_object_set_pos( &pacman, map1_nodes[37]->x, map1_nodes[37]->y);

    if (_2player)
    {
        game_object_set_node(&greeny, map1_nodes[38]);
        game_object_set_pos( &greeny, map1_nodes[38]->x, map1_nodes[38]->y);
    }
    else
    {
        greeny.hidden = true;
        game_object_set_node(&greeny, dead_node);
        game_object_set_pos(&greeny, dead_node->x, dead_node->y);
    }

    game_object_set_node(&blinky, map1_nodes[24]);
    game_object_set_pos( &blinky, map1_nodes[24]->x, map1_nodes[24]->y);

    game_object_set_node(&clyde, map1_nodes[31]);
    game_object_set_pos( &clyde, map1_nodes[31]->x, map1_nodes[31]->y);

    game_object_set_node(&inky, map1_nodes[29]);
    game_object_set_pos( &inky, map1_nodes[29]->x, map1_nodes[29]->y);

    game_object_set_node(&pinky, map1_nodes[30]);
    game_object_set_pos( &pinky, map1_nodes[30]->x, map1_nodes[30]->y);
}

void play_death_anim(game_object* self, tImage* sprite_list[])
{
    for (int i = 0; i<13; i++)
    {
        tImage* temp_sprite = sprite_list[i];
        LCD_Bitmap(self->x, self->y,
                   temp_sprite->w, temp_sprite->h,
                   (uint8_t*)temp_sprite->data,
                    false, false);

        play_note(death_notes, death_durations, i, 1);
    }

    // play last notes
    for (int i = 0; i<4; i++)
    {
        play_note(death_notes, death_durations, i+13, 1);
    }
}

void draw_background(void)
{

    // cast because compiler warning
    draw_sd_img((char*)"map1_1-1.txt",   0,   0);
    draw_sd_img((char*)"map1_1-2.txt",  56,   0);
    draw_sd_img((char*)"map1_1-3.txt", 112,   0);
    draw_sd_img((char*)"map1_1-4.txt", 168,   0);
    draw_sd_img((char*)"map1_2-1.txt",   0,  30);
    draw_sd_img((char*)"map1_2-2.txt",  56,  30);
    draw_sd_img((char*)"map1_2-3.txt", 112,  30);
    draw_sd_img((char*)"map1_2-4.txt", 168,  30);
    draw_sd_img((char*)"map1_3-1.txt",   0,  60);
    draw_sd_img((char*)"map1_3-2.txt",  56,  60);
    draw_sd_img((char*)"map1_3-3.txt", 112,  60);
    draw_sd_img((char*)"map1_3-4.txt", 168,  60);
    draw_sd_img((char*)"map1_4-1.txt",   0,  90);
    draw_sd_img((char*)"map1_4-2.txt",  56,  90);
    draw_sd_img((char*)"map1_4-3.txt", 112,  90);
    draw_sd_img((char*)"map1_4-4.txt", 168,  90);
    draw_sd_img((char*)"map1_5-1.txt",   0, 120);
    draw_sd_img((char*)"map1_5-2.txt",  56, 120);
    draw_sd_img((char*)"map1_5-3.txt", 112, 120);
    draw_sd_img((char*)"map1_5-4.txt", 168, 120);
    draw_sd_img((char*)"map1_6-1.txt",   0, 150);
    draw_sd_img((char*)"map1_6-2.txt",  56, 150);
    draw_sd_img((char*)"map1_6-3.txt", 112, 150);
    draw_sd_img((char*)"map1_6-4.txt", 168, 150);
    draw_sd_img((char*)"map1_7-1.txt",   0, 180);
    draw_sd_img((char*)"map1_7-2.txt",  56, 180);
    draw_sd_img((char*)"map1_7-3.txt", 112, 180);
    draw_sd_img((char*)"map1_7-4.txt", 168, 180);
    draw_sd_img((char*)"map1_8-1.txt",   0, 210);
    draw_sd_img((char*)"map1_8-2.txt",  56, 210);
    draw_sd_img((char*)"map1_8-3.txt", 112, 210);
    draw_sd_img((char*)"map1_8-4.txt", 168, 210);

    background = false;
}
