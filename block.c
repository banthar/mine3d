
#include "config.h"

#include "block.h"

#include "world.h"

public BlockDefinition block_definition[256]=
{
//   id|                    name | solid|transp|  draw mode|  color mode| textures
     [0] = {                "Air", false,  true,  DRAW_NONE,  COLOR_NONE, {}},
     [1] = {              "Stone",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x01,0x01,0x01,0x01,0x01,0x01}},
     [2] = {              "Grass",  true, false, DRAW_GRASS, COLOR_GRASS, {0xb4,0x28,0x26,0x26,0x26,0x26}},
     [3] = {               "Dirt",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x02,0x02,0x02,0x02,0x02,0x02}},
     [4] = {       "Cobblerstone",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x10,0x10,0x10,0x10,0x10,0x10}},
     [5] = {       "Wooden Plank",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x04,0x04,0x04,0x04,0x04,0x04}},
     [6] = {            "Sapling", false,  true,  DRAW_TREE,  COLOR_NONE, {0x0f,0x0f,0x0f,0x0f}},
     [7] = {            "Bedrock",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x11,0x11,0x11,0x11,0x11,0x11}},
     [8] = {              "Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0x90,0x90,0x90,0x90,0x90,0x90}},
     [9] = {   "Stationary Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0x90,0x90,0x90,0x90,0x90,0x90}},
//   [8] = {              "Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0xcd,0xcd,0xcd,0xcd,0xcd,0xcd}},
//   [9] = {   "Stationary Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0xcd,0xcd,0xcd,0xcd,0xcd,0xcd}},
    [10] = {               "Lava",  false, true, DRAW_BLOCK,  COLOR_NONE, {255,255,255,255,255,255}},
    [11] = {    "Stationary Lava",  false, true, DRAW_BLOCK,  COLOR_NONE, {255,255,255,255,255,255}},
    [12] = {               "Sand",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x12,0x12,0x12,0x12,0x12,0x12}},
    [13] = {             "Gravel",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x13,0x13,0x13,0x13,0x13,0x13}},
    [14] = {           "Gold Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x20,0x20,0x20,0x20,0x20,0x20}},
    [15] = {           "Iron Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x21,0x21,0x21,0x21,0x21,0x21}},
    [16] = {           "Coal Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x22,0x22,0x22,0x22,0x22,0x22}},
    [17] = {               "Wood",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x15,0x15,0x14,0x14,0x14,0x14}},
    [18] = {             "Leaves",  true,  true, DRAW_BLOCK, COLOR_GRASS, {0x34,0x34,0x34,0x34,0x34,0x34}},
    [19] = {             "Sponge",  true, false,   DRAW_BLOCK,COLOR_NONE,{0x30,0x30,0x30,0x30,0x30,0x30}},
    [20] = {              "Glass",  true,  true, DRAW_BLOCK,COLOR_NONE,{0x43,0x43,0x43,0x43,0x43,0x43}},
    [21] = {   "Lapis Lazuli Ore",  true, false,DRAW_BLOCK,COLOR_NONE,{0xA0,0xA0,0xA0,0xA0,0xA0,0xA0}},
    [22] = { "Lapis Lazuli Block",  true, false,DRAW_BLOCK,COLOR_NONE,{0x90,0x90,0x90,0x90,0x90,0x90}},
    [23] = {          "Dispenser",  true, false,DRAW_BLOCK,COLOR_NONE,{0x3e,0x3e,0x2e,0x2d,0x2d,0x2d}},
    [24] = {         "Sand Stone",  true, false, DRAW_BLOCK,  COLOR_NONE, {0xd0,0xb0,0xc0,0xc0,0xc0,0xc0}},
    [25] = {         "Note Block",  true, false,DRAW_BLOCK,COLOR_NONE,{0x4A,0x4A,0x4A,0x4A,0x4A,0x4A}},
    [26] = {                "Bed",  true,  true,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [27] = {       "Powered Rail", false,  true,DRAW_RAIL,COLOR_NONE,{0xB3}},
    [28] = {      "Detector Rail",  true,  true,DRAW_RAIL,COLOR_NONE,{0xC3}},
    [29] = {      "Sticky Piston",  true,  true,DRAW_BLOCK,COLOR_NONE,{0x6A,0x6D,0x6C,0x6C,0x6C,0x6C}},
    [30] = {             "Cobweb", false,  true,DRAW_TREE,COLOR_NONE,{0x0B,0x0B,0x0B,0x0B}},
    [31] = {         "Tall Grass", false,  true,  DRAW_TREE, COLOR_GRASS, {0x27,0x27,0x27,0x27}},
    [32] = {        "Dead Shrubs", false,  true,  DRAW_TREE,  COLOR_NONE, {0x37,0x37,0x37,0x37}},
    [33] = {             "Piston",  true, false,DRAW_BLOCK,COLOR_NONE,{0x6B,0x6D,0x6C,0x6C,0x6C,0x6C}},
    [34] = {   "Piston Extension",  true,  true,DRAW_NONE,COLOR_NONE,{0x6B,0x6B,0x00,0x00,0x00,0x00}},
    [35] = {               "Wool",  true, false,DRAW_BLOCK,COLOR_WOOL,{0x40,0x40,0x40,0x40,0x40,0x40}},
    [36] = { "Block moved By Piston",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [37] = {         "Dandelion", false,  true,  DRAW_TREE,  COLOR_NONE, {0x0D,0x0D,0x0D,0x0D}},
    [38] = {             "Rose", false,  true,  DRAW_TREE,  COLOR_NONE, {0x0C,0x0C,0x0C,0x0C}},
    [39] = {   "Brown Mushroom",false,true,DRAW_TREE,COLOR_NONE,{0x1D,0x1D,0x1D,0x1D,0x1D,0x1D}},
    [40] = {     "Red Mushroom",false,true,DRAW_TREE,COLOR_NONE,{0x1C,0x1C,0x1C,0x1C,0x1C,0x1C}},
    [41] = {       "Gold Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x17,0x17,0x17,0x17,0x17,0x17}},
    [42] = {       "Iron Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x16,0x16,0x16,0x16,0x16,0x16}},
    [43] = {     "Double Slabs",true,false,DRAW_BLOCK,COLOR_NONE,{0x06,0x06,0x05,0x05,0x05,0x05}},
    [44] = {            "Slabs",true,false,DRAW_BLOCK,COLOR_NONE,{0x06,0x06,0x05,0x05,0x05,0x05}},
    [45] = {      "Brick Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x07,0x07,0x07,0x07,0x07,0x07}},
    [46] = {              "TNT",true,false,DRAW_BLOCK,COLOR_NONE,{0x0A,0x09,0x08,0x08,0x08,0x08}},
    [47] = {        "Bookshelf",true,false,DRAW_BLOCK,COLOR_NONE,{0x04,0x04,0x23,0x23,0x23,0x23}},
    [48] = {         "Moss Stone",true,false,DRAW_BLOCK,COLOR_NONE,{0x24,0x24,0x24,0x24,0x24,0x24}},
    [49] = {           "Obsidian",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x25,0x25,0x25,0x25,0x25,0x25}},
    [50] = {              "Torch",false,true,DRAW_BLOCK,COLOR_NONE,{0x50,0x50,0x50,0x50,0x50,0x50}},
    [51] = {               "Fire",false,true,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [52] = {    "Monster Spawner",true,true,DRAW_BLOCK,COLOR_NONE,{0x41,0x41,0x41,0x41,0x41,0x41}},
    [53] = {      "Wooden Stairs",true,false,DRAW_BLOCK,COLOR_NONE,{0x04,0x04,0x04,0x04,0x04,0x04}},
    [54] = {              "Chest",true,false,DRAW_BLOCK,COLOR_NONE,{0x19,0x19,0x1B,0x1A,0x1A,0x1A}},
    [55] = {    "Redstone Wire I",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [56] = {        "Diamond Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x32,0x32,0x32,0x32,0x32,0x32}},
    [57] = {      "Diamond Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x18,0x18,0x18,0x18,0x18,0x18}},
    [58] = {     "Crafting Table",true,false,DRAW_BLOCK,COLOR_NONE,{0x04,0x2B,0x3B,0x3C,0x3B,0x3C}},
    [59] = {              "Seeds",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [60] = {           "Farmland",true,false,DRAW_BLOCK,COLOR_NONE,{0x02,0x57,0x02,0x02,0x02,0x02}},
    [61] = {            "Furnace",true,false,DRAW_BLOCK,COLOR_NONE,{0x3E,0x3E,0x2C,0x2D,0x2D,0x2D}},
    [62] = {      "Burning Furnace",true,false,DRAW_BLOCK,COLOR_NONE,{0x3E,0x3E,0x3D,0x2D,0x2D,0x2D}},
    [63] = {            "Sign Post",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [64] = {          "Wooden Door",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [65] = {              "Ladders",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [66] = {                "Rails",true,false,DRAW_RAIL,COLOR_NONE,{0x80}},
    [67] = {   "Cobblestone Stairs",true,false,DRAW_STAIRS,COLOR_NONE,{0x10,0x10,0x10,0x10,0x10,0x10}},
    [68] = {              "Wall Sign",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [69] = {                  "Lever",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [70] = {   "Stone Pressure Plate",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [71] = {              "Iron Door",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [72] = {  "Wooden Pressure Plate",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [73] = {           "Redstone Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x33,0x33,0x33,0x33,0x33,0x33}},
    [74] = {   "Glowing Redstone Ore",true,false,DRAW_BLOCK,COLOR_NONE,{0x33,0x33,0x33,0x33,0x33,0x33}},
    [75] = {     "Redstone Torch off",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [76] = {      "Redstone Torch on",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [77] = {           "Stone Button",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [78] = {                   "Snow",true,false,DRAW_BLOCK,COLOR_NONE,{0x42,0x42,0x42,0x42,0x42,0x42}},
    [79] = {                    "Ice",true,false,DRAW_BLOCK,COLOR_NONE,{0x43,0x43,0x43,0x43,0x43,0x43}},
    [80] = {             "Snow Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x42,0x42,0x42,0x42,0x42,0x42}},
    [81] = {                 "Cactus",  true,  true,  DRAW_TREE,  COLOR_NONE, {0x47,0x45,0x46,0x46,0x46,0x46}},
    [82] = {             "Clay Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x48,0x48,0x48,0x48,0x48,0x48}},
    [83] = {             "Sugar Cane",  true,  true,  DRAW_TREE,  COLOR_NONE, {0x49,0x49,0x49,0x49}},
    [84] = {                "Jukebox",true,false,DRAW_BLOCK,COLOR_NONE,{0x4A,0x4B,0x4A,0x4A,0x4A,0x4A}},
    [85] = {                  "Fence",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [86] = {                "Pumpkin",true,false,DRAW_BLOCK,COLOR_NONE,{0x66,0x66,0x77,0x76,0x76,0x76}},
    [87] = {             "Netherrack",true,false,DRAW_BLOCK,COLOR_NONE,{0x67,0x67,0x67,0x67,0x67,0x67}},
    [88] = {              "Soul Sand",true,false,DRAW_BLOCK,COLOR_NONE,{0x68,0x68,0x68,0x68,0x68,0x68}},
    [89] = {        "Glowstone Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x69,0x69,0x69,0x69,0x69,0x69}},
    [90] = {                 "Portal",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [91] = {         "Jack-O-Lantern",true,false,DRAW_BLOCK,COLOR_NONE,{0x66,0x66,0x78,0x76,0x76,0x76}},
    [92] = {             "Cake Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [93] = {  "Redstone Repeater off",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [94] = {   "Redstone Repeater on",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [95] = {           "Locked Chest",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
    [96] = {               "Trapdoor",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},

};

static void addVertex(VertexBuffer* buffer, Vertex v)
{
    assert(buffer->size<buffer->maxSize);
    buffer->data[buffer->size++]=v;
}

void blockDrawCube(World* world, Vec4i loc, VertexBuffer* buffer, Vec4f baseColor, int textures[])
{

    Block block=worldGet(world,loc);

    static const Vec4i face[6][4]={
        {{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
        {{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

        {{1,0,0},{0,0,0},{0,0,1},{1,0,1}},
        {{0,1,0},{1,1,0},{1,1,1},{0,1,1}},

        {{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
        {{1,1,0},{1,0,0},{1,0,1},{1,1,1}},
    };

    static const int texCoord[4][2]={
        {0,1},{1,1},{1,0},{0,0},
    };

    static const Vec4i normal[6]={
        { 0, 0,-1},
        { 0, 0, 1},
        { 0,-1, 0},
        { 0, 1, 0},
        {-1, 0, 0},
        { 1, 0, 0},
    };

    static const Vec4i light_vertex_mask[6][4]={
        {{ 0, 0, 1},{ 0, 1, 1},{ 1, 0, 1},{ 1, 1, 1}},
        {{ 0, 0, 1},{ 0, 1, 1},{ 1, 0, 1},{ 1, 1, 1}},
        {{ 0, 1, 0},{ 0, 1, 1},{ 1, 1, 0},{ 1, 1, 1}},
        {{ 0, 1, 0},{ 0, 1, 1},{ 1, 1, 0},{ 1, 1, 1}},
        {{ 1, 0, 0},{ 1, 0, 1},{ 1, 1, 0},{ 1, 1, 1}},
        {{ 1, 0, 0},{ 1, 0, 1},{ 1, 1, 0},{ 1, 1, 1}},
    };

    for(int i=0;i<6;i++)
    {

        Block normalBlock=worldGet(world,loc+normal[i]);

        if(block_definition[normalBlock.id].transparent==false)
            continue;

        if((block.id==8 || block.id==9) && block.id==normalBlock.id)
            continue;

        int tile_id=textures[i];

        int tileX=tile_id%TEXTURE_SIZE;
        int tileY=tile_id/TEXTURE_SIZE;

        for(int v=0;v<4;v++)
        {

            Vec4i vertex_normal=face[i][v]*(Vec4i){2,2,2}-(Vec4i){1,1,1};
            float light=0.0;
            float sky_light=0.0;

            for(int j=0;j<4;j++)
            {
                Block b=worldGet(world,loc+vertex_normal*light_vertex_mask[i][j]);
                light+=b.light;
                sky_light+=b.skyLight;
            }

            light/=4.0;
            sky_light/=4.0;

            float l=clampf(light+sky_light,0.1,1.0);
            //l=l>0.75;
            Vertex vertex={
                .pos=loc+face[i][v],
                .color=(Vec4f){l,l,l,1}*baseColor,
                .texCoord=(Vec2f){(texCoord[v][0]+tileX)*1.0/TEXTURE_SIZE,(texCoord[v][1]+tileY)*1.0/TEXTURE_SIZE},
            };

            addVertex(buffer,vertex);

        }

    }
}

void blockDrawTree(World* world, Vec4i loc, VertexBuffer* buffer,Vec4f baseColor, int textures[])
{

    Block block=worldGet(world,loc);

    static const Vec4i tree_face[4][4]={
        {{1,0,0},{0,1,0},{0,1,1},{1,0,1}},
        {{0,1,0},{1,0,0},{1,0,1},{0,1,1}},
        {{1,1,0},{0,0,0},{0,0,1},{1,1,1}},
        {{0,0,0},{1,1,0},{1,1,1},{0,0,1}},
    };

    static const int texCoord[4][2]={
        {0,1},{1,1},{1,0},{0,0},
    };

    for(int i=0;i<4;i++)
    {
        int tile_id=textures[i];

        int tileX=tile_id%TEXTURE_SIZE;
        int tileY=tile_id/TEXTURE_SIZE;

        for(int v=0;v<4;v++)
        {

            float light=clampf(block.light+block.skyLight,0.2,1.0);

            Vertex vertex={
                .pos=loc+tree_face[i][v],
                .color=(Vec4f){light,light,light,1}*baseColor,
                .texCoord=(Vec2f){(texCoord[v][0]+tileX)*1.0/TEXTURE_SIZE,(texCoord[v][1]+tileY)*1.0/TEXTURE_SIZE},
            };

            addVertex(buffer,vertex);

        }

    }
}

void blockDraw(World* world, Vec4i loc, VertexBuffer* buffer)
{

    Block block=worldGet(world,loc);

    Vec4f baseColor;

    #define RGB8(r,g,b) (Vec4f){r/255.0,g/255.0,b/255.0,1.0}

    Vec4f woolColors[16]={
        RGB8(222, 223, 222),
        RGB8(234, 127,  57),
        RGB8(192,  77, 200),
        RGB8(105, 140, 211),
        RGB8(194, 182,  36),
        RGB8( 56, 189,  52),
        RGB8(217, 132, 155),
        RGB8( 66,  66,  66),
        RGB8(158, 166, 166),
        RGB8( 39, 117, 149),
        RGB8(130,  55, 195),
        RGB8( 40,  52, 153),
        RGB8( 85,  50,  28),
        RGB8( 55,  76,  25),
        RGB8(164,  45,  40),
        RGB8( 26,  22,  22),
    };

    switch(block_definition[block.id].color_mode)
    {
        case COLOR_NONE:
            baseColor=(Vec4f){1.0,1.0,1.0,1.0};
            break;
        case COLOR_GRASS:
            baseColor=(Vec4f){0.2,0.9,0.1,1.0};
            break;
        case COLOR_WOOL:
            baseColor=woolColors[block.metadata&0xf];
            break;
        default:
            panic("unknown color mode");
    }

    switch(block_definition[block.id].draw_mode)
    {
        case DRAW_TREE:
            blockDrawTree(world,loc,buffer,baseColor,block_definition[block.id].textures);
            break;
        case DRAW_BLOCK:
            blockDrawCube(world,loc,buffer,baseColor,block_definition[block.id].textures);
            break;
        case DRAW_GRASS:
            blockDrawCube(world,loc,buffer,baseColor,block_definition[block.id].textures);
            blockDrawCube(world,loc,buffer,(Vec4f){1.0,1.0,1.0,1.0},block_definition[3].textures);
            break;
        default:
            break;
    }


}

