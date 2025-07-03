#ifndef PERSON_H
#define PERSON_H

#include "raylib.h"
#include <btBulletDynamicsCommon.h>
#include <string>

enum class PersonType {
    ENEMY,
    ALLY,
    NEUTRAL
};

class Person {
public:
    Person(Vector3 position, PersonType type);
    ~Person();

    // Update & render
    void Update(float deltaTime);
    void Render() const;

    // Position
    Vector3 GetPosition() const;
    void SetPosition(Vector3 pos);

    // Physics
    btRigidBody* GetRigidBody();

    // Type info
    PersonType GetType() const;
    std::string GetTypeName() const;


    btRigidBody* body = nullptr;

private:
    void InitPhysics(Vector3 position);
    void CleanupPhysics();

    Vector3 position;
    PersonType type;

    // Bullet Physics components
    btCollisionShape* shape = nullptr;
    btDefaultMotionState* motionState = nullptr;
};

#endif // PERSON_H
