/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   timer.h
 * Author: drew
 *
 * Created on August 12, 2022, 6:01 PM
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

