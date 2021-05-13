#ifndef __NODE_MAP__
#define __NODE_MAP__

#include <stdint.h>

typedef struct Node
{
    uint16_t x,y;
    uint8_t value; // to help navigation to ghost nest
    bool pellet; // has pellet
    bool super; // is pellet super pellet
    Node* up;
    Node* down;
    Node* left;
    Node* right;

} Node;

Node* map1_nodes[68];

Node* node_new(uint16_t x, uint16_t y, uint8_t value, bool pellet, bool super)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->x = x;
    n->y = y;
    n->value = value;
    n->pellet = pellet;
    n->super = super;
    n->up = NULL;
    n->down = NULL;
    n->left = NULL;
    n->right = NULL;

    return n;
}

void node_link(Node** map, uint8_t id1, uint8_t id2)
{
    Node* n1 = map[id1];
    Node* n2 = map[id2];

    if (n1->x == n2->x)
    {
        if (n1->y < n2->y)
        {
            n1->down = n2;
            n2->up = n1;
        }
        else
        {
            n2->down = n1;
            n1->up = n2;
        }
    }

    if (n1->y == n2->y)
    {
        if (n1->x < n2->x)
        {
            n1->right = n2;
            n2->left = n1;
        }
        else
        {
            n2->left = n1;
            n1->right = n2;
        }
    }
}

