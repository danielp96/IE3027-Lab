#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include <stdint.h>
#include <stdbool.h>

#include "map.h"

#ifndef __TIMAGE_STRUCT__
#define __TIMAGE_STRUCT__

typedef struct
{
    uint8_t *data;
    uint16_t w;
    uint16_t h;
} tImage;

#endif // __TIMAGE_STRUCT__


enum SpriteID {SIDE1, SIDE2, UP1, UP2, DOWN1, DOWN2, DEAD_SIDE, DEAD_UP, DEAD_DOWN, SICK1, SICK2};
enum Direction {UP, DOWN, LEFT, RIGHT};


typedef struct
{
    uint16_t x,y;
    uint16_t w,h;

    uint16_t max_x, min_x;
    uint16_t max_y, min_y;

    uint16_t score;
    uint16_t old_score;

    uint8_t lives;
    uint8_t old_lives;

    bool enable_x, enable_y;

    bool walk_sprite; // which walking sprite show
    bool x_flip, y_flip;

    tImage** sprite_list;
    uint8_t sprite_index;

    Direction direction;
    uint8_t steps;
    uint8_t steps_max; // amount of steps before changing sprite
    uint8_t dead_steps;
    uint8_t dead_steps_max; // amount of steps before reviving
    uint16_t powerup_steps;
    uint16_t powerup_steps_max;  // amount of steps before powerup off

    Node* node;

    bool dead;
    bool powerup; // pacman has powerup
    bool sick;    // ghosts vulnerable to powerup

    bool hidden;    
} game_object;


void game_object_init(game_object* self, uint16_t x, uint16_t y, tImage* sprite_list[])
{
    self->x = x;
    self->y = y;

    self->enable_x = true;
    self->enable_y = true;

    self->w = sprite_list[0]->w;
    self->h = sprite_list[0]->h;

    self->max_x = 0;
    self->min_x = 0;
    self->max_y = 0;
    self->min_y = 0;

    self->x_flip = false;
    self->y_flip = false;

    self->sprite_list = sprite_list;
    self->sprite_index = 0;

    self->hidden = false;
    self->dead = false;

    self->sick = false;
    self->powerup = false;

    self->direction = RIGHT;
    self->steps = 0;
    self->steps_max = 10;
    self->dead_steps = 0;
    self->dead_steps_max = 60;
    self->powerup_steps = 0;
    self->powerup_steps_max = 80;

    self->score = 0;
    self->lives = 3;
    self->old_score = -1;
    self->old_lives = -1;

    self->node = NULL;
}

void game_object_add_x(game_object* self, int8_t x, uint16_t max, uint16_t min)
{
    int temp = self->x + x;

    if (temp > (max - self->w))
    {
        self->x = max - self->w;
        return;
    }

    if (temp < min)
    {
        self->x = min;
        return;
    }

    self->x = temp;
}

void game_object_add_y(game_object* self, int8_t y, uint16_t max, uint16_t min)
{
    int temp = self->y + y;

    if (temp > (max - self->w))
    {
        self->y = max - self->w;
        return;
    }

    if (temp < min)
    {
        self->y = min;
        return;
    }

    self->y = temp;
}

void game_object_set_pos(game_object* self, uint16_t x, uint16_t y)
{
    self->x = x;
    self->y = y;
}

void game_object_set_sprite(game_object* self, SpriteID id, bool x_flip, bool y_flip)
{
    self->sprite_index = id;

    self->w = self->sprite_list[id]->w;
    self->h = self->sprite_list[id]->h;

    self->x_flip = x_flip;
    self->y_flip = y_flip;
}

void game_object_direction(game_object* self, Direction d)
{
    self->direction = d;
}

bool game_object_is_on_node(game_object* self)
{
    return (self->x == self->node->x) && (self->y == self->node->y);
}

void game_object_set_node(game_object* self, Node* n)
{
    self->node = n;

    self->enable_x = true;
    self->enable_y = true;

    if (n->up != NULL)
    {
        self->min_y = n->up->y;
    }
    else
    {
        self->min_y = n->y;
    }

    if (n->down != NULL)
    {
        self->max_y = n->down->y + self->h;
    }
    else
    {
        self->max_y = n->y + self->h;
    }


    if (n->left != NULL)
    {
        self->min_x = n->left->x;
    }
    else
    {
        self->min_x = n->x;
    }

    if (n->right != NULL)
    {
        self->max_x = n->right->x + self->w;
    }
    else
    {
        self->max_x = n->x + self->w;
    }
}

void game_object_move(game_object* self, Direction d)
{
    self->enable_x = (self->y == self->node->y);

    self->enable_y = (self->x == self->node->x);

    game_object_direction(self, d);
    switch (d)
    {
        case UP:
            if (self->enable_y)
            {
                game_object_add_y(self, -1, self->max_y, self->min_y);

                if (self->y == self->node->up->y)
                {
                    game_object_set_node(self, self->node->up);
                }
            }
            break;

        case DOWN:
            if (self->enable_y)
            {
                game_object_add_y(self, 1, self->max_y, self->min_y);

                if (self->y == self->node->down->y)
                {
                    game_object_set_node(self, self->node->down);
                }

            }
            break;

        case LEFT:
            if (self->enable_x)
            {
                game_object_add_x(self, -1, self->max_x, self->min_x);

                if (self->x == self->node->left->x)
                {
                    game_object_set_node(self, self->node->left);
                }

            }
            break;

        case RIGHT:
            if (self->enable_x)
            {
                game_object_add_x(self, 1, self->max_x, self->min_x);

                if (self->x == self->node->right->x)
                {
                    game_object_set_node(self, self->node->right);
                }
            }
            break;
    }

    self->steps++;

    if (self->steps > self->steps_max)
    {
        self->steps = 0;
        self->walk_sprite = !self->walk_sprite;
    }
}

