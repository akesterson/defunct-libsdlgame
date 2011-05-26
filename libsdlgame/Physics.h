#ifndef __PHYSICS_H__
#define __PHYSICS_H__

// x = pounds to convert to newtons
#define CALC_CONV_LB2NEWTON(x) ((x)*4.44822162)
// x = ounces to convert to newtons
#define CALC_CONV_OZ2NEWTON(x) (((x)*4.44822162)/16)
// x = kilograms to convert to newtons
#define CALC_CONV_KG2NEWTON(x) ((x)*9.81)
// x = milligrams to convert to newtons
#define CALC_CONV_MG2NEWTON(x) (((x)*9.81)/1000

// W = weight in newtons, g = constant acceleration of gravity
#define CALC_MASSFROMWEIGHT(W, g) ((W)/(g))
// F = Force applied to object, a = acceleration of object
#define CALC_MASSFROMFORCE(F, a) ((F)/(a))
// v = linear velocity of object, r = radius of circular path
#define CALC_ACCEL_CENTRIPETAL(v, r) (((v)*(v))/(r))
// v1 = starting velocity, v2 = ending velocity, t = time elapsed between v1 and v2
#define CALC_ACCEL_VELOCITYDELTA(v1, v2, t) (((v2)-(v1)) / (t))
// F = force applied to object, m = mass of object
#define CALC_ACCEL_FORCE(F, m) ((F)/(m))
// d = distance object has traveled, t = time object has spent in motion
#define CALC_VELOCITY_AVG(d, t) ((d)/(t))
// W = Weight of object, H = Height of object above the ground plane
#define CALC_ENERGY_POTENTIAL(W, H) ((W)*(H))
// m = mass of object, v = velocity of object
#define CALC_ENERGY_KINETIC(m, v) (((m)*(v))/2)


#define PHYSICS_NOPHYSICS		0000000000000000b
#define PHYSICS_FRICTIONLESS		0000000000000001b
#define PHYSICS_NOGRAVITY		0000000000000010b
#define PHYSICS_MATERIAL_LIQUID		0000000000000100b
#define PHYSICS_MATERIAL_GAS		0000000000001000b
#define PHYSICS_MATERIAL_SOLID		0000000000010000b

struct WorldPhysics
{
    float gravity;
    float airfriction;
}

struct StaticPhysics
{
    float friction;
    float bouyancy;
    int flags;
}

struct ObjectPhysics : public StaticPhysics
{
    float mass;
    float weight;
    float p1; // position 1, used for calculating velocity
    float p2; // position 2, ""    "     "            "   
    float v1; // velocity 1, used for calculation acceleration
    float v2; // velocity 2, "     "      "           "
    float pe; // potential energy of an object at rest
    float ke; // kinetic energy of a moving object
}

#endif

