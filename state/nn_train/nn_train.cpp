#include "nn_train.h"


void Initialize_Fake_Submarine(struct _Submarine_Object* Submarine)
{
    Submarine->Models[0] = Create_SE3_Model();
        Load_SE3_Model(Submarine->Models[0], "resources/data/submarines/car2.se3");
        BMPtoSEI("raydrone.bmp","raydrone.sei");
        Load_SE3_Texture(Submarine->Models[0], "raydrone.sei");

Submarine->Current_Orders_Issued = 0;
Submarine->Current_Orders_Received = 0;
Submarine->Current_Leader_Submarine_ID = -1; //-1 means no leader

Submarine->Intended_Heading[0] = 0;
Submarine->Intended_Heading[1] = 0;
Submarine->Intended_Heading[2] = 0;

Submarine->Intended_Throttle = 5;

        Submarine->plane_x = rand() % 4000;
        Submarine->plane_y = rand() % 4000;
        Submarine->plane_z = rand() % 4000;
        Submarine->lastForwardDir.x = 0;
        Submarine->lastForwardDir.y = 0;
        Submarine->lastForwardDir.z = 0;
        Submarine->plane_yaw = 0;
        Submarine->plane_pitch = 0;
        Submarine->plane_roll = 0;
        Submarine->Single_Pilot = 0;

        Submarine->On_The_Ground = false;


    Submarine->Start_Frame = 0;
    Submarine->Current_Frame = 0;
    Submarine->End_Frame = 0;
    Submarine->Current_Model = 0;

    Submarine->Current_State = 0;

    Submarine->Forward_Vector.x = 0;
    Submarine->Forward_Vector.y = -1;
    Submarine->Forward_Vector.z = 0;


    Submarine->Right_Vector.x = 1;
    Submarine->Right_Vector.y = 0;
    Submarine->Right_Vector.z = 0;


    Submarine->Up_Vector.x = 0;
    Submarine->Up_Vector.y = 0;
    Submarine->Up_Vector.z = 1;



    Submarine->Ray_Sensor_Vector.x = 0;
    Submarine->Ray_Sensor_Vector.y = 0;
    Submarine->Ray_Sensor_Vector.z = 0;


        Submarine->Throttle = 1;

    Submarine->Floor_Collision = false;
    Submarine->Wall_Collision = false;
    Submarine->Ceiling_Collision = false;
    Submarine->Object_Collision = false;

    Submarine->On_Top_Of_An_AABB_Collision = false;
    Submarine->Underneath_An_AABB_Collision = false;
    Submarine->Colliding_With_An_AABB_Collision = false;

    Submarine->On_Top_Of_Mesh_Collision = false;
    Submarine->Underneath_Mesh_Collision = false;
    Submarine->Colliding_With_Mesh_Collision = false;

    Submarine->Able_To_Move_Away = true;

    Submarine->Able_To_Move_Off_Platform = true;


    Submarine->Feet_Collision_Geometry.min.x = -4;
    Submarine->Feet_Collision_Geometry.min.y = -4;
    Submarine->Feet_Collision_Geometry.min.z = 0;

    Submarine->Feet_Collision_Geometry.max.x = 4;
    Submarine->Feet_Collision_Geometry.max.y = 4;
    Submarine->Feet_Collision_Geometry.max.z = 20.5;


    Submarine->Head_Collision_Geometry.min.x = -4;
    Submarine->Head_Collision_Geometry.min.y = -4;
    Submarine->Head_Collision_Geometry.min.z = 20.5;

    Submarine->Head_Collision_Geometry.max.x = 4;
    Submarine->Head_Collision_Geometry.max.y = 4;
    Submarine->Head_Collision_Geometry.max.z = 40.5;


    Submarine->Do_Not_Alter_Velocity = false;

    Submarine->displacement.x = 0;
    Submarine->displacement.y = 0;
    Submarine->displacement.z = 0;




      Submarine->SPEED_STEP = 0.005f;
    Submarine->ANGLE_STEP = 1.0f;
    Submarine->TIMER_MS = 16; // ~60 FPS update rate

// --- Global State Variables ---
// Position (World Coordinates)

    Submarine->plane_roll = 0.0f;  // Rotation around Y-axis (Forward/Backward)

// Speed (Magnitude of movement)
    Submarine->plane_speed = 0.5;
}

struct _Neural_Network_Training_State* Create_Neural_Network_Training_State()
{
    struct _Neural_Network_Training_State* NNTS = (struct _Neural_Network_Training_State*)calloc(1,sizeof(struct _Neural_Network_Training_State));
    return NNTS;
};


