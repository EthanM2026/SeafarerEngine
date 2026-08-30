#ifndef FACTION_H_INCLUDED
#define FACTION_H_INCLUDED

//Control whatever you can.
//Cooperate when it's useful.
//Sabotage when you can't fight openly.
//Strike when you can.

#define END_GOAL_SOLE_SURVIVOR 1 //Then your goal is eventual crippling of all other factions.
#define END_GOAL_CLAIM_TERRITORY 2 //Then you need to hold and expand territory while crippling threats to you.
#define END_GOAL_RESEARCH 3 //Then complete the most research as efficiently as possible.
#define END_GOAL_AID_AS_MANY_AS_POSSIBLE 4 //Then you need the largest amount of people helped as extensively as you can.
#define END_GOAL_PROTECT_NATURAL_WORLD 5 //Then you need the largest area of ocean clean.
#define END_GOAL_PROFIT 6 //Then you need to make the most money and eliminate competitors.
#define END_GOAL_FIELD_SUPREMACY 7 //Then you need to train your forces until they are superior.
#define END_GOAL_EXECUTE_PROTOCOL 8
#define END_GOAL_FURTHER_ANARCHISM 9 //Then you need to cripple as many power structures as possible.
#define END_GOAL_SEPARATISM 10 //Then you need to rally supporters and cripple threats to you.
#define END_GOAL_UNIFICATION 11 //Then you need to purge the disloyal and copy your parent faction.
#define END_GOAL_NONE 12


#define FACTION_CLASS_MILITARY_REMNANTS 1
#define FACTION_CLASS_RAIDERS 2
#define FACTION_CLASS_REGIME 3
#define FACTION_CLASS_CULT 4
#define FACTION_CLASS_SCIENTISTS 5
#define FACTION_CLASS_NOMADS 6
#define FACTION_CLASS_PREPPERS 7
#define FACTION_CLASS_MERCENARIES 8
#define FACTION_CLASS_TRADERS 9
#define FACTION_CLASS_ANARCHISTS 10
#define FACTION_CLASS_CRIMINAL_SYNDICATES 11
#define FACTION_CLASS_ISOLATIONISTS 12
#define FACTION_CLASS_HUMANITARIAN_GROUPS 13
#define FACTION_CLASS_SCAVENGERS 14
#define FACTION_CLASS_INDEPENDENT_STRONGHOLDS 15

#define FACTION_ACTION_COOPERATE 1
#define FACTION_ACTION_COMPETE 2

#define FACTION_ACTION_FIGHT 3
#define FACTION_ACTION_ALLIANCE 4

#define FACTION_ACTION_INTEGRATE 5
#define FACTION_ACTION_DIVEST 6

#define FACTION_ACTION_CENTRALIZE 7
#define FACTION_ACTION_DECENTRALIZE 8

#define FACTION_ACTION_EQUALIZE_RESOURCES 9
#define FACTION_ACTION_REDIRECT_RESOURCES 10

#define FACTION_ACTION_LEARN_FROM_FOREIGNER 11

#define FACTION_ACTION_ECONOMICALLY_SPECIALIZE 12
#define FACTION_ACTION_ECONOMICALLY_GENERALIZE 13

#define FACTION_ACTION_MIGRATE_TO_FOREIGN_NATION 14
#define FACTION_ACTION_CALL_IN_EXPATS 15

#define FACTION_ACTION_EXECUTE_PROTOCOL_PROGRAM 16
#define FACTION_ACTION_STOP_PROTOCOL_PROGRAM 17

#define FACTION_ACTION_REDRAW_BORDERS 18
#define FACTION_ACTION_EXPAND_BORDERS 19
#define FACTION_ACTION_CONTRACT_BORDERS 20

#define FACTION_ACTION_EXECUTE_PROXY_PROTOCOL_PROGRAM 21
#define FACTION_ACTION_STOP_PROXY_PROTOCOL_PROGRAM 22

#define FACTION_ACTION_CREATE_INSIDER_GROUP 23
#define FACTION_ACTION_PROGRAM_INSIDER_GROUP 24
#define FACTION_ACTION_ABOLISH_INSIDER_GROUP 25
#define FACTION_ACTION_STOKE_DISORDER 26
#define FACTION_ACTION_STABILIZE_DISORDER 27

#define FACTION_ACTION_START_MUTUALIST_NETWORK 28
#define FACTION_ACTION_START_COMMENSALIST_NETWORK 29
#define FACTION_ACTION_START_PARASITIC_NETWORK 30

#define FACTION_ACTION_END_MUTUALIST_NETWORK 31
#define FACTION_ACTION_END_COMMENSALIST_NETWORK 32
#define FACTION_ACTION_END_PARASITIC_NETWORK 33

