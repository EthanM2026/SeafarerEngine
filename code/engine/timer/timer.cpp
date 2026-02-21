/*
Copyright (c) 2024 Ethan D. Carbonate.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "timer.h"
#include "GLFW/glfw3.h"
#include "stdlib.h"
struct _Timer* Create_Timer()
{
    struct _Timer* T = (struct _Timer*) calloc(1,sizeof (struct _Timer));
    T->Time = 0;
    T->Elapsed = 0;
    T->Start_Time = 0;
    T->Time_Paused = 0;
    T->Paused = false;
    T->Active = false;
    return T;
}

void Delete_Timer()
{

}

void Set_Timer(struct _Timer* Timer)
{
    Timer->Active = true;
    Timer->Paused = false;

    Timer->Start_Time = glfwGetTime();
    Timer->Time_Paused = 0;
}

void Stop_Timer(struct _Timer* Timer)
{
    Timer->Active = false;
    Timer->Paused = false;

   Timer->Start_Time = 0;
    Timer->Time_Paused = 0;
}

void Pause_Timer(struct _Timer* Timer)
{
if(Timer->Active && !Timer->Paused)
{
    Timer->Paused = true;

    Timer->Time_Paused = glfwGetTime() - Timer->Start_Time;
    Timer->Start_Time = 0;
}
}

void Resume_Timer(struct _Timer* Timer)
{
if(Timer->Active && Timer->Paused)
{
    Timer->Paused = false;

    Timer->Start_Time = glfwGetTime() - Timer->Time_Paused;
    Timer->Time_Paused = 0;
}
}

double Get_Ticks(struct _Timer* Timer)
{
    Timer->Time = 0;
    if(Timer->Active)
    {
        if(Timer->Paused)
        {
            Timer->Time = Timer->Time_Paused;
        }
        else if(!Timer->Paused)
        {
            Timer->Time = glfwGetTime() - Timer->Start_Time;
        }
    }
    return Timer->Time;
}

bool Get_Active(struct _Timer* Timer)
{
    return Timer->Active;
}

bool Get_Paused(struct _Timer* Timer)
{
    return Timer->Paused && Timer->Active;
}