void Initialize_Neural_Network_Training_State(struct _Engine* Engine)
{
    srand(time(NULL));

    Engine->Neural_Network_Training_State->Laser_Model = Create_SE3_Model();
    Load_SE3_Model(Engine->Neural_Network_Training_State->Laser_Model, "laser.se3");
    Load_SE3_Texture(Engine->Neural_Network_Training_State->Laser_Model, "laser.sei");

    Engine->Neural_Network_Training_State->Number_Of_Laser_Blasts = 0;
    Engine->Neural_Network_Training_State->Current_Laser_Blast = 0;

    Engine->Neural_Network_Training_State->zoom = 0;

    Engine->Neural_Network_Training_State->Number_Of_Flights = 1;
    Engine->Neural_Network_Training_State->Flights[0] = (struct _Submarine_Flight*)calloc(1,sizeof(struct _Submarine_Flight));

    for(int j = 0; j < 16; j++)
    {
    Engine->Neural_Network_Training_State->Fake_Submarine[j] = Create_Fake_Submarine();
    Initialize_Fake_Submarine(Engine->Neural_Network_Training_State->Fake_Submarine[j]);
    }

    Assign_Submarine_Leader(Engine, 1, 0);
    Assign_Submarine_Leader(Engine, 2, 0);
    Assign_Submarine_Leader(Engine, 3, 0);

    Assign_Submarine_Leader(Engine, 5, 4);
    Assign_Submarine_Leader(Engine, 6, 4);
    Assign_Submarine_Leader(Engine, 7, 4);

    Assign_Submarine_Leader(Engine, 9, 8);
    Assign_Submarine_Leader(Engine, 10, 8);
    Assign_Submarine_Leader(Engine, 11, 8);

    Engine->Neural_Network_Training_State->Training = true;
    Engine->Neural_Network_Training_State->Manual_Control= true;

    Engine->Neural_Network_Training_State->Current_Timestamp = 0;
    Engine->Neural_Network_Training_State->Maximum_Timestamp = 9;


    //srand((unsigned int)time(NULL));

    // Structural Hyperparameters



    Engine->Neural_Network_Training_State->SRS[0] = 0.0001;
    Engine->Neural_Network_Training_State->SRS[1] = METER_CONVERSION *0.1;
    Engine->Neural_Network_Training_State->SRS[2] = METER_CONVERSION *0.2;
    Engine->Neural_Network_Training_State->SRS[3] = METER_CONVERSION *0.3;
    Engine->Neural_Network_Training_State->SRS[4] = METER_CONVERSION *0.4;
    Engine->Neural_Network_Training_State->SRS[5] = METER_CONVERSION *0.5;
    Engine->Neural_Network_Training_State->SRS[6] = METER_CONVERSION *0.6;
    Engine->Neural_Network_Training_State->SRS[7] = METER_CONVERSION *0.7;
    Engine->Neural_Network_Training_State->SRS[8] = METER_CONVERSION *0.8;
    Engine->Neural_Network_Training_State->SRS[9] = METER_CONVERSION *0.9;
    Engine->Neural_Network_Training_State->SRS[10] = METER_CONVERSION *1;

    Engine->Neural_Network_Training_State->MRS[0] = 1;
    Engine->Neural_Network_Training_State->MRS[1] = METER_CONVERSION *1;
    Engine->Neural_Network_Training_State->MRS[2] = METER_CONVERSION *2;
    Engine->Neural_Network_Training_State->MRS[3] = METER_CONVERSION *3;
    Engine->Neural_Network_Training_State->MRS[4] = METER_CONVERSION *4;
    Engine->Neural_Network_Training_State->MRS[5] = METER_CONVERSION *5;
    Engine->Neural_Network_Training_State->MRS[6] = METER_CONVERSION *6;
    Engine->Neural_Network_Training_State->MRS[7] = METER_CONVERSION *7;
    Engine->Neural_Network_Training_State->MRS[8] = METER_CONVERSION *8;
    Engine->Neural_Network_Training_State->MRS[9] = METER_CONVERSION *9;
    Engine->Neural_Network_Training_State->MRS[10] = METER_CONVERSION * 10;

    Engine->Neural_Network_Training_State->LRS[0] = 100;
    Engine->Neural_Network_Training_State->LRS[1] = METER_CONVERSION *100;
    Engine->Neural_Network_Training_State->LRS[2] = METER_CONVERSION *200;
    Engine->Neural_Network_Training_State->LRS[3] = METER_CONVERSION *300;
    Engine->Neural_Network_Training_State->LRS[4] = METER_CONVERSION *400;
    Engine->Neural_Network_Training_State->LRS[5] = METER_CONVERSION *500;
    Engine->Neural_Network_Training_State->LRS[6] = METER_CONVERSION *600;
    Engine->Neural_Network_Training_State->LRS[7] = METER_CONVERSION *700;
    Engine->Neural_Network_Training_State->LRS[8] = METER_CONVERSION *800;
    Engine->Neural_Network_Training_State->LRS[9] = METER_CONVERSION *900;
    Engine->Neural_Network_Training_State->LRS[10] = METER_CONVERSION * 1000;

    memset(Engine->Neural_Network_Training_State->My_Long_Range_Sensors,0,6);
    memset(Engine->Neural_Network_Training_State->My_Medium_Range_Sensors,0,6);
    memset(Engine->Neural_Network_Training_State->My_Short_Range_Sensors,0,6);

    memset(Engine->Neural_Network_Training_State->My_Current_Output_Policy,0,4);

    //srand(time(NULL));

    // Define Network: 3 Inputs, 2 Hidden NNLayers (8 neurons each), 2 Outputs
    // Inputs: [Pitch angle, Roll angle, Altitude error]
    // Outputs: [Elevator command, Aileron command]




    Engine->Neural_Network_Training_State->Snapshot_Timer = Create_Timer();
    Set_Timer(Engine->Neural_Network_Training_State->Snapshot_Timer);

    Engine->Neural_Network_Training_State->Current_IO_Example = 0;
    Engine->Neural_Network_Training_State->Current_IO_Capture = 0;
    Engine->Neural_Network_Training_State->Snapshot_Time = 0;

    Engine->Neural_Network_Training_State->Mesh = Create_Preloaded_Collision_Mesh("resources/sphere.se3");
    Engine->Neural_Network_Training_State->Manual_Control = true;
    Engine->Neural_Network_Training_State->Mesh->z = 0;

    Engine->Neural_Network_Training_State->Skybox = Create_SE3_Model();
    Load_SE3_Model(Engine->Neural_Network_Training_State->Skybox,"resources/models/model_skybox.se3");
    Load_SE3_Texture(Engine->Neural_Network_Training_State->Skybox,"resources/models/title_sonichu_skybox_light.sei");

    Engine->Neural_Network_Training_State->Test_Sub = Create_SE3_Model();
    Load_SE3_Model(Engine->Neural_Network_Training_State->Test_Sub,"resources/sphere.se3");
    Load_SE3_Texture(Engine->Neural_Network_Training_State->Test_Sub,"car.sei");

    glColor3f(1,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH); // Enable smooth shading (Gouraud shading)
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40,40,40,0,0,20,0,0,1);

    // Somewhere in the initialization part of your program…
    // Enable depth testing and lighting
    // --- Enable Depth Test and Smooth Shading ---

