/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef COCKPIT_H_INCLUDED
#define COCKPIT_H_INCLUDED

#define SUBMARINE_ONBOARD_COMPUTER_STATE_POWER_ON 1
#define SUBMARINE_ONBOARD_COMPUTER_STATE_SHOW_BOOT_SCREEN 2
#define SUBMARINE_ONBOARD_COMPUTER_STATE_CHECKLIST_RUN_THROUGH 3
#define SUBMARINE_ONBOARD_COMPUTER_STATE_READY_TO_LAUNCH 4



#define SUBMARINE_STATUS_NOT_INSTALLED 1
#define SUBMARINE_STATUS_NOMINAL 2
#define SUBMARINE_STATUS_ISSUE 3 //In yellow
#define SUBMARINE_STATUS_ISSUE_NO_GO 4 //In red

struct _Submarine_Control_Checklist
{
    int Control_Surface_Status;

    int Ballast_Tank_Status;
    int Life_Support_System_Status;
    int Fire_Suppression_System_Status;
    int Ejection_System_Status;
    int Food_And_Water_Storage_Status; //Check if you have no food!

    int Jet_Shields_Status;
    int Tractor_Beam_Status;

    int Torpedo_Status;
    int Saser_Beam_Status;
    int Depth_Charge_Status;
    int Missile_Status;

    int Navigational_Computer_System_Status;

    int Camera_Status;
    int Sonar_Status;
    int Magnetometer_Status;
    int Lidar_Status;

    int Comm_Transceiver_Status;

    int Supersonic_Drive_Status;
    int Subsonic_Engines_Status;
    int Turbine_Generator_Status;
    int Battery_Power_Status;
    int Fuel_Tank_Status;

    int Door_Hatch_And_Bay_Status;

    int Hull_Pressurization_Status;
};

struct _Submarine_Operating_System //Poseidon
{
    struct _Image* Black_Screen;
    struct _Image* Orange_Screen;

    struct _Image* OS_Image;

    struct _Image* Cursor_Image;

    struct _Image* Not_Installed_Image;
    struct _Image* Nominal_Image;
    struct _Image* Issue_Image;
    struct _Image* Issue_No_Go_Image;
};

struct _Submarine_Control_Checklist* Create_Submarine_Control_Checklist();
void Initialize_Submarine_Control_Checklist(struct _Submarine_Control_Checklist* Checklist);

#endif // COCKPIT_H_INCLUDED
