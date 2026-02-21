#ifndef GLOBE_H_INCLUDED
#define GLOBE_H_INCLUDED

#define GLOBE_CIRCUMFERENCE_IN_KM 40000

#define GLOBE_SHANNON_ISLAND_X 25000 //Shannon island is at x=2500, or 25,000km.
#define GLOBE_SHANNON_ISLAND_Y 11700 //Shannon island is at y=1170, or 11,700km.
#define GLOBE_SHANNON_ISLAND_RADIUS_KM 2 //The biggest, nicest island. 4096 x 4096 map

#define GLOBE_ATTU_ISLAND_X 26000
#define GLOBE_ATTU_ISLAND_Y 12000
#define GLOBE_ATTU_RADIUS_KM 0.3 //3km. The rocky desolate wasteland. Constantly fought over. Closest to Shannon. No places to hide.

#define GLOBE_FOMENTERA_ISLAND_X 17800
#define GLOBE_FOMENTERA_ISLAND_Y 14000
#define GLOBE_FOMENTERA_RADIUS_KM 0.26 //-Battleship Island-Contains Stonehenge Turret Network-Contains airport-Fortified against hypercanes

#define GLOBE_CAPE_VERDE_ISLAND_X 14000
#define GLOBE_CAPE_VERDE_ISLAND_Y 6000
#define GLOBE_CAPE_VERDE_RADIUS_KM 0.08 //Cape Verde: 4 sq km-Smallest Island-Out of the way of Hypercanes.

#define GLOBE_NOSSI_BETA_ISLAND_X 32700
#define GLOBE_NOSSI_BETA_ISLAND_Y 8400
#define GLOBE_NOSSI_BETA_RADIUS_KM 0.7 //Nosy Beta: 160 sq km(The Beautiful)(The ancient ruins)(DONE)Looks nice In the way of hypercanes. Contains ancient ruins Furthest from Shannon

#define GLOBE_NOSSI_ALPHA_ISLAND_X 27000
#define GLOBE_NOSSI_ALPHA_ISLAND_Y 17000
#define GLOBE_NOSSI_ALPHA_RADIUS_KM 0.3//-Lush Desert-In the way of hypercanes.-Second closest to Shannon

#define GLOBE_HEARD_ISLAND_X 27000
#define GLOBE_HEARD_ISLAND_Y 3300
#define GLOBE_HEARD_RADIUS_KM 0.35//-The Dump; thousands of neominerals reside herd -Out of the way of Hypercanes.

#define COORDINATE_ADJUSTMENT_X -20000
#define COORDINATE_ADJUSTMENT_Y -20000

#define GAME_WORLD_EFFECTIVE_RADIUS_KM 5 //Beyond this, nothing happens.


struct _Global_Coordinates
{
    double Radius_KM;
    double Azimuthal_Angle;
    double Polar_Angle;
};

struct _Cylindrical_Coordinates //On a ship
{
    double Radius;
    double Azimuthal_Angle;
    double Height;
};

double Get_Distance_From_Planet_Core(double x, double y, double z);
double Get_Azimuthal_Angle(double y, double x);
double Get_Polar_Angle(double z, double Radius);

double Get_Cylindrical_Radius();
double Get_Cylindrical_Azimuthal_Angle();
double Get_Cylindrical_Height();

#endif // GLOBE_H_INCLUDED