glEnable(GL_DEPTH_TEST);

// --- Lighting Setup ---
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Light source position (W=0 means directional light, W=1 means positional light)
    GLfloat light_position[] = {5.0f, 5.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Light colors (bright white light)
    GLfloat light_ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat light_diffuse[] = {0.25f, 0.25f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // --- Configuration Constants ---
const float FOG_DENSITY = 0.000003f; // Controls how thick the fog is (0.01 is light, 0.1 is thick) Radius is 32km
// Dark Blue Fog Color (R, G, B,= A)
const float FOG_COLOR[4] = {0.0f, 0.0f, 0.1f, 1.0f};


    // --- Fog Implementation ---
    glEnable(GL_FOG);

    // 1. Set the Fog Color (The same color as the clear color for seamless transition)
    glFogfv(GL_FOG_COLOR, FOG_COLOR);

    // 2. Set the Fog Mode (How the fog density changes with distance)
    // GL_EXP2 is often used for a realistic, atmospheric effect
    glFogi(GL_FOG_START, 0);
    glFogi(GL_FOG_END, METER_CONVERSION*32000); //THe effective range will be 16 km.
    glFogi(GL_FOG_MODE, GL_EXP2);

    // 3. Set the Fog Density
    glFogf(GL_FOG_DENSITY, FOG_DENSITY);

    // 4. (Optional) Make sure the rendering hint is set for quality
    glHint(GL_FOG_HINT, GL_NICEST);


     glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
            gluPerspective(60, 1.7777,1.0f, METER_CONVERSION * 75000);
        glCullFace(GL_BACK); // Temporarily flip culling


    Engine->Neural_Network_Training_State->Submarine = Create_Submarine_Object();
    Initialize_Submarine_Object(Engine,0);









};


void Process_NN_Plane(struct _Engine* Engine)
{
    Engine->Neural_Network_Training_State->Submarine->plane_x += Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x * Engine->Neural_Network_Training_State->Submarine->Throttle;
    Engine->Neural_Network_Training_State->Submarine->plane_y += Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y * Engine->Neural_Network_Training_State->Submarine->Throttle;
    Engine->Neural_Network_Training_State->Submarine->plane_z += Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z * Engine->Neural_Network_Training_State->Submarine->Throttle;
}


void Render_Neural_Network_Training_State(struct _Engine* Engine)
{
    glPushMatrix();
    glTranslatef(0,0,0);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Skybox,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,Engine->Neural_Network_Training_State->Mesh->z);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Test_Sub,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(Engine->Neural_Network_Training_State->Submarine->plane_x, Engine->Neural_Network_Training_State->Submarine->plane_y, Engine->Neural_Network_Training_State->Submarine->plane_z);
    // Apply the orientation matrix
    float mat[16] = {
        (float)Engine->Neural_Network_Training_State->Submarine->Right_Vector.x, (float)Engine->Neural_Network_Training_State->Submarine->Right_Vector.y, (float)Engine->Neural_Network_Training_State->Submarine->Right_Vector.z, 0,
        (float)Engine->Neural_Network_Training_State->Submarine->Up_Vector.x,    (float)Engine->Neural_Network_Training_State->Submarine->Up_Vector.y,    (float)Engine->Neural_Network_Training_State->Submarine->Up_Vector.z,    0,
        -(float)Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x,  -(float)Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y,  -(float)Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Submarine->Models[0],0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
};

void Handle_Neural_Network_Training_State_Inputs(struct _Engine* Engine, struct _Keypad Keypad)
{
    if(Engine->Neural_Network_Training_State->Manual_Control)
    {
    if(!Keypad.Keyboard_A || Keypad.Keyboard_B)
    {
        Engine->Neural_Network_Training_State->Delta_Throttle = 0;
    }

    if(!Keypad.Keyboard_A || Keypad.Keyboard_B)
    {
        //Engine->Neural_Network_Training_State->Delta_Roll = 0;
    }

    //printf("Throttle %f meters per frame\n", Engine->Neural_Network_Training_State->Submarine->Throttle * METER_CONVERSION);


    if(!Keypad.Keyboard_Up || Keypad.Keyboard_Down)
    {
        Engine->Neural_Network_Training_State->Delta_Pitch = 0;
    }


    if(!Keypad.Keyboard_Left || Keypad.Keyboard_Right)
    {
        Engine->Neural_Network_Training_State->Delta_Yaw = 0;
    }

        if(Keypad.Keyboard_A && Engine->Neural_Network_Training_State->Submarine->Throttle + 1 <= MAX_THROTTLE)
        {
            Engine->Neural_Network_Training_State->Submarine->Throttle += 1;
            Engine->Neural_Network_Training_State->Delta_Throttle = 1;
        }

        if(Keypad.Keyboard_B && Engine->Neural_Network_Training_State->Submarine->Throttle -1 >= 0)
        {
            Engine->Neural_Network_Training_State->Submarine->Throttle -= 1;
            Engine->Neural_Network_Training_State->Delta_Throttle = -1;
        // Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_C)
        {
            Engine->Neural_Network_Training_State->Submarine->Throttle = 0;
        //  Process_NN_Plane(Engine);
        }


        if(Keypad.Keyboard_Up)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Pitch = 1;
        // Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_Down)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Pitch = -1;
        //Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_Left)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Yaw = 1;
       //Process_NN_Plane(Engine);
        }

        if(Keypad.Keyboard_Right)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
            Engine->Neural_Network_Training_State->Delta_Yaw = -1;
       // Process_NN_Plane(Engine);
        }
    }

    else if(!Engine->Neural_Network_Training_State->Manual_Control)
    {
        double Intended_Pitch = round(Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[0] * 360);
        double Intended_Roll = round(Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[1] * 360);
        double Intended_Yaw = round(Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[2] * 360);
        double Intended_Throttle = Engine->Neural_Network_Training_State->My_Neuropilot_Output_Policy[3] * MAX_THROTTLE;

        if(Intended_Pitch < 0)
        {
            Intended_Pitch += 360;
        }

        if(Intended_Roll < 0)
        {
            Intended_Roll += 360;
        }

        if(Intended_Yaw < 0)
        {
            Intended_Yaw += 360;
        }

        if(Intended_Throttle < 0)
        {
            Intended_Throttle = 0;
        }



        if(Intended_Pitch > 360)
        {
            Intended_Pitch -= 360;
        }

        if(Intended_Roll > 360)
        {
            Intended_Roll -= 360;
        }

        if(Intended_Yaw > 360)
        {
            Intended_Yaw -= 360;
        }

        if(Intended_Throttle > MAX_THROTTLE)
        {
            Intended_Throttle = MAX_THROTTLE;
        }


        //printf("Intended PRY: %f %f %f\n", Intended_Pitch, Intended_Roll, Intended_Yaw);

        if(Engine->Neural_Network_Training_State->Submarine->plane_pitch > Intended_Pitch)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
        }

        else if(Engine->Neural_Network_Training_State->Submarine->plane_pitch < Intended_Pitch)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_pitch += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_pitch > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_pitch -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Forward_Vector,  &Engine->Neural_Network_Training_State->Submarine->Up_Vector, Engine->Neural_Network_Training_State->Submarine->Right_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);

        }

        if(Engine->Neural_Network_Training_State->Submarine->plane_yaw < Intended_Yaw)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw += Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw > 359)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw -= 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
        }

        else if(Engine->Neural_Network_Training_State->Submarine->plane_yaw > Intended_Yaw)
        {
            Engine->Neural_Network_Training_State->Submarine->plane_yaw -= Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP;
            if(Engine->Neural_Network_Training_State->Submarine->plane_yaw < 0)
            {
                Engine->Neural_Network_Training_State->Submarine->plane_yaw += 360;
            }
            rotate_axes(&Engine->Neural_Network_Training_State->Submarine->Up_Vector,  &Engine->Neural_Network_Training_State->Submarine->Right_Vector, Engine->Neural_Network_Training_State->Submarine->Forward_Vector,   -Engine->Neural_Network_Training_State->Submarine->ANGLE_STEP);
        }

        if(Engine->Neural_Network_Training_State->Submarine->Throttle < Intended_Throttle)
        {
            if(Engine->Neural_Network_Training_State->Submarine->Throttle + 1 <= MAX_THROTTLE)
            {
            Engine->Neural_Network_Training_State->Submarine->Throttle += 1;
            }
        }

        if(Engine->Neural_Network_Training_State->Submarine->Throttle > Intended_Throttle)
        {
            if(Engine->Neural_Network_Training_State->Submarine->Throttle - 1 >= 0)
            {
            Engine->Neural_Network_Training_State->Submarine->Throttle -= 1;
            }
        }
        //double Intended_Throttle =


        //printf("Neuropilot pitch: %f roll: %f yaw: %f\n", Engine->Neural_Network_Training_State->Submarine->plane_pitch,Engine->Neural_Network_Training_State->Submarine->plane_roll,Engine->Neural_Network_Training_State->Submarine->plane_yaw,Engine->Neural_Network_Training_State->Submarine->Throttle);
        //printf("Neuropilot throttle: %f\n", Engine->Neural_Network_Training_State->Submarine->Throttle);
    }

    if(Keypad.Keyboard_D)
    {
        //Engine->Neural_Network_Training_State->Submarine->View_Mode = TPS_MODE;

        Engine->Neural_Network_Training_State->Laser_Blasts[Engine->Neural_Network_Training_State->Current_Laser_Blast] = Create_Laser_Blast();
        Initialize_Laser_Blast(Engine, Engine->Neural_Network_Training_State->Laser_Blasts[Engine->Neural_Network_Training_State->Current_Laser_Blast],Engine->Neural_Network_Training_State->Submarine->plane_x,Engine->Neural_Network_Training_State->Submarine->plane_y,Engine->Neural_Network_Training_State->Submarine->plane_z,Engine->Neural_Network_Training_State->Submarine->Right_Vector,Engine->Neural_Network_Training_State->Submarine->Up_Vector,Engine->Neural_Network_Training_State->Submarine->Forward_Vector,Engine->Neural_Network_Training_State->Submarine->Throttle,Get_Ticks(Engine->Neural_Network_Training_State->Snapshot_Timer));

        printf("Created laser\n");

        Engine->Neural_Network_Training_State->Current_Laser_Blast += 1;
        Engine->Neural_Network_Training_State->Number_Of_Laser_Blasts += 1;
        printf("Lasers: %d\n",Engine->Neural_Network_Training_State->Current_Laser_Blast );
    }

    if(Keypad.Keyboard_E)
    {
       // Engine->Neural_Network_Training_State->Submarine->View_Mode = REAR_VIEW_MODE;
    }


    if(Keypad.Keyboard_F)
    {
        Engine->Neural_Network_Training_State->Training= false;
        //printf("Training is off.\n");
        Engine->Neural_Network_Training_State->Submarine->plane_x = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_y = 0;
        Engine->Neural_Network_Training_State->Submarine->plane_z = 0;
    }

    if(Keypad.Keyboard_G)
    {
        Engine->Neural_Network_Training_State->Training = true;
        //printf("Training is on.\n");
    }

    if(Engine->Neural_Network_Training_State->Manual_Control)
    {
        //printf("Manual Control is ON!\n");
    }

    if(Engine->Neural_Network_Training_State->Training)
    {
       //printf("Training is ON!\n");
    }


    if(!Engine->Neural_Network_Training_State->Manual_Control)
    {
        //printf("Manual Control isnt ON!\n");
    }

    if(!Engine->Neural_Network_Training_State->Training)
    {
       //printf("Training isnt ON!\n");
    }

    if(Keypad.Keyboard_Start)
    {
        //printf("Third-Person View!\n");
        Engine->Neural_Network_Training_State->Third_Person = true;
    }

};


