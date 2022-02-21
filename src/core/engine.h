#pragma once

/*
 *  Graphic Engine
 */

#include "core/window/window_object.h"


class Engine
{
 public:
    static Windowobject* Init(const WindowProperties &props);

    static Windowobject* GetWindow();

    // Get elapsed time in seconds since the application started
    static double GetElapsedTime();

    static void Exit();

 private:
    static Windowobject* window;
};
