#ifndef FACTION_H_INCLUDED
#define FACTION_H_INCLUDED

//authoritarian military remnants, cannibalistic raiders, isolationist communities, doomsday cults, trade-hub settlements, and organized scientific factions

//Military Remnants
//Survivor Communities
//Raiders/Bandits
//Authoritarian Regimes
//Religious or Ideological Cults
//Scientists & Researchers
//Government Remnants
//Nomads
//Preppers
//Mercenaries/Private Forces
//Traders
//Criminal Syndicates
//Isolationists
//Humanitarian Groups
//Scavengers
//Independent Strongholds

//The Engineers restore a hydroelectric dam.
//The Seed Bank expands agriculture using the new irrigation.
//The Railroad distributes food across the region.
//The Postal Service keeps settlements connected.
//The Archivists preserve technical manuals and train new engineers.
//The Arbitration Council negotiates water-sharing agreements.
//The Cartographers update maps as roads reopen.

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




#define POPULATION 1000000000

#define SHIP_TOTAL 10000000

#define SHIPS_IN_FACTIONS 2300000
#define SHIPS_PER_FACTION 36510

#define SMALL_GROUPS_POPULATION 2375000
#define LARGE_GROUPS_POPULATION 78000000

#define POPULATION_IN_FACTIONS 230000000

#define POPULATION_RATIO_VIRGINIA 0.324
#define POPULATION_RATIO_NEATO 0.22
#define POPULATION_RATIO_KASORI 0.133
#define POPULATION_RATIO_SHONGUO 0.094
#define POPULATION_RATIO_SAXONY 0.057
#define POPULATION_RATIO_KOGURYEO 0.054
#define POPULATION_RATIO_AMAZONIA 0.035
#define POPULATION_RATIO_ANGLORIA 0.029
#define POPULATION_RATIO_HISPANIOLA 0.028
#define POPULATION_RATIO_ROSSIYA 0.01
#define POPULATION_RATIO_MISC 0.016

#define STATUS_HOT_WAR 1
#define STATUS_COLD_WAR 2
#define STATUS_NEUTRAL 3
#define STATUS_CORDIAL 4
#define STATUS_ALLIES 5
#define STATUS_UNITED 6

#define GROUP_BASER 1 //retain the majority of their core membership over time and largely build bases.
//Tend to stay in one spot and extend a sensor mesh.
//They don't like to split apart.

#define GROUP_RAIDER 2 //groups that grief bases
//They are sparse, fast and lethal. They live in tiny ships and groups of 10 folks.
//They go wherever they please. They are contracted by other groups.
//You either love them or hate them!

#define GROUP_EXCAV 3 //that have made significant contributions to the many highways/
//Excavs tend to move down a straight path. They operate heavy, slow equipment, and contract out their defense.
//They tend to work together.
//Prized for their resources and tools.

struct _Foreign_Relations
{
    unsigned long IDs[2];
    unsigned char Status;
};

struct _Group
{
    unsigned char Name[32];
    unsigned char Type;
    unsigned long ID;
    unsigned long Number_Of_Members;
    double Home_X;
    double Home_Y;
    double Detection_Radius_In_KM;
};


#endif // FACTION_H_INCLUDED
