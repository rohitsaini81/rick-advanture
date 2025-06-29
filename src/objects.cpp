// objects.cpp
#include "objects.h"
#include "physics.h" // make sure dynamicsWorld is visible
#include "iostream"
#include "elements.h" 
Elements* objectal[4]; // array of pointers
// std::vector<Elements> elementList;
std::vector<Elements*> elementList;

void CREATE_ELEM() {
    try {
        if (dynamicsWorld) {
            std::cout << "fine working ?? " << std::endl;
            Elements box(ElementType::BOX, dynamicsWorld);
            std::cout << "Note------------------------" << box.mass << std::endl;
            std::cout << "dynamicsWorld ptr: " << dynamicsWorld << std::endl;
            std::cout << "box.body ptr: " << box.body << std::endl;

            if (dynamicsWorld && box.body) {
                std::cout << "About to add rigid body\n";
                dynamicsWorld->addRigidBody(box.body);


        

                Elements* newElem = new Elements(ElementType::BOX, dynamicsWorld);
                elementList.push_back(newElem);
                
                
                
                // objectal[0] = new Elements(ElementType::BOX, dynamicsWorld);                std::cout << "Rigid body added\n";
            } else {
                std::cerr << "Invalid world or body\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught unknown exception" << std::endl;
    }
}
