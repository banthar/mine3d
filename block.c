
#include "config.h"

#include "block.h"

#include "world.h"

public BlockDefinition block_definition[256]=
{
//   id|                    name | solid|transp|  draw mode|  color mode| textures
	 [0] = {                "Air", false,  true,  DRAW_NONE,  COLOR_NONE, { 0, 0, 0, 0, 0, 0}},
	 [1] = {              "Stone",  true, false, DRAW_BLOCK,  COLOR_NONE, { 1, 1, 1, 1, 1, 1}},
	 [2] = {              "Grass",  true, false, DRAW_GRASS, COLOR_GRASS, {0xb4,0x28, 0x26, 0x26, 0x26, 0x26}},
	 [3] = {               "Dirt",  true, false, DRAW_BLOCK,  COLOR_NONE, { 2, 2, 2, 2, 2, 2}},
	 [4] = {       "Cobblerstone",  true, false, DRAW_BLOCK,  COLOR_NONE, {16,16,16,16,16,16}},
	 [5] = {       "Wooden Plank",  true, false, DRAW_BLOCK,  COLOR_NONE, { 4, 4, 4, 4, 4, 4}},
	 [6] = {            "Sapling", false,  true,  DRAW_TREE,  COLOR_NONE, {15,15,15,15,15,15}},
	 [7] = {            "Bedrock",  true, false, DRAW_BLOCK,  COLOR_NONE, {17,17,17,17,17,17}},
	 [8] = {              "Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0x90,0x90,0x90,0x90,0x90,0x90}},
	 [9] = {   "Stationary Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0x90,0x90,0x90,0x90,0x90,0x90}},
//	 [8] = {              "Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0xcd,0xcd,0xcd,0xcd,0xcd,0xcd}},
//	 [9] = {   "Stationary Water", false,  true, DRAW_BLOCK,  COLOR_NONE, {0xcd,0xcd,0xcd,0xcd,0xcd,0xcd}},
	[10] = {               "Lava",  true, false, DRAW_BLOCK,  COLOR_NONE, {255,255,255,255,255,255}},
	[11] = {    "Stationary Lava",  true, false, DRAW_BLOCK,  COLOR_NONE, {255,255,255,255,255,255}},
	[12] = {               "Sand",  true, false, DRAW_BLOCK,  COLOR_NONE, {18,18,18,18,18,18}},
	[13] = {             "Gravel",  true, false, DRAW_BLOCK,  COLOR_NONE, {19,19,19,19,19,19}},
	[14] = {           "Gold Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x20,0x20,0x20,0x20,0x20,0x20}},
	[15] = {           "Iron Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x21,0x21,0x21,0x21,0x21,0x21}},
	[16] = {           "Coal Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x22,0x22,0x22,0x22,0x22,0x22}},
	[17] = {               "Wood",  true, false, DRAW_BLOCK,  COLOR_NONE, {21,21,20,20,20,20}},
	[18] = {             "Leaves", false,  true, DRAW_BLOCK, COLOR_GRASS, {52,52,52,52,52,52}},
	[19] = {             "Sponge",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[20] = {              "Glass",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[21] = {   "Lapis Lazuli Ore",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[22] = { "Lapis Lazuli Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[23] = {          "Dispenser",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[24] = {         "Sand Stone",  true, false, DRAW_BLOCK,  COLOR_NONE, {0xd0,0xb0,0xc0,0xc0,0xc0,0xc0}},
	[25] = {           "Notelock",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[26] = {                "Bed",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[27] = {       "Powered Rail",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[28] = {      "Detector Rail",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[29] = {      "Sticky Piston",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[30] = {             "Cobweb",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[31] = {         "Tall Grass", false,  true,  DRAW_TREE, COLOR_GRASS, {39,39,39,39}},
	[32] = {        "Dead Shrubs", false,  true,  DRAW_TREE,  COLOR_NONE, {55,55,55,55}},
	[33] = {             "Piston",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[34] = {   "Piston Extension",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[35] = {               "Wool",true,false,DRAW_BLOCK,COLOR_WOOL,{0xe1,0xe1,0xe1,0xe1,0xe1,0xe1}},
	[36] = { "Block moved By Piston",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[37] = {        "Dandelion", false,  true,  DRAW_TREE,  COLOR_NONE, {13,13,13,13}},
	[38] = {             "Rose", false,  true,  DRAW_TREE,  COLOR_NONE, {12,12,12,12}},
	[39] = {   "Brown Mushroom",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[40] = {     "Red Mushroom",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[41] = {         "Goldlock",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[42] = {         "Ironlock",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[43] = {     "Double Slabs",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[44] = {            "Slabs",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[45] = {      "Brick Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[46] = {              "TNT",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[47] = {        "Bookshelf",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[48] = {         "Moss Stone",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[49] = {           "Obsidian",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x25,0x25,0x25,0x25,0x25,0x25}},
	[50] = {              "Torch",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[51] = {               "Fire",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[52] = {    "Monster Spawner",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[53] = {      "Wooden Stairs",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[54] = {              "Chest",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[55] = {    "Redstone Wire I",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[56] = {        "Diamond Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x32,0x32,0x32,0x32,0x32,0x32}},
	[57] = {      "Diamond Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[58] = {     "Crafting Table",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[59] = {              "Seeds",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[60] = {           "Farmland",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[61] = {            "Furnace",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[62] = {      "Burning Furnace",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[63] = {            "Sign Post",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[64] = {          "Wooden Door",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[65] = {              "Ladders",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[66] = {                "Rails",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[67] = {   "Cobblestone Stairs",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[68] = {              "Wall Sign",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[69] = {                  "Lever",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[70] = {   "Stone Pressure Plate",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[71] = {              "Iron Door",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[72] = {  "Wooden Pressure Plate",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[73] = {           "Redstone Ore",  true, false, DRAW_BLOCK,  COLOR_NONE, {0x33,0x33,0x33,0x33,0x33,0x33}},
	[74] = {   "Glowing Redstone Ore",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[75] = {     "Redstone Torch off",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[76] = {      "Redstone Torch on",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[77] = {           "Stone Button",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[78] = {                   "Snow",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[79] = {                    "Ice",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[80] = {             "Snow Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[81] = {                 "Cactus",  true,  true,  DRAW_TREE,  COLOR_NONE, {70,70,70,70}},
	[82] = {             "Clay Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[83] = {             "Sugar Cane",  true,  true,  DRAW_TREE,  COLOR_NONE, {0x49,0x49,0x49,0x49}},
	[84] = {                "Jukebox",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[85] = {                  "Fence",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[86] = {                "Pumpkin",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[87] = {             "Netherrack",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[88] = {              "Soul Sand",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[89] = {        "Glowstone Block",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[90] = {                 "Portal",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
	[91] = {         "Jack-O-Lantern",true,false,DRAW_BLOCK,COLOR_NONE,{0x00,0x00,0x00,0x00,0x00,0x00}},
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