#define FACTION_ACTION_BREAK_OFF_FROM_NETWORK 34
#define FACTION_ACTION_JOIN_NETWORK 35
#define FACTION_ACTION_SWAP_NETWORK 36

#define FACTION_ACTION_CREATE_BRANCH 37
#define FACTION_ACTION_ABOLISH_BRANCH 38

#define FACTION_ACTION_RECRUIT_MEMBERS 39
#define FACTION_ACTION_REASSIGN_MEMBERS 40
#define FACTION_ACTION_ERADICATE_MEMBERS 41
#define FACTION_ACTION_ARREST_MEMBERS 42


//Faction agents can choose between
//Deterministic and Entropic choices.
//Loyal vs Self-Centered Choices.
//Altruistic vs Selfish Choices.

//This can change based on the environment and context! This keeps things interesting and unpredictable.


//11. Conspiracies that nobody controls
//10. Conspiracies that don't exist
//9. Elite capture
//8. "Soft" rebellion
//7. Administrative divisions as political actors
//6. Internal diplomacy
//5. Secret knowledge
//4. Competing definitions of "the national interest"
//3. Plausible deniability
//2. Secret agreements between subnational actors
//1. Informal power networks



//40,075 km per 4096 px

//Small surface action (4–12 ships per side): about 0.2–0.8 ships forced to retreat per hour.
//Major fleet engagement (20–100+ ships involved): about 1–4 ships forced to retreat per hour during periods of active combat.
//Peak moments (such as concentrated air strikes or torpedo attacks) could temporarily reach 5–10 ships per hour, but these bursts rarely lasted long.

#define SHIPS_DESTROYED_PER_HOUR_IN_SKIRMISH_PERCENTAGE 1

#define WORLD_SEA_TEMPERATURE_CELCIUS 2.5

#define WORLD_MAP_KM_PER_PIXEL_HORIZONTAL 9.78393554688
#define WORLD_MAP_KM_PER_PIXEL_VERTICAL 17.3936631944

#define SONIC_VORTEX_KM_H 300 //8 days to go 40,075 km.
#define MAXIMUM_CRUISING_VELOCITY_KM_H 150
//This is the Sonic Vortex.
//If I cap the velocity at 210, then it becomes much more difficult to game the system.




#define TOTAL_POPULATION 1000000000

#define TOTAL_POPULATION_IN_GROUPS 729000000

#define TOTAL_POPULATION_PER_GROUP 11390625

#define AVERAGE_SHIPS_PER_GROUP 15400 //Total population/370, then half are in bases, not ships

#define AVERAGE_BASES_PER_GROUP 154 //Bases house 100 times as many people as ships.

//Key Functions of Navy BasesFleet Support and Maintenance: Provide piers, dry docks, and shipyards to perform routine maintenance, major repairs, and restocking of food, fuel, and ammunition.Operational Command: Act as headquarters and launch points for global maritime operations, combat readiness, and power projection.Aviation and Submarine Support: House specialized facilities like Naval Air Stations for aircraft squadrons and submarine bases for undersea fleets.Personnel and Training: Train sailors and support service members and their families by providing housing, medical clinics, recreational areas, and administrative command centers.

//Divided by 8 per sector divided by 8 groups in each sector


//Bases include:
//Hanging gardens: Hot air balloon-powered farms.
//Energy Rigs
//Undersea Cities
//Shipyards
//Manufacturing Plants

#define POPULATION_RATIO_VIRGINIA 32.4
#define POPULATION_RATIO_NOVA 22.0
#define POPULATION_RATIO_KASORI 13.3
#define POPULATION_RATIO_SHONGUO 9.4
#define POPULATION_RATIO_SAXONY 5.7
#define POPULATION_RATIO_KOGURYEO 5.4
#define POPULATION_RATIO_AMAZONIA 3.5
#define POPULATION_RATIO_ANGLORIA 2.9
#define POPULATION_RATIO_HISPANIOLA 2.8
#define POPULATION_RATIO_ROSSIYA 1.0
#define POPULATION_RATIO_MISC 1.6








#define PLAYFIELD_MAXIMUM_X 4095
#define PLAYFIELD_MAXIMUM_Y 2303

#define ISLAND_X 2600
#define ISLAND_Y 1470

#define FLEET_DENSITY_PER_20000KM 1 //1 fleet per ~17,700 km² of ocean

//Fleet Sizes
//1
//2
//4
//8
//16
//32
//64
//128
//256





struct _Faction_Policy_Action
{
    unsigned int Action_ID;
    struct _Universal_ID Recipient_Faction;
}

struct _Faction_Node_Connection
{
    struct _Universal_ID Faction_Node_IDs[2];
    double Direction;
}

