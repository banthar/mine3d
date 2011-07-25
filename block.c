
#include "block.h"

BlockDefinition block_definition[]=
{
//   id|                  name | solid|transp|  draw mode| color mode| textures
     [0 ... 255] = {"Error",false,true,DRAW_BLOCK,COLOR_NONE,{0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}},
	 [0] = {              "Air", false,  true,  DRAW_NONE,  COLOR_NONE, { 0, 0, 0, 0, 0, 0}},
	 [1] = {            "Stone",  true, false, DRAW_BLOCK,  COLOR_NONE, { 1, 1, 1, 1, 1, 1}},
	 [2] = {            "Grass",  true, false, DRAW_BLOCK, COLOR_GRASS, { 2, 0, 3, 3, 3, 3}},
	 [3] = {             "Dirt",  true, false, DRAW_BLOCK,  COLOR_NONE, { 2, 2, 2, 2, 2, 2}},
	 [4] = {     "Cobblerstone",  true, false, DRAW_BLOCK,  COLOR_NONE, {16,16,16,16,16,16}},
	 [5] = {     "Wooden Plank",  true, false, DRAW_BLOCK,  COLOR_NONE, { 4, 4, 4, 4, 4, 4}},
	 [6] = {          "Sapling", false,  true,  DRAW_TREE,  COLOR_NONE, {15,15,15,15,15,15}},
	 [7] = {          "Bedrock",  true, false, DRAW_BLOCK,  COLOR_NONE, {17,17,17,17,17,17}},
	 [8] = {            "Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {205,205,205,205,205,205}},
	 [9] = { "Stationary Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {205,205,205,205,205,205}},
	[10] = {             "Lava",  true, false, DRAW_BLOCK,  COLOR_NONE, {255,255,255,255,255,255}},
	[11] = {  "Stationary Lava",  true, false, DRAW_BLOCK,  COLOR_NONE, {255,255,255,255,255,255}},
	[12] = {             "Sand",  true, false, DRAW_BLOCK,  COLOR_NONE, {18,18,18,18,18,18}},
	[13] = {           "Gravel",  true, false, DRAW_BLOCK,  COLOR_NONE, {19,19,19,19,19,19}},
	[17] = {             "Wood",  true, false, DRAW_BLOCK,  COLOR_NONE, {21,21,20,20,20,20}},
	[18] = {           "Leaves", false,  true, DRAW_BLOCK, COLOR_GRASS, {52,52,52,52,52,52}},
	[24] = {       "Sand Stone",  true, false, DRAW_BLOCK,  COLOR_NONE, {0xd0,0xb0,0xc0,0xc0,0xc0,0xc0}},
	[31] = {       "Tall Grass", false,  true,  DRAW_TREE, COLOR_GRASS, {39,39,39,39}},
	[32] = {      "Dead Shrubs", false,  true,  DRAW_TREE,  COLOR_NONE, {55,55,55,55}},
	[37] = {        "Dandelion", false,  true,  DRAW_TREE,  COLOR_NONE, {13,13,13,13}},
	[38] = {             "Rose", false,  true,  DRAW_TREE,  COLOR_NONE, {12,12,12,12}},
	[81] = {           "Cactus",  true,  true,  DRAW_TREE,  COLOR_NONE, {70,70,70,70}},
};
