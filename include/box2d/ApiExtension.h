#ifndef APIEXTENSION_H
#define APIEXTENSION_H

#include "box2d/box2d.h"

typedef void (*ContactCallback)(intptr_t userDataA, intptr_t userDataB);

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

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) b2World* CreateWorld(b2Vec2 gravity);
    __declspec(dllexport) void DeleteWorld(b2World* world);
    __declspec(dllexport) void SetContactListener(b2World* world, ContactListener* listener);
    __declspec(dllexport) b2Fixture* CreateBoxFixture(b2Body* body, float width, float height, float density, bool isSensor);
    __declspec(dllexport) b2Fixture* CreateCircleFixture(b2Body* body, float radius, float density, bool isSensor);
    
    __declspec(dllexport) b2Body* CreateBody(b2World* world, const b2BodyDef* def);
    __declspec(dllexport) void SetLinearVelocity(b2Body* body, const b2Vec2& velocity);
    __declspec(dllexport) void GetLinearVelocity(b2Body* body, b2Vec2* velocity);
    __declspec(dllexport) void GetBodyPosition(b2Body* body, b2Vec2* position);
    __declspec(dllexport) void SetBodyPosition(b2Body* body, const b2Vec2& position);

    // contact listener
    __declspec(dllexport) ContactListener* CreateContactListener();
    __declspec(dllexport) void SetBeginContactCallback(ContactListener* listener, ContactCallback callback);
    __declspec(dllexport) void SetEndContactCallback(ContactListener* listener, ContactCallback callback);
    __declspec(dllexport) void RegisterContactListener(b2World* world, ContactListener* listener);
    
#ifdef __cplusplus
}
#endif

#endif // APIEXTENSION_H
