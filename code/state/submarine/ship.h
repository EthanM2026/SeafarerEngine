/*
Copyright (c) 2026 Ethan Mortonson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

struct _Ship_Information
{
    char Ship_Name[256];
    char Ship_Prefix[2];
    unsigned int Ship_Number;
};

struct _Network_Node
{
    int Category;
    int Role;
    int ID;

    float Relative_x;
    float Relative_y;
    float Relative_z;

    float Roll;
    float Yaw;
    float Pitch;

    unsigned char Model_Filepath[256];
    unsigned char Texture_Filepath[256];

    unsigned char Damaged_Model_Filepath[256];
    unsigned char Damaged_Texture_Filepath[256];

    unsigned char Destroyed_Model_Filepath[256];
    unsigned char Destroyed_Texture_Filepath[256];

    unsigned char Model_Collision_Mesh_Filepath[256];
    unsigned char Damaged_Model_Collision_Mesh_Filepath[256];
    unsigned char Destroyed_Model_Collision_Mesh_Filepath[256];
}

struct _Network_Connection
{
    int Category[2];
    int Role[2];
    int ID[2];

    float Connection_Strength;

    unsigned char Model_Filepath[256];
    unsigned char Texture_Filepath[256];

    unsigned char Damaged_Model_Filepath[256];
    unsigned char Damaged_Texture_Filepath[256];

    unsigned char Destroyed_Model_Filepath[256];
    unsigned char Destroyed_Texture_Filepath[256];

    unsigned char Model_Collision_Mesh_Filepath[256];
    unsigned char Damaged_Model_Collision_Mesh_Filepath[256];
    unsigned char Destroyed_Model_Collision_Mesh_Filepath[256];
};

struct _Ship_Electrical_Grid
{
    struct _Network_Node Antimatter_Reactor;
    struct _Network_Node Pressurizer;
    struct _Network_Node Control_Rods;
    struct _Network_Node Steam_Generator;
    struct _Network_Node Coolant_Pump;

    struct _Network_Node Condenser_Pump;
    struct _Network_Node Motor_Condenser;
    struct _Network_Node Motor_Generator;
    struct _Network_Node Turbo_Generator;
    struct _Network_Node Main_Engine_Throttle;
    struct _Network_Node Main_Turbine;

    struct _Network_Node Reduction_Gearing;
    struct _Network_Node Clutch;
    struct _Network_Node Electric_Propulsion_Motor;
    struct _Network_Node Thrust_Block;

    struct _Network_Node Emergency_Battery;

    struct _Network_Node Main_Bus; //GETS POWER FROM THE TURBINE!!!
};

struct _Computer_Node
{
    int Category;
    int Role;
    int ID;

    float Relative_x;
    float Relative_y;
    float Relative_z;

    float Roll;
    float Yaw;
    float Pitch;

    unsigned char Model_Filepath[256];
    unsigned char Texture_Filepath[256];

    unsigned char Damaged_Model_Filepath[256];
    unsigned char Damaged_Texture_Filepath[256];

    unsigned char Destroyed_Model_Filepath[256];
    unsigned char Destroyed_Texture_Filepath[256];

    unsigned char Model_Collision_Mesh_Filepath[256];
    unsigned char Damaged_Model_Collision_Mesh_Filepath[256];
    unsigned char Destroyed_Model_Collision_Mesh_Filepath[256];
};

struct _Ship_Sensor_Computer_Weapons_Grid
{
    struct _Aegis_Combat_System Aegis_Combat_System; //Weapon control, threat tracking, missile guidance.
    struct _Ship_Self_Defense System Ship_Self_Defense System; //Integrates sensors and weapons for defense against air/surface threats.
    struct _Command_Decision_Software Command_Decision_Software; //Tactical decision support, threat assessment, and engagement planning.
    struct _Gun_Computer_System Gun_Computer_System; //Fire control for naval guns.

    struct _Link_16_Terminal_Software Link_16_Terminal_Software; //Tactical data link for secure information sharing with other ships, aircraft, and command centers.
    struct _Cooperative_Engagement_Capability_Software Cooperative_Engagement_Capability_Software; //Networked fire control and sensor fusion.
    struct _Tactical_Display_System Tactical_Display_System; //Radar, sonar, and tracking visualization.

    struct _Radar_Display_Software //Tracks air, surface, and missile threats.
    struct _Sonar_Processing_Software //Anti-submarine warfare detection and classification.
    struct _Electronic_Warfare_Suite_Software //Monitors, analyzes, and counters enemy electronic signals.

    struct _Secure_Voice_and_Messaging_Applications //Encrypted tactical communications.
    struct _Satellite_Communications_Terminal_Software //Manages SATCOM links and encryption.

    struct _Weapons_Engagement_Planning_Software //Calculates trajectories, interception points, and firing solutions.
    struct _Battle_Management_Systems //Overall CIC situational awareness and coordination.


    //Bridge
    struct _Electronic_Chart_Display_and_Information_System //Digital nautical charts, route planning, and collision avoidance.
    struct _Integrated_Bridge_System_Software//Combines autopilot, steering, navigation sensors, and engine control.
    struct _Radar_Navigation_Software//Monitors nearby vessels, hazards, and coastal features.
    struct _Automatic_Identification_System //Tracks other ships to prevent collisions.

    struct _Autopilot_Heading_Control_Software//Follows courses, adjusts rudder and speed automatically.
    struct _Propulsion_Monitoring_Software//Monitors engines, fuel, and auxiliary systems (sometimes integrated with IPMS).

    struct _Internal_Ship_Communication_Terminals//Email, intercom, and messaging systems.
    struct _External_Communication_Links//VHF/UHF radios and SATCOM terminal interfaces.

    struct _Weather_and_Tide_Display_Software//Real-time meteorological and oceanographic data.
    struct _Collision_Avoidance_Algorithms//Integrates radar, ECDIS, and AIS data for alerts.
};

struct _Ship_Blueprints
{
    struct _Ship_Electrical_Grid Ship_Electrical_Grid;

    struct _Ship_Sensor_Computer_Weapons_Grid Ship_Sensor_Computer_Weapons_Grid;

    struct _Ship_Water_Distribution_Network Ship_Water_Distribution_Network;
    struct _Ship_Life_Support_Distribution_Network Ship_Life_Support_Distribution_Network;
    struct _Ship_Garbage_Chute_Distribution_Network Ship_Garbage_Chute_Distribution_Network;

    struct _Ship_Shuttle_Latching_System Ship_Shuttle_Latching_System;
    struct _Ship_To_Ship_Docking_System Ship_To_Ship_Docking_System;
    struct _Ship_Door_And_Entry_Point_System Ship_Door_And_Entry_Point_System;

    struct _Ship_Elevator_System Ship_Elevator_System;
    struct _Ship_Room_System Ship_Room_System;
};







#endif // SHIP_H_INCLUDED