void game_object_update_index(game_object* self)
{
    SpriteID tempID = SIDE1;
    bool temp_x_flip = false;
    bool temp_y_flip = false;

    switch (self->direction)
    {
        case UP:
            tempID = self->walk_sprite?UP1:UP2;

            if (self->sick)
            {
                tempID = self->walk_sprite?SICK1:SICK2;
            }
            tempID = self->dead?DEAD_UP:tempID;
            temp_x_flip = false;
            temp_y_flip = false;
            game_object_set_sprite(self, tempID, temp_x_flip, temp_y_flip);
            break;

        case DOWN:
            tempID = self->walk_sprite?DOWN1:DOWN2;

            if (self->sick)
            {
                tempID = self->walk_sprite?SICK1:SICK2;
            }
            tempID = self->dead?DEAD_DOWN:tempID;
            temp_x_flip = false;
            temp_y_flip = false;
            game_object_set_sprite(self, tempID, temp_x_flip, temp_y_flip);
            break;

        case LEFT:
            tempID = self->walk_sprite?SIDE1:SIDE2;

            if (self->sick)
            {
                tempID = self->walk_sprite?SICK1:SICK2;
            }
            tempID = self->dead?DEAD_SIDE:tempID;
            temp_x_flip = true;
            temp_y_flip = false;
            game_object_set_sprite(self, tempID, temp_x_flip, temp_y_flip);
            break;

        case RIGHT:
            tempID = self->walk_sprite?SIDE1:SIDE2;

            if (self->sick)
            {
                tempID = self->walk_sprite?SICK1:SICK2;
            }
            tempID = self->dead?DEAD_SIDE:tempID;
            temp_x_flip = false;
            temp_y_flip = false;
            game_object_set_sprite(self, tempID, temp_x_flip, temp_y_flip);
            break;
    }
}

// assumes randomseed is set
void game_object_random_move(game_object* self)
{
    if (!game_object_is_on_node(self))
    {
        game_object_move(self, self->direction);
        return;
    }

    // dont enter the nest when alive
    if (self->node->value == 1)
    {
        if (self->direction == UP)
        {
            game_object_move(self, (Direction)(random(2)+2));
        }
        else
        {
            game_object_move(self, self->direction);
        }

        return;
    }

    Direction dir;

    Direction valid_dirs[3];

    switch (self->direction)
    {
        case UP:
            valid_dirs[0] = UP;
            valid_dirs[1] = LEFT;
            valid_dirs[2] = RIGHT;
            break;

        case DOWN:
            valid_dirs[0] = DOWN;
            valid_dirs[1] = LEFT;
            valid_dirs[2] = RIGHT;
            break;
            
        case LEFT:
            valid_dirs[0] = UP;
            valid_dirs[1] = DOWN;
            valid_dirs[2] = LEFT;
            break;
            
        case RIGHT:
            valid_dirs[0] = UP;
            valid_dirs[1] = RIGHT;
            valid_dirs[2] = DOWN;
            break;
    }

    dir = valid_dirs[random(3)];

    game_object_move(self, dir);
}

// assumes randomseed is set
void game_object_ghost_move(game_object* self)
{
    if (!self->dead)
    {
        game_object_random_move(self);
        return;
    }

    // if not on node, kepp moving
    if (!game_object_is_on_node(self))
    {
        game_object_move(self, self->direction);
        return;
    }

    // if not on node and in the nest, move left-right
    if (self->node->value == 0)
    {
        game_object_move(self, (Direction)(random(2)+2));
        self->dead_steps++;

        // countdown to revive
        if (self->dead_steps > self->dead_steps_max)
        {
            self->dead_steps = 0;
            self->dead = false;
        }

        return;
    }

    Node* dir_list[4];

    dir_list[0] = self->node->up;
    dir_list[1] = self->node->down;
    dir_list[2] = self->node->left;
    dir_list[3] = self->node->right;

    uint8_t min = 255; // higher than any posibble node value
    Direction dir = LEFT;

    // search min value
    for (int i=0; i<4; i++)
    {
        if (dir_list[i] != NULL)
        {
            if (dir_list[i]->value < min)
            {
                min = dir_list[i]->value;
            }
        }
    }

    // get index of min value
    for (int i=0; i<4; i++)
    {
        if (dir_list[i] != NULL)
        {
            if (dir_list[i]->value == min)
            {
                dir = (Direction)i;
                break;
            }
        }
    }

    game_object_move(self, dir);
}

void game_object_pacman_move(game_object* self, Direction d)
{
    // if not on node, kepp moving
    if (!game_object_is_on_node(self))
    {
        game_object_move(self, self->direction);
        return;
    }

    // dont enter ghost nest
    if ((self->node->value == 1) && (d == DOWN))
    {
        return;
    }

    if (self->node->pellet)
    {
        if (self->node->super)
        {
            self->score += 200;
            self->powerup = true;
        }
        else
        {
            self->score += 50;
        }

        self->node->pellet = false;
    }

    game_object_move(self, d);


    if (self->powerup)
    {
        self->powerup_steps++;
    
        if (self->powerup_steps > self->powerup_steps_max)
        {
            self->powerup_steps = 0;
            self->powerup = false;
        }
    }
}

void game_object_collision(game_object* player, game_object* ghost)
{
    uint8_t delta_x = abs(player->x - ghost->x);
    uint8_t delta_y = abs(player->y - ghost->y);

    if ((delta_x < 5) && (delta_y < 5))
    {
        if (player->powerup)
        {
            player->score += 500;
            ghost->dead = true;
        }
        else
        {
            player->lives--;
            player->dead = true;
        }
    }
}

#endif // __GAME_OBJECT__