struct _Faction_Node
{
    struct _Universal_ID Faction_ID;
    struct _Universal_ID Faction_Node_ID;
    struct _Universal_ID Faction_Leader_ID[15];
    bool Is_Led_By_A_Council;
    unsigned short Number_Of_Council_Members;
    struct _Faction_Policy_Action Current_Policy_Actions[256];
    unsigned short End_Goals[2];
    unsigned short Faction_Class;

    //Deterministic and Entropic choices.
    //Loyal vs Self-Centered Choices.
    //Altruistic vs Selfish Choices.

    char Deterministic_Entropic_Axis;
    char Loyal_Self_Centered_Axis;
    char Altruistic_Selfish_Axis;
    char Ethics_Survival_Axis;
    char Covert_Overt_Action_Axis;
    char Gullible_Paranoid_Axis;


    bool Leader_Power_MAKING_LAWS;
    bool Leader_Power_EXECUTING_FEDERAL_LAWS;
    bool Leader_Power_ENFORCING_FEDERAL_LAW;
    bool Leader_Power_NATIONAL_DEFENSE;
    bool Leader_Power_INTELLIGENCE_AND_NATIONAL_SECURITY;
    bool Leader_Power_FOREIGN_RELATIONS;
    bool Leader_Power_MANAGING_MONEY_AND_TAXATION;
    bool Leader_Power_BANKING_AND_FINANCIAL_REGULATION;
    bool Leader_Power_PUBLIC_HEALTH;
    bool Leader_Power_SOCIAL_WELFARE;
    bool Leader_Power_EDUCATION;
    bool Leader_Power_INFRASTRUCTURE_AND_TRANSPORTATION;
    bool Leader_Power_NATURAL_RESOURCES_AND_ENVIRONMENT;
    bool Leader_Power_AGRICULTURE_AND_FOOD;
    bool Leader_Power_SCIENCE_AND_RESEARCH;
    bool Leader_Power_SPACE;
    bool Leader_Power_DISASTER_RESPONSE;
    bool Leader_Power_VETERANS;
    bool Leader_Power_LABOR_AND_WORKPLACE_REGULATION;
    bool Leader_Power_CIVIL_RIGHTS_AND_DISCRIMINATION_ENFORCEMENT;
    bool Leader_Power_ELECTIONS_AND_DEMOCRATIC_ADMINISTRATION;
    bool Leader_Power_JUSTICE_AND_CONSTITUTIONAL_INTERPRETATION;
    bool Leader_Power_INDEPENDENT_REGULATORY_AGENCIES;
    bool Leader_Power_QUASI_INDEPENDENT_INSTITUTIONS;
    bool Leader_Power_FEDERAL_LANDS_AND_TERRITORIES;
    bool Leader_Power_OCCUPIED_PEOPLES_AFFAIRS;
    bool Leader_Power_FEDERAL_PERSONNEL_AND_ADMINISTRATION;
    bool Leader_Power_GOVERNMENT_OVERSIGHT;
    bool Leader_Power_COMMUNICATIONS_AND_INFORMATION;
}

struct _Faction_Array_Header
{
    unsigned short Number_Of_Faction_Nodes;
    unsigned short Number_Of_Faction_Connections;
    int Offset_To_Faction_Nodes;
    int Offset_To_Faction_Connections;
}

struct _Faction_Node_Packet
{
    //A ship which belongs to a node.
    unsigned short x;
    unsigned short y;

    struct _Universal_ID ID;
    struct _Universal_ID Faction_ID;
    struct _Universal_ID Faction_Node_ID;

    struct _Universal_ID Population[65536];
}

//WIRELESS COMMS ARE NOT POSSIBLE BEYOND 10KM. Factions must dock at bases to communicate over undersea wire.

#define COMM_VELOCITY_PIXELS_PER_HOUR 0.5 //1 pixel is 10km, speed of sound is 5000kmh

//To get your messages, you must go to a Gordias Array.

//These are living post offices with no supersonic drive and a mind and ambitions of their own.

struct _Faction_Array_Object
{
    unsigned short Number_Of_Faction_Nodes;
    unsigned short Number_Of_Faction_Connections;

    struct _Faction_Node* Faction_Nodes[65536];
    struct _Faction_Node_Connection* Faction_Connections[65536];

    unsigned int Number_Of_Faction_Node_Packets;
    struct _Faction_Node_Packet* Faction_Node_Packets[2700000];
}

struct _Faction_Array_Object* Create_Faction_Array_Object();
void Initialize_Faction_Array_Object(struct _Faction_Array_Object* Faction_Array_Object);
void Process_Faction_Array_Object(struct _Faction_Array_Object* Faction_Array_Object);

#endif // FACTION_H_INCLUDED