void map1_init()
{
    map1_nodes[ 0] = node_new(  4,  4, 6, true, false);
    map1_nodes[ 1] = node_new( 44,  4, 6, true, false);
    map1_nodes[ 2] = node_new( 68,  4, 7, true, false);
    map1_nodes[ 3] = node_new(140,  4, 7, true, false);
    map1_nodes[ 4] = node_new(164,  4, 6, true, false);
    map1_nodes[ 5] = node_new(204,  4, 6, true, false);

    map1_nodes[ 6] = node_new(  4, 28, 5, true, true);
    map1_nodes[ 7] = node_new( 20, 28, 4, true, false);
    map1_nodes[ 8] = node_new( 44, 28, 5, true, false);
    map1_nodes[ 9] = node_new( 68, 28, 6, true, false);
    map1_nodes[10] = node_new( 92, 28, 5, true, false); 
    map1_nodes[11] = node_new(116, 28, 5, true, false);
    map1_nodes[12] = node_new(140, 28, 6, true, false);
    map1_nodes[13] = node_new(164, 28, 5, true, false);
    map1_nodes[14] = node_new(188, 28, 4, true, false);
    map1_nodes[15] = node_new(204, 28, 5, true, true);

    map1_nodes[16] = node_new( 44, 52, 4, true, false);
    map1_nodes[17] = node_new( 68, 52, 3, true, false);
    map1_nodes[18] = node_new( 92, 52, 4, true, false);
    map1_nodes[19] = node_new(116, 52, 4, true, false);
    map1_nodes[20] = node_new(140, 52, 3, true, false);
    map1_nodes[21] = node_new(164, 52, 4, true, false);

    map1_nodes[22] = node_new( 20, 76, 3, true, false);
    map1_nodes[23] = node_new( 68, 76, 2, true, false);
    map1_nodes[24] = node_new(104, 76, 1, true, false);
    map1_nodes[25] = node_new(140, 76, 2, true, false);
    map1_nodes[26] = node_new(188, 76, 3, true, false);

    map1_nodes[27] = node_new( 44, 100, 4, true, false);
    map1_nodes[28] = node_new( 68, 100, 3, true, false);
    map1_nodes[29] = node_new( 88, 100, 0, false, false);
    map1_nodes[30] = node_new(104, 100, 0, false, false);
    map1_nodes[31] = node_new(120, 100, 0, false, false);
    map1_nodes[32] = node_new(140, 100, 3, true, false);
    map1_nodes[33] = node_new(164, 100, 4, true, false);

    map1_nodes[34] = node_new( 20, 124, 6, true, false);
    map1_nodes[35] = node_new( 44, 124, 5, true, false);
    map1_nodes[36] = node_new( 68, 124, 4, true, false);
    map1_nodes[37] = node_new( 92, 124, 5, false, false);
    map1_nodes[38] = node_new(116, 124, 5, false, false);
    map1_nodes[39] = node_new(140, 124, 4, true, false);
    map1_nodes[40] = node_new(164, 124, 5, true, false);
    map1_nodes[41] = node_new(188, 124, 6, true, false);

    map1_nodes[42] = node_new( 20, 148, 5, true, false);
    map1_nodes[43] = node_new( 68, 148, 6, true, false);
    map1_nodes[44] = node_new( 92, 148, 6, true, false);
    map1_nodes[45] = node_new(116, 148, 6, true, false);
    map1_nodes[46] = node_new(140, 148, 6, true, false);
    map1_nodes[47] = node_new(188, 148, 5, true, false);

    map1_nodes[48] = node_new(  4, 172, 7, true, true);
    map1_nodes[49] = node_new( 20, 172, 6, true, false);
    map1_nodes[50] = node_new( 44, 172, 7, true, false);
    map1_nodes[51] = node_new( 68, 172, 7, true, false);
    map1_nodes[52] = node_new( 92, 172, 8, true, false); 
    map1_nodes[53] = node_new(116, 172, 8, true, false);
    map1_nodes[54] = node_new(140, 172, 7, true, false);
    map1_nodes[55] = node_new(164, 172, 7, true, false);
    map1_nodes[56] = node_new(188, 172, 6, true, false);
    map1_nodes[57] = node_new(204, 172, 7, true, true);

    map1_nodes[58] = node_new( 68, 196, 10, true, false);
    map1_nodes[59] = node_new( 92, 196, 9, true, false);
    map1_nodes[60] = node_new(116, 196, 9, true, false);
    map1_nodes[61] = node_new(140, 196, 10, true, false);

    map1_nodes[62] = node_new(  4, 220, 8, true, false);
    map1_nodes[63] = node_new( 44, 220, 8, true, false);
    map1_nodes[64] = node_new( 68, 220, 9, true, false);
    map1_nodes[65] = node_new(140, 220, 9, true, false);
    map1_nodes[66] = node_new(164, 220, 8, true, false);
    map1_nodes[67] = node_new(204, 220, 8, true, false);

    node_link(map1_nodes,  0,  1);
    node_link(map1_nodes,  0,  6);
    node_link(map1_nodes,  6,  7);
    node_link(map1_nodes,  7,  8);
    node_link(map1_nodes,  1,  8);
    node_link(map1_nodes,  8,  9);
    node_link(map1_nodes,  9,  2);
    node_link(map1_nodes,  9, 10);
    node_link(map1_nodes, 10, 11);
    node_link(map1_nodes, 11, 12);
    node_link(map1_nodes,  2,  3);
    node_link(map1_nodes,  3, 12);
    node_link(map1_nodes, 12, 13);
    node_link(map1_nodes, 13, 14);
    node_link(map1_nodes, 13,  4);
    node_link(map1_nodes,  4,  5);
    node_link(map1_nodes, 14, 15);
    node_link(map1_nodes,  5, 15);
    node_link(map1_nodes,  8, 16);
    node_link(map1_nodes, 10, 18);
    node_link(map1_nodes, 11, 19);
    node_link(map1_nodes, 13, 21);
    node_link(map1_nodes, 16, 17);
    node_link(map1_nodes, 17, 18);
    node_link(map1_nodes, 19, 20);
    node_link(map1_nodes, 20, 21);
    node_link(map1_nodes, 17, 23);
    node_link(map1_nodes, 20, 25);
    node_link(map1_nodes,  7, 22);
    node_link(map1_nodes, 14, 26);
    node_link(map1_nodes, 22, 23);
    node_link(map1_nodes, 23, 24);
    node_link(map1_nodes, 24, 25);
    node_link(map1_nodes, 25, 26);
    node_link(map1_nodes, 27, 28);
    node_link(map1_nodes, 28, 23);
    node_link(map1_nodes, 24, 30);
    node_link(map1_nodes, 29, 30);
    node_link(map1_nodes, 30, 31);
    node_link(map1_nodes, 25, 32);
    node_link(map1_nodes, 32, 33);
    node_link(map1_nodes, 26, 41);
    node_link(map1_nodes, 22, 34);
    node_link(map1_nodes, 27, 35);
    node_link(map1_nodes, 34, 35);
    node_link(map1_nodes, 28, 36);
    node_link(map1_nodes, 36, 37);
    node_link(map1_nodes, 37, 38);
    node_link(map1_nodes, 38, 39);
    node_link(map1_nodes, 32, 39);
    node_link(map1_nodes, 33, 40);
    node_link(map1_nodes, 40, 41);
    node_link(map1_nodes, 42, 43);
    node_link(map1_nodes, 43, 44);
    node_link(map1_nodes, 45, 46);
    node_link(map1_nodes, 46, 47);
    node_link(map1_nodes, 41, 47);
    node_link(map1_nodes, 34, 42);
    node_link(map1_nodes, 37, 44);
    node_link(map1_nodes, 38, 45);
    node_link(map1_nodes, 42, 49);
    node_link(map1_nodes, 48, 49);
    node_link(map1_nodes, 49, 50);
    node_link(map1_nodes, 50, 51);
    node_link(map1_nodes, 43, 51);
    node_link(map1_nodes, 51, 52);
    node_link(map1_nodes, 52, 53);
    node_link(map1_nodes, 53, 54);
    node_link(map1_nodes, 54, 55);
    node_link(map1_nodes, 55, 56);
    node_link(map1_nodes, 56, 57);
    node_link(map1_nodes, 46, 54);
    node_link(map1_nodes, 47, 56);
    node_link(map1_nodes, 58, 59);
    node_link(map1_nodes, 60, 61);
    node_link(map1_nodes, 52, 59);
    node_link(map1_nodes, 53, 60);
    node_link(map1_nodes, 62, 63);
    node_link(map1_nodes, 63, 64);
    node_link(map1_nodes, 64, 65);
    node_link(map1_nodes, 65, 66);
    node_link(map1_nodes, 66, 67);
    node_link(map1_nodes, 57, 67);
    node_link(map1_nodes, 48, 62);
    node_link(map1_nodes, 61, 65);
    node_link(map1_nodes, 58, 64);
    node_link(map1_nodes, 50, 63);
    node_link(map1_nodes, 55, 66);


    // DEBUG
    //Serial.print("N0: ");
    //Serial.print((uint32_t)map1_nodes[0], HEX);
    //Serial.print(" N1: ");
    //Serial.print((uint32_t)map1_nodes[ 1], HEX);
    //Serial.println("");
    //Serial.print(" N0.right: ");
    //Serial.print((uint32_t)(map1_nodes[ 0]->right), HEX);
}

void map_debug_draw(Node** map)
{
    for (int i = 0; i<68; i++)
    {
        Node* n = map[i];
        FillRect(n->x, n->y, 2, 2, 0xFFFF);

        if (n->up != NULL)
        {
            V_line(n->up->x, n->up->y, n->y - n->up->y, 0xFFFF);
        }

        if (n->down != NULL)
        {
            V_line(n->x, n->y, n->up->y - n->y, 0xFFFF);
        }

        if (n->left != NULL)
        {
            H_line(n->left->x, n->left->y, n->x - n->left->x, 0xFFFF);
        }

        if (n->right != NULL)
        {
            H_line(n->x, n->y, n->right->x - n->x, 0xFFFF);
        }
    } // end debug
}

#endif // __NODE_MAP__