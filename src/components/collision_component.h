#ifndef _COLLISION_COMPONENT_H_
#define _COLLISION_COMPONENT_H_

enum BoundingType {
    MESH = 0,
    CIRCLE = 1,
    BOX = 2
};

struct sCollider {
    BoundingType bound;
};

struct sColliderComponent {

    

    bool testCollitionTo(sColliderComponent &element, int element_id);
    bool testCollitionToEverything(sColliderComponent &element);
    //bool testCollitionBetween
    //bool testRayCollitionTo(Ray, int element_id);
    //bool testRayCollitionToEveything(Ray);
    
};

#endif