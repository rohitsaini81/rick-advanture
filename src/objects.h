#ifndef OBJECTS
#define OBJECTS
#include "elements.h"
#include "vector"
#include "LinearMath/btAlignedObjectArray.h"
#include "time.h"
#include "raylib.h"
void CREATE_ELEM();

extern Elements* objectal[4];
// extern std::vector<Elements> elementList;

extern std::vector<Elements*> elementList;
extern std::vector<btRigidBody*> rigidBodyList;
extern btAlignedObjectArray<btRigidBody*> rigidBodies;

extern Color GetRandomColor();
#endif