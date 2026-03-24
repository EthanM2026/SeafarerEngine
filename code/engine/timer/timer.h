/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TIMER_H
#define TIMER_H
#include "stdbool.h"
struct _Timer
    {
        double Time;
        double Elapsed;
        double Start_Time;
        double Time_Paused;
        bool Paused;
        bool Active;
    };


    struct _Timer* Create_Timer();
    void Delete_Timer();

    bool Get_Active(struct _Timer* Timer);
    bool Get_Paused(struct _Timer* Timer);
    void Set_Timer(struct _Timer* Timer);
    void Pause_Timer(struct _Timer* Timer);
    void Resume_Timer(struct _Timer* Timer);
    void Stop_Timer(struct _Timer* Timer);
    double Get_Ticks(struct _Timer* Timer);

#endif /* TIMER_H */

