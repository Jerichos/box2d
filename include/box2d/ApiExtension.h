#ifndef APIEXTENSION_H
#define APIEXTENSION_H

#include <vector>

#include "box2d/box2d.h"

typedef void (*ContactCallback)(int entityA, int entityB);

class ContactListener : public b2ContactListener
{
public:
    ContactListener();
    void SetBeginContactCallback(ContactCallback callback);
    void SetEndContactCallback(ContactCallback callback);
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

private:
    ContactCallback beginContactCallback;
    ContactCallback endContactCallback;
};

class EntityQueryCallback : public b2QueryCallback
{
public:
    std::vector<int> entities;
    int ignoreEntity = -1;

    EntityQueryCallback(int ignoreEntity = -1) : ignoreEntity(ignoreEntity) { }
    
    bool ReportFixture(b2Fixture* fixture) override
    {
        // Assuming the user data is stored as an integer ID directly
        const int entity = fixture->GetBody()->GetUserData().entity;

        if(entity == ignoreEntity)
        {
            return true;
        }
        
        if (entity) {
            entities.push_back(entity);
        }
        return true; // Continue querying
    }
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) b2World* CreateWorld(b2Vec2 gravity);
    __declspec(dllexport) void DeleteWorld(b2World* world);
    __declspec(dllexport) void DebugDraw(b2World* world);
    __declspec(dllexport) void SetContactListener(b2World* world, ContactListener* listener);
    __declspec(dllexport) b2Fixture* CreateBoxFixture(b2Body* body, float width, float height, float density, bool isSensor);
    __declspec(dllexport) b2Fixture* CreateCircleFixture(b2Body* body, float radius, float density, bool isSensor);
    
    __declspec(dllexport) b2Body* CreateBody(b2World* world, b2BodyDef* def, int entityID);
    __declspec(dllexport) void SetLinearVelocity(b2Body* body, const b2Vec2& velocity);
    __declspec(dllexport) void SetBodyPosition(b2Body* body, const b2Vec2& position);
    __declspec(dllexport) void GetLinearVelocity(b2Body* body, b2Vec2* velocity);
    __declspec(dllexport) void GetBodyPosition(b2Body* body, b2Vec2* position);
    __declspec(dllexport) int GetBodyEntity(b2Body* body);

    // contact listener
    __declspec(dllexport) ContactListener* CreateContactListener();
    __declspec(dllexport) void SetBeginContactCallback(ContactListener* listener, ContactCallback callback);
    __declspec(dllexport) void SetEndContactCallback(ContactListener* listener, ContactCallback callback);
    __declspec(dllexport) void RegisterContactListener(b2World* world, ContactListener* listener);

    // raycast and query
    // let's add optional parameter for entity which will be ignored
    __declspec(dllexport) bool QueryAABB(b2World* world, const b2AABB& aabb, int* entities, int& count, int ignoreEntity = -1);
    
#ifdef __cplusplus
}
#endif

#endif // APIEXTENSION_H
