#include "ApiExtension.h"

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

        // Assuming you are storing entity IDs or pointers as intptr_t directly in the userData
        int userDataA = fixtureA->GetBody()->GetUserData();
        int userDataB = fixtureB->GetBody()->GetUserData();

        // Call the callback with user data from both fixtures
        beginContactCallback(userDataA, userDataB);
    }
}

void ContactListener::EndContact(b2Contact* contact)
{
    // similar to BeginContact
    if(endContactCallback)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        intptr_t userDataA = fixtureA->GetBody()->GetUserData().pointer;
        intptr_t userDataB = fixtureB->GetBody()->GetUserData().pointer;

        endContactCallback(userDataA, userDataB);
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
    shape.SetAsBox(width, height);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.isSensor = isSensor;
    fixtureDef.userData.pointer = body->GetUserData().pointer;
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
    fixtureDef.userData.pointer = body->GetUserData().pointer;
    return body->CreateFixture(&fixtureDef);
}

b2Body* CreateBody(b2World* world, const b2BodyDef* def)
{
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


