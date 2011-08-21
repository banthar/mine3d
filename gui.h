
#pragma once

#include "config.h"

#include "SDL.h"
#include "vmath.h"

typedef struct Rectangle Rectangle;

typedef struct Component Component;
typedef struct Container Container;
typedef struct Button Button;
typedef struct Slot Slot;
typedef struct Label Label;
typedef struct Icon Icon;
typedef struct ScrollBar ScrollBar;

typedef struct Window Window;

typedef struct Event Event;


typedef enum
{
    NONE,
    KEY_DOWN,
    KEY_UP,
    MOUSE_MOTION,
    MOUSE_UP,
    MOUSE_DOWN,
}EventType;

struct Event
{
    EventType type;

    Vec2f mouse;
    int button;

    int keyCode;
    int buttonState;
    int keyState;
};

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
}ComponentType;

struct Component
{
    ComponentType type;
    Vec2f pos;
    Vec2f size;
    int nextChild;
};

struct Container
{
    Component;
    int firstChild;
};

struct Button
{
    Component;
    bool enabled;
    const char* text;
};

struct Slot
{
    Component;
    bool enabled;
    void* data;
};

struct Label
{
    Component;
    const char* text;
};

struct Icon
{
    Component;
    uint texture;
    Rectangle scrollRect;
};

struct Window
{

    Container;
    bool background;
    union
    {
        Component component;
        Container container;
        Button button;
        Slot slot;
        Label label;
        Icon icon;
    }component[];

};

public void windowDraw(Window* window);
public bool windowEvent(Window* client,Event* event);
