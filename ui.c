
#include "ui.h"

#include "gui.h"

Window mainMenu={
    {{CONTAINER,{-120,-120},{240,240}},0},
    false,
    {
        {.button={{BUTTON,{20,108},{200,20},1}}},
        {.button={{BUTTON,{20,132},{200,20},2}}},
        {.button={{BUTTON,{20,156},{200,20},3}}},
        {.button={{BUTTON,{20,192},{100,20},4}}},
        {.button={{BUTTON,{122,192},{100,20},-1}}},
    }
};

Window equipment={
    {{CONTAINER,{-88,-86},{176,166}},0},
    true,
    {
        {.slot={{SLOT,{144,36},{18,18},1},true,NULL}},
        {.slot={{SLOT,{88,26},{18,18},2},true,NULL}},

        {.slot={{SLOT,{144,36},{18,18},3},true,NULL}},

        {.slot={{SLOT,{88,26},{18,18},4},true,NULL}},
        {.slot={{SLOT,{88,26+18},{18,18},5},true,NULL}},
        {.slot={{SLOT,{88+18,26},{18,18},6},true,NULL}},
        {.slot={{SLOT,{88+18,26+18},{18,18},7},true,NULL}},

        {.slot={{SLOT,{7,7+18*0},{18,18},8},true,NULL}},
        {.slot={{SLOT,{7,7+18*1},{18,18},9},true,NULL}},
        {.slot={{SLOT,{7,7+18*2},{18,18},10},true,NULL}},
        {.slot={{SLOT,{7,7+18*3},{18,18},11},true,NULL}},

        {.slot={{SLOT,{7+18*0,83},{18,18},12},true,NULL}},
        {.slot={{SLOT,{7+18*1,83},{18,18},13},true,NULL}},
        {.slot={{SLOT,{7+18*2,83},{18,18},14},true,NULL}},
        {.slot={{SLOT,{7+18*3,83},{18,18},15},true,NULL}},
        {.slot={{SLOT,{7+18*4,83},{18,18},16},true,NULL}},
        {.slot={{SLOT,{7+18*5,83},{18,18},17},true,NULL}},
        {.slot={{SLOT,{7+18*6,83},{18,18},18},true,NULL}},
        {.slot={{SLOT,{7+18*7,83},{18,18},19},true,NULL}},
        {.slot={{SLOT,{7+18*8,83},{18,18},20},true,NULL}},

        {.slot={{SLOT,{7+18*0,83+18},{18,18},21},true,NULL}},
        {.slot={{SLOT,{7+18*1,83+18},{18,18},22},true,NULL}},
        {.slot={{SLOT,{7+18*2,83+18},{18,18},23},true,NULL}},
        {.slot={{SLOT,{7+18*3,83+18},{18,18},24},true,NULL}},
        {.slot={{SLOT,{7+18*4,83+18},{18,18},25},true,NULL}},
        {.slot={{SLOT,{7+18*5,83+18},{18,18},26},true,NULL}},
        {.slot={{SLOT,{7+18*6,83+18},{18,18},27},true,NULL}},
        {.slot={{SLOT,{7+18*7,83+18},{18,18},28},true,NULL}},
        {.slot={{SLOT,{7+18*8,83+18},{18,18},29},true,NULL}},

        {.slot={{SLOT,{7+18*0,83+18*2},{18,18},30},true,NULL}},
        {.slot={{SLOT,{7+18*1,83+18*2},{18,18},31},true,NULL}},
        {.slot={{SLOT,{7+18*2,83+18*2},{18,18},32},true,NULL}},
        {.slot={{SLOT,{7+18*3,83+18*2},{18,18},33},true,NULL}},
        {.slot={{SLOT,{7+18*4,83+18*2},{18,18},34},true,NULL}},
        {.slot={{SLOT,{7+18*5,83+18*2},{18,18},35},true,NULL}},
        {.slot={{SLOT,{7+18*6,83+18*2},{18,18},36},true,NULL}},
        {.slot={{SLOT,{7+18*7,83+18*2},{18,18},37},true,NULL}},
        {.slot={{SLOT,{7+18*8,83+18*2},{18,18},38},true,NULL}},

        {.slot={{SLOT,{7+18*0,142},{18,18},39},true,NULL}},
        {.slot={{SLOT,{7+18*1,142},{18,18},40},true,NULL}},
        {.slot={{SLOT,{7+18*2,142},{18,18},41},true,NULL}},
        {.slot={{SLOT,{7+18*3,142},{18,18},42},true,NULL}},
        {.slot={{SLOT,{7+18*4,142},{18,18},43},true,NULL}},
        {.slot={{SLOT,{7+18*5,142},{18,18},44},true,NULL}},
        {.slot={{SLOT,{7+18*6,142},{18,18},45},true,NULL}},
        {.slot={{SLOT,{7+18*7,142},{18,18},46},true,NULL}},
        {.slot={{SLOT,{7+18*8,142},{18,18},-1},true,NULL}},
    }
};

/*
Window equipment{
    &(CONTAINER){
    };

};

static Frame equipmentFrame={CONTAINER,{-88,-86},{176,166}};



static Frame equipmentPlayerView={CANVAS,{26,7},{54,72}};
static Icon equipmentCraftingIcon={{ICON,{125,37},{16,13}}};
static Label equipmentCraftingLabel={{CANVAS,{26,7},{54,72}}};
*/

