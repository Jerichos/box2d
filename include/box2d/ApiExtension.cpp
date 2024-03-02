#include "ApiExtension.h"

#include <cstdio>

ContactListener::ContactListener():
    beginContactCallback(nullptr),
    endContactCallback(nullptr){ }

void ContactListener::SetBeginContactCallback(ContactCallback callback)
{
    beginContactCallback = callback;
}

void ContactListener::SetEndContactCallback(ContactCallback callback)
{
    endContactCallback = callback;
}

void ContactListener::BeginContact(b2Contact* contact)
{
    if(beginContactCallback)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        auto userDataA = fixtureA->GetBody()->GetUserData().entity;
        auto userDataB = fixtureB->GetBody()->GetUserData().entity;

        // Check if either or both fixtures are sensors
        bool isSensorA = fixtureA->IsSensor();
        bool isSensorB = fixtureB->IsSensor();

        if (isSensorA && !isSensorB) {
            // Fixture A is sensor, invoke callback with (A, B)
            beginContactCallback(userDataA, userDataB);
        } else if (!isSensorA && isSensorB) {
            // Fixture B is sensor, invoke callback with (B, A)
            beginContactCallback(userDataB, userDataA);
        } else if (isSensorA && isSensorB) {
            // Both are sensors, invoke callback twice: once with (A, B), then with (B, A)
            beginContactCallback(userDataA, userDataB);
            beginContactCallback(userDataB, userDataA);
        }
    }
}

void ContactListener::EndContact(b2Contact* contact)
{
    // similar to BeginContact
    if(endContactCallback)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        auto userDataA = fixtureA->GetBody()->GetUserData().entity;
        auto userDataB = fixtureB->GetBody()->GetUserData().entity;

        // Check if either or both fixtures are sensors
        bool isSensorA = fixtureA->IsSensor();
        bool isSensorB = fixtureB->IsSensor();

        if (isSensorA && !isSensorB) {
            // Fixture A is sensor, invoke callback with (A, B)
            endContactCallback(userDataA, userDataB);
        } else if (!isSensorA && isSensorB) {
            // Fixture B is sensor, invoke callback with (B, A)
            endContactCallback(userDataB, userDataA);
        } else if (isSensorA && isSensorB) {
            // Both are sensors, invoke callback twice: once with (A, B), then with (B, A)
            endContactCallback(userDataA, userDataB);
            endContactCallback(userDataB, userDataA);
        }
    }
}

// contact listener
ContactListener* CreateContactListener()
{
    return new ContactListener();
}

void SetBeginContactCallback(ContactListener* listener, ContactCallback callback)
{
    if(listener)
        listener->SetBeginContactCallback(callback);
}

void SetEndContactCallback(ContactListener* listener, ContactCallback callback)
{
    if(listener)
        listener->SetEndContactCallback(callback);
}

void RegisterContactListener(b2World* world, ContactListener* listener)
{
    if(world && listener)
        world->SetContactListener(listener);
}

// exported functions
b2World* CreateWorld(b2Vec2 gravity)
{
    return new b2World(gravity);
}

void DeleteWorld(b2World* world)
{
    delete world;
}

b2Fixture* CreateBoxFixture(b2Body* body, float width, float height, float density, bool isSensor)
{
    b2PolygonShape shape;
    shape.SetAsBox(width / 2, height / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.isSensor = isSensor;
    return body->CreateFixture(&fixtureDef);
}

b2Fixture* CreateCircleFixture(b2Body* body, float radius, float density, bool isSensor)
{
    b2CircleShape circleShape;
    circleShape.m_radius = radius;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = density;
    fixtureDef.isSensor = isSensor;
    return body->CreateFixture(&fixtureDef);
}

b2Body* CreateBody(b2World* world, b2BodyDef* def, int entityID)
{
    // print user data
    printf("entityID: %zu\n", entityID);
    // set entity id as user data to def
    def->userData.entity = entityID;
    // print pointer
    printf("def->userData.pointer: %zu\n", def->userData.pointer);
    return world->CreateBody(def);
}

void SetLinearVelocity(b2Body* body, const b2Vec2& velocity)
{
    body->SetLinearVelocity(velocity);
}

void SetBodyPosition(b2Body* body, const b2Vec2& position)
{
    body->SetTransform(position, body->GetAngle());
}

void GetLinearVelocity(b2Body* body, b2Vec2* velocity)
{
    *velocity = body->GetLinearVelocity();
}

void GetBodyPosition(b2Body* body, b2Vec2* position)
{
    *position = body->GetPosition();
}

int GetBodyEntity(b2Body* body)
{
    return body->GetUserData().entity;
}

bool QueryAABB(b2World* world, const b2AABB& aabb, int* entities, int& count, int ignoreEntity)
{
    EntityQueryCallback callback(ignoreEntity);
    world->QueryAABB(&callback, aabb);
    int maxSize = count;
    count = std::min(static_cast<int>(callback.entities.size()), maxSize);
    for(int i = 0; i < count; ++i)
    {
        entities[i] = callback.entities[i];
    }

    // return true if at least one entity is found
    return count > 0;
}

void DebugDraw(b2World* world)
{
    world->DebugDraw();
}
