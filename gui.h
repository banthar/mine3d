
#pragma once

#include "config.h"

#include "SDL.h"
#include "math.h"

typedef struct Frame Frame;
typedef struct Container Container;
typedef struct Button Button;
typedef struct Slot Slot;
typedef struct Label Label;
typedef struct Icon Icon;
typedef struct ScrollBar ScrollBar;
typedef struct Rectangle Rectangle;
typedef struct Window Window;

struct Rectangle
{
    Vec2f pos;
    Vec2f size;
};

typedef enum
{
    CONTAINER,
    BUTTON,
    SLOT,
    LABEL,
    ICON,
    SCROLL_BAR,
    CANVAS,
}FrameType;

struct Frame
{
    FrameType type;
    Vec2f pos;
    Vec2f size;
};

struct Container
{
    Frame;
    bool background;
    bool scroll;
    int childs;
    Frame* child[];
};

struct Button
{
    Frame;
    bool enabled;
    const char* text;
};

struct Slot
{
    Frame;
    bool enabled;
    void* data;
};

struct Label
{
    Frame;
    const char* text;
};

struct Icon
{
    Frame;
    uint texture;
    Rectangle scrollRect;
};

struct Window
{
    bool visible;
    bool enabled;
    const Frame* frame;
};

typedef struct
{
    Frame* equipment;
}Layouts;

extern Layouts layouts;

public void windowDraw(Window* window);
public bool windowEvent(Window* window, SDL_Event* event);