struct _Submarine_Object* Create_Fake_Submarine()
{
    struct _Submarine_Object* SO = (struct _Submarine_Object*)calloc(1,sizeof(struct _Submarine_Object));
    return SO;
}


void calculate_yaw_pitch(Vec3 direction, double *yaw, double *pitch) {
    // 1. Yaw (Rotation around the Z-axis, for X-Y horizontal plane)
    // atan2(Y, X) gives the angle from the positive X-axis in the X-Y plane.
    // Since +Y is "forward" and +X is "right", we use Y/X.
    // The range is [-PI, PI]. We convert to degrees.
    *yaw = atan2f(direction.y, direction.x) * (180.0f / 3.14);

    // 2. Pitch (Rotation around the X-axis, for vertical alignment)
    // Pitch is based on the vertical component (Z) and the length of the horizontal projection.
    float horizontal_distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
    *pitch = atan2f(direction.z, horizontal_distance) * (180.0f / 3.14);
}



// Convert Quaternion to a 4x4 Column-Major Matrix for OpenGL
void quat_to_matrix(Quat q, float* m) {
    float xx = q.x * q.x; float yy = q.y * q.y; float zz = q.z * q.z;
    float xy = q.x * q.y; float xz = q.x * q.z; float yz = q.y * q.z;
    float wx = q.w * q.x; float wy = q.w * q.y; float wz = q.w * q.z;

    m[0] = 1.0f - 2.0f * (yy + zz); m[4] = 2.0f * (xy - wz);        m[8]  = 2.0f * (xz + wy);        m[12] = 0.0f;
    m[1] = 2.0f * (xy + wz);        m[5] = 1.0f - 2.0f * (xx + zz); m[9]  = 2.0f * (yz - wx);        m[13] = 0.0f;
    m[2] = 2.0f * (xz - wy);        m[6] = 2.0f * (yz + wx);        m[10] = 1.0f - 2.0f * (xx + yy); m[11] = 0.0f;
    m[3] = 0.0f;                    m[7] = 0.0f;                    m[11] = 0.0f;                    m[15] = 1.0f;
}


// Create a quaternion from an axis and an angle (in radians)
Quat quat_from_axis_angle(Vec3 axis, float angle) {
    float half_angle = angle * 0.5f;
    float sin_half = sinf(half_angle);
    axis = vec3_normalize(axis);
    return (Quat){ cosf(half_angle), axis.x * sin_half, axis.y * sin_half, axis.z * sin_half };
}


// Normalize a quaternion to avoid numerical drift
Quat quat_normalize(Quat q) {
    float len = sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    return (Quat){ q.w/len, q.x/len, q.y/len, q.z/len };
}

// Multiply two quaternions (combines rotations)
Quat quat_mult(Quat q1, Quat q2) {
    return (Quat){
        q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
        q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
        q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
        q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w
    };
}


float normalizeAngle(float angle) {
    while (angle > 180.0f) angle -= 360.0f;
    while (angle < -180.0f) angle += 360.0f;
    return angle;
}

void Process_Fake_Submarine(struct _Engine* Engine, struct _Submarine_Object* Submarine)
{
    //Submarine->plane_x += Submarine->Forward_Vector.x * Submarine->Throttle;
   // Submarine->plane_y += Submarine->Forward_Vector.y * Submarine->Throttle;
   // Submarine->plane_z += Submarine->Forward_Vector.z * Submarine->Throttle;

   int Random = rand() % 1000;

   if(Submarine->Current_Leader_Submarine_ID != -1)
   {

   Submarine->Intended_Position[0] = Engine->Neural_Network_Training_State->Fake_Submarine[Submarine->Current_Leader_Submarine_ID]->plane_x + Random;
   Submarine->Intended_Position[1] = Engine->Neural_Network_Training_State->Fake_Submarine[Submarine->Current_Leader_Submarine_ID]->plane_y + Random;
   Submarine->Intended_Position[2] = Engine->Neural_Network_Training_State->Fake_Submarine[Submarine->Current_Leader_Submarine_ID]->plane_z + Random;
    }

    else if(Submarine->Current_Leader_Submarine_ID == -1)
   {

   Submarine->Intended_Position[0] = Engine->Neural_Network_Training_State->Submarine->plane_x;
   Submarine->Intended_Position[1] = Engine->Neural_Network_Training_State->Submarine->plane_y;
   Submarine->Intended_Position[2] = Engine->Neural_Network_Training_State->Submarine->plane_z;
    }

    // 1. Calculate vector from airplane to target
    Vec3 dirToTarget = {
        Submarine->Intended_Position[0] - Submarine->plane_x,
        Submarine->Intended_Position[1] - Submarine->plane_y,
        Submarine->Intended_Position[2] - Submarine->plane_z
    };

    float dist = sqrtf(dirToTarget.x * dirToTarget.x + dirToTarget.y * dirToTarget.y + dirToTarget.z * dirToTarget.z);

    double distance = Get_Distance(Submarine->Intended_Position[0],Submarine->Intended_Position[1],Submarine->Intended_Position[2],Submarine->plane_x,Submarine->plane_y,Submarine->plane_z);

    if(distance < METER_CONVERSION * 5) //Stop before 10 meters.
    {
        Submarine->Intended_Throttle = 0;
        //printf("Too close!\n");
    }

    else if(distance > METER_CONVERSION * 5) //Stop before 10 meters.
    {
        Submarine->Intended_Throttle = 9;
        //printf("Too far!\n");
    }

    for(int j = 0; j < 4; j++) //Check if i'm to close to other subs
    {
            distance = Get_Distance(Engine->Neural_Network_Training_State->Fake_Submarine[j]->plane_x,Engine->Neural_Network_Training_State->Fake_Submarine[j]->plane_y,Engine->Neural_Network_Training_State->Fake_Submarine[j]->plane_z, Submarine->plane_x,Submarine->plane_y,Submarine->plane_z);

    if(distance < METER_CONVERSION * 50) //Stop before 10 meters.
    {
        Submarine->Intended_Throttle = 0;
        //printf("Too close!\n");
    }

    else if(distance > METER_CONVERSION * 50) //Stop before 10 meters.
    {
        Submarine->Intended_Throttle = 9;
        //printf("Too far!\n");
    }

    }
    //if (dist < 10.f) return; // Arrived at target

    if(Submarine->Throttle > Submarine->Intended_Throttle)
    {
        if(Submarine->Throttle - 1 >= 0)
        {
            Submarine->Throttle -= 1;
        }
    }

    if(Submarine->Throttle < Submarine->Intended_Throttle)
    {
        if(Submarine->Throttle + 1 < MAX_THROTTLE)
        {
            Submarine->Throttle += 1;
        }
    }


    dirToTarget.x /= dist;
    dirToTarget.y /= dist;
    dirToTarget.z /= dist;

    // 2. Compute the Airplane's CURRENT Local Direction Vectors
float pRad = Submarine->pitch * M_PI / 180.0f;
float yRad = Submarine->yaw * M_PI / 180.0f;
float bRad = Submarine->bank * M_PI / 180.0f;

float sinY = sinf(yRad), cosY = cosf(yRad);
float sinP = sinf(pRad), cosP = cosf(pRad);
float sinB = sinf(bRad), cosB = cosf(bRad);

// Precise Local Forward Vector (Where the nose points)
Vec3 localForward = {
    -cosP * sinY,
    -cosP * cosY,
     sinP
};

// Precise Local Right Vector (Where the right wing points)
Vec3 localRight = {
    -cosY * cosB - sinY * sinP * sinB,
     sinY * cosB - cosY * sinP * sinB,
    -cosP * sinB
};

// Precise Local Up Vector (Where the roof points)
Vec3 localUp = {
    -cosY * sinB + sinY * sinP * cosB,
     sinY * sinB + cosY * sinP * cosB,
     cosP * cosB
};

    // 3. Project the target vector onto the airplane's local axes
float targetInFront = dirToTarget.x * localForward.x + dirToTarget.y * localForward.y + dirToTarget.z * localForward.z;
float targetToRight = dirToTarget.x * localRight.x   + dirToTarget.y * localRight.y   + dirToTarget.z * localRight.z;
float targetAbove   = dirToTarget.x * localUp.x      + dirToTarget.y * localUp.y      + dirToTarget.z * localUp.z;

// 4. Steering Logic
// --- New Flight Tuning Constants ---
const float MAX_PITCH_SPEED = 8.0f;  // Much higher maximum pitch rate
const float MAX_TURN_SPEED = 0.15f;  // Much higher yaw-by-bank rate
const float PITCH_SENSITIVITY = 5.0f; // How aggressively it reacts to pitch error

// 4. Advanced High-Speed Steering Logic

// PITCH CONTROL (Proportional)
// Instead of adding a fixed PITCH_SPEED, add speed proportional to targetAbove
float desiredPitchSpeed = targetAbove * PITCH_SENSITIVITY;

// Clamp the pitch speed so it doesn't rotate infinitely fast
if (desiredPitchSpeed > MAX_PITCH_SPEED)  desiredPitchSpeed = MAX_PITCH_SPEED;
if (desiredPitchSpeed < -MAX_PITCH_SPEED) desiredPitchSpeed = -MAX_PITCH_SPEED;

Submarine->pitch += desiredPitchSpeed;

// Tighten pitch bounds slightly below 90 to protect the matrix math
if (Submarine->pitch > 80.0f) Submarine->pitch = 80.0f;
if (Submarine->pitch < -80.0f) Submarine->pitch = -80.0f;


// BANK & YAW CONTROL (Proportional)
// The bank angle matches how far left/right the target is
float desiredBank = -targetToRight * MAX_BANK;
if (targetInFront < 0.0f) {
    // Target is behind; snap to maximum bank to pivot around backward
    desiredBank = (targetToRight > 0.0f) ? -MAX_BANK : MAX_BANK;
}

// If turn speeds are high, we must smooth out the roll transition
// to prevent the wings from shaking violently (twitching)
Submarine->bank += (desiredBank - Submarine->bank) * 0.15f;

// Yaw scales dynamically based on your high turn speed constant
Submarine->yaw += Submarine->bank * -MAX_TURN_SPEED;
Submarine->yaw = normalizeAngle(Submarine->yaw);

// 5. Move Forward along the current local forward vector
Submarine->plane_x += Submarine->Throttle * localForward.x;
Submarine->plane_y += Submarine->Throttle * localForward.y;
Submarine->plane_z += Submarine->Throttle * localForward.z;

    // Print telemetry to console
   //printf("Pos: (%.1f, %.1f, %.1f) | Yaw: %.1f | Pitch: %.1f | Bank: %.1f\n",
           //Submarine->plane_x, Submarine->plane_y, Submarine->plane_z, Submarine->yaw, Submarine->pitch, Submarine->bank);


           glPushMatrix();
    glTranslatef(Submarine->plane_x, Submarine->plane_y, Submarine->plane_z);
    // Apply the orientation matrix
    float mat[16] = {
        (float)localRight.x, (float)localRight.y, (float)localRight.z, 0,
        (float)localUp.x,    (float)localUp.y,    (float)localUp.z,    0,
        -(float)localForward.x,  -(float)localForward.y,  -(float)localForward.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);
    Render_SE3_Model(Submarine->Models[0],0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();




       double x = Get_Ticks(Engine->Neural_Network_Training_State->Snapshot_Timer);
   if(x > Engine->Neural_Network_Training_State->Last_Fire_Time + 0.5 + Random)
   {

   Engine->Neural_Network_Training_State->Laser_Blasts[Engine->Neural_Network_Training_State->Current_Laser_Blast] = Create_Laser_Blast();
        Initialize_Laser_Blast(Engine, Engine->Neural_Network_Training_State->Laser_Blasts[Engine->Neural_Network_Training_State->Current_Laser_Blast],Submarine->plane_x,Submarine->plane_y,Submarine->plane_z,localRight,localUp,localForward,Submarine->Throttle,Get_Ticks(Engine->Neural_Network_Training_State->Snapshot_Timer));

        printf("Created laser\n");

        Engine->Neural_Network_Training_State->Current_Laser_Blast += 1;
        Engine->Neural_Network_Training_State->Number_Of_Laser_Blasts += 1;
        printf("Lasers: %d\n",Engine->Neural_Network_Training_State->Current_Laser_Blast );

        Engine->Neural_Network_Training_State->Last_Fire_Time = x;
    }
}

void Render_Fake_Submarine(struct _Engine* Engine, struct _Submarine_Object* Submarine)
{


    glPushMatrix();
   glTranslatef(Submarine->plane_x, Submarine->plane_y, Submarine->plane_z);

   // 1. Yaw: Rotate around the Up axis (+Z)
glRotatef(Submarine->yaw, 0.0f, 0.0f, 1.0f);

// 2. Pitch: Rotate around the Wing axis (-X)
// If a positive pitch angle means "nose up" (moving toward +Z),
// rotating around the negative X-axis achieves this in a right-handed system.
glRotatef(Submarine->pitch, -1.0f, 0.0f, 0.0f);

// 3. Bank: Rotate around the Forward axis (-Y)
// If a positive bank means "roll right" (dipping the left wing up toward +Z),
// rotating around the negative Y-axis aligns with this convention.
glRotatef(Submarine->bank, 0.0f, -1.0f, 0.0f);

   // Render_SE3_Model(Submarine->Models[0],0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
}

void Handle_Fake_Submarine_Inputs(struct _Engine* Engine, struct _Submarine_Object* Submarine)
{

}


void Process_Neural_Network_Training_State(struct _Engine* Engine)
{
       for(int j = 0; j < Engine->Neural_Network_Training_State->Number_Of_Laser_Blasts; j++)
   {
        Process_Laser_Blast(Engine, Engine->Neural_Network_Training_State->Laser_Blasts[j]);
        //printf("Processing laser \n");
   }

       for(int j = 0; j < Engine->Neural_Network_Training_State->Number_Of_Laser_Blasts; j++)
   {
        Render_Laser_Blast(Engine, Engine->Neural_Network_Training_State->Laser_Blasts[j]);
       // printf("Rendering laser \n");
   }

    for(int j = 0; j < 16; j++)
    {
        Process_Fake_Submarine(Engine, Engine->Neural_Network_Training_State->Fake_Submarine[j]);
    }



    Process_NN_Plane(Engine);

    Vec3 cam;
    switch(Engine->Neural_Network_Training_State->Submarine->View_Mode)
    {
        case REAR_VIEW_MODE:
        Engine->Neural_Network_Training_State->zoom = REAR_VIEW_ZOOM;
        break;

        case TPS_MODE:
        Engine->Neural_Network_Training_State->zoom = FRONT_VIEW_ZOOM;
        break;
    }

    cam.x = Engine->Neural_Network_Training_State->Submarine->plane_x - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.x * Engine->Neural_Network_Training_State->zoom + Engine->Neural_Network_Training_State->Submarine->Up_Vector.x * 30.0f;
            cam.y = Engine->Neural_Network_Training_State->Submarine->plane_y - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.y * Engine->Neural_Network_Training_State->zoom + Engine->Neural_Network_Training_State->Submarine->Up_Vector.y * 30.0f;
            cam.z = Engine->Neural_Network_Training_State->Submarine->plane_z - Engine->Neural_Network_Training_State->Submarine->Forward_Vector.z * Engine->Neural_Network_Training_State->zoom + Engine->Neural_Network_Training_State->Submarine->Up_Vector.z * 30.0f;



    //printf("Zoom: %f\n", Engine->Neural_Network_Training_State->zoom);

        glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(!Engine->Neural_Network_Training_State->Third_Person)
    {
    gluLookAt(cam.x, cam.y, cam.z, Engine->Neural_Network_Training_State->Submarine->plane_x, Engine->Neural_Network_Training_State->Submarine->plane_y, Engine->Neural_Network_Training_State->Submarine->plane_z, Engine->Neural_Network_Training_State->Submarine->Up_Vector.x, Engine->Neural_Network_Training_State->Submarine->Up_Vector.y, Engine->Neural_Network_Training_State->Submarine->Up_Vector.z);
    }

    else if(Engine->Neural_Network_Training_State->Third_Person)
    {
    gluLookAt(0,0,0, Engine->Neural_Network_Training_State->Submarine->plane_x, Engine->Neural_Network_Training_State->Submarine->plane_y, Engine->Neural_Network_Training_State->Submarine->plane_z, 0,0,1);
    }

};

void Assign_Submarine_Leader(struct _Engine* Engine, int This_Submarine, int Will_Have_This_Leader)
{
    Engine->Neural_Network_Training_State->Fake_Submarine[This_Submarine]->Current_Leader_Submarine_ID = Will_Have_This_Leader;
}


void Processes_Submarine_Flight(struct _Submarine_Flight* Flight, struct _Engine* Engine)
{
    for(int j = 0; j < Engine->Neural_Network_Training_State->Number_Of_Flights; j++)
    {

    }
}


struct _Laser_Blast* Create_Laser_Blast()
{
    struct _Laser_Blast* Laser_Blast=(struct _Laser_Blast*)calloc(1,sizeof(struct _Laser_Blast));
    return Laser_Blast;
}

void Initialize_Laser_Blast(struct _Engine* Engine,struct _Laser_Blast* Laser_Blast, double x, double y, double z, Vec3 localRight, Vec3 localUp, Vec3 localForward, double Velocity,double Time_Created)
{
    Laser_Blast->x = x;
    Laser_Blast->y = y;
    Laser_Blast->z = z;

    Laser_Blast->localForward.x = localForward.x;
    Laser_Blast->localForward.y = localForward.y;
    Laser_Blast->localForward.z = localForward.z;

    Laser_Blast->localRight.x = localRight.x;
    Laser_Blast->localRight.y = localRight.y;
    Laser_Blast->localRight.z = localRight.z;

    Laser_Blast->localUp.x = localUp.x;
    Laser_Blast->localUp.y = localUp.y;
    Laser_Blast->localUp.z = localUp.z;

    Laser_Blast->Velocity = Velocity + 100;

    Laser_Blast->Time_Created = Time_Created;

    Laser_Blast->Dead = false;
}

void Process_Laser_Blast(struct _Engine* Engine,struct _Laser_Blast* Laser_Blast)
{
    if(!Laser_Blast->Dead)
    {
    Laser_Blast->x += Laser_Blast->Velocity * Laser_Blast->localForward.x;
    Laser_Blast->y += Laser_Blast->Velocity * Laser_Blast->localForward.y;
    Laser_Blast->z += Laser_Blast->Velocity * Laser_Blast->localForward.z;

    double x = Get_Ticks(Engine->Neural_Network_Training_State->Snapshot_Timer);
    if(x > Laser_Blast->Time_Created + 5)
    {
        Laser_Blast->Dead = true;
    }
    }
}

void Render_Laser_Blast(struct _Engine* Engine,struct _Laser_Blast* Laser_Blast)
{
        if(!Laser_Blast->Dead)
    {
           glPushMatrix();
    glTranslatef(Laser_Blast->x,Laser_Blast->y,Laser_Blast->z);
    // Apply the orientation matrix
    float mat[16] = {
        (float)Laser_Blast->localRight.x, (float)Laser_Blast->localRight.y, (float)Laser_Blast->localRight.z, 0,
        (float)Laser_Blast->localUp.x,    (float)Laser_Blast->localUp.y,    (float)Laser_Blast->localUp.z,    0,
        -(float)Laser_Blast->localForward.x,  -(float)Laser_Blast->localForward.y,  -(float)Laser_Blast->localForward.z,  0,
        0,       0,       0,       1
    };
    glMultMatrixf(mat);
    Render_SE3_Model(Engine->Neural_Network_Training_State->Laser_Model,0,0,0,0,0,0,0,false,1,1,1,false);
    glPopMatrix();
    }
}

