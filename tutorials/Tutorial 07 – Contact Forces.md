# Tutorial – Contact Forces

## Introduction and Objective:
By now you should have a physics engine that supports collision detection and resolution with at least two shapes (circles and planes at a minimum, but ideally also boxes).

As we noted in the previous tutorial, if your shapes remain penetrating each other after a collision then the forces generated can quickly build and the physics engine can become unpredictable (basically, the shapes will keep applying forces to each other for every update in which they are penetrating).

In this tutorial we will solve this problem by applying “contact forces” that move the shapes after collision so that they no longer overlap.

## Prerequisites:
We will be using the DIY Physics engine that you have been writing.
Ensure you have completed the following tutorials:
- ***Fixed Timestep Tutorial. Available in the Introduction to Physics session.***
- ***Linear Force and Momentum Tutorial. Available in the Linear Force and Momentum session.***
- ***Collision Detection Tutorial. Available in the Collision Detection session.***
- ***Collision Resolution: Introduction Tutorial. Available in the Collision Resolution session.***
- ***Collision Resolution: Circle-to-Circle Tutorial. Available in the Collision Resolution session.***
- ***Collision Resolution: Circle-to-Plane Tutorial. Available in the Collision Resolution session.***
- ***Rotational Force, part 1. Available in the Rotational Force session.***
- ***Rotational Force, part 2. Available in the Rotational Force session.***
- ***Collision Resolution: Static and Dynamic Friction. Available in the Collision Resolution session.***

Although recommended, it is not necessary to have completed the following previous tutorials:
- ***Projectile Physics part 1: Analytical Solution. Available in the Projectile Physics session.***
- ***Projectile Physics part 2: Numerical Integration. Available in the Projectile Physics session.***

## Contact Forces and Penetration:
If two RigidBodys are overlapping, but have no velocity (say they’ve been initialized this way), then our force-based collision detection and resolution can do nothing to separate them.

We can fix this problem by applying “contact forces” that directly move the objects so that they don’t overlap, and do this along with the application of forces.

We can do this in the ResolveCollision() functions for Rigidbody and Plane. All we need in both cases are a collision normal and a degree of penetration. The types of rigidbody (Circle or Box) don’t matter as long as we have this information.

The total contact force is calculated as the amount of penetration of the bodies times the normalized displacement vector.

We need to work out how best to divide this displacement between the two objects involved.
- If one of them is a kinematic Rigidbody, or a Plane, then the other one should do all the movement.
- If two Rigidbodies of equal mass are involved, then they should each take half of the displacement.

What about two Rigidbodies of unequal mass? Rather than use a binary system like this, we can make each body move proportional to the other one’s mass.
    Body 1 moves by mass2/(mass1 + mass2) * total displacement
    Body 2 moves by mass1/(mass1 + mass2) * total displacement

A plane counts as having infinite mass for these purposes.

We can add a general purpose function to PhysicsScene for applying displacements between two objects a given vector for a given degree of penetration:

``` c++
// body2 can be null for a Plane
void PhysicsScene::ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen)
{
	float body2Mass = body2 ? body2->GetMass() : INT_MAX;

	float body1Factor = body2Mass / (body1->GetMass() + body2Mass);

	body1->SetPosition(body1->GetPosition() - body1Factor * norm * pen);
	if (body2) 
        body2->SetPosition(body2->GetPosition() + (1 - body1Factor) * norm * pen);
}
```
We can use this for Planes as well by passing in a nullptr value for body2.
Make this function static, so we can call it without a pointer to the PhysicsScene.
It would be ideal to call this from Rigidbody::ResolveCollision, since that is a general function used for all types of rigidbody-pair interactions.

To do so we need to add an extra parameter and pass the penetration through.

``` c++
void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr, float pen = 0);
```

We can then call this from inside ResolveCollision like so:

``` c++
if (v1 > v2) // they're moving closer
{
    ...
    ...
    if (pen > 0)
	    PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
}
```

All we need to do now is pass in the correct value of penetration for the three cases where we call ResolveCollision. We haven’t explicitly calculated the penetration in most cases, but can easily re-arrange our code to do so.

Circle2Circle: We were comparing the distance to the sum of the radii. Replace that with a subtraction to get the degree of overlap.

``` c++
bool PhysicsScene::Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	...
    ...
		// if the circles touch, set their velocities to zero for now
		float penetration = circle1->GetRadius() + circle2->GetRadius() - dist;
		if (penetration > 0)
		{
			circle1->ResolveCollision(circle2, (circle1->GetPosition() + circle2->GetPosition())*0.5f, nullptr, penetration);
			return true;
		}
	}
	return false;
}
```

Box2Box: We already had a penetration calculated, so just need to pass it through.

``` c++
bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
	...
    ...
		if (pen > 0) {
			box1->ResolveCollision(box2, contact / float(numContacts), &norm, pen);
		}
		return true;
	}
	return false;
}

```

In Circle2Box (or Box2Circle): we replace the direct comparison between radius and distance to closest point with a subtraction.
``` c++
bool PhysicsScene::Circle2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Circle* circle = dynamic_cast<Circle*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (box != nullptr && circle != nullptr)
	{
        ...
        ...

		glm::vec2 circleToBox = circle->GetPosition() - closestPointOnBoxWorld;
		float penetration = circle->GetRadius() - glm::length(circleToBox);
		if (penetration > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(circle, contact, &direction, penetration);
		}
	}

	return false;
}
```
## Contact Forces and Planes
With a Plane, we know that the collision normal is always the plane normal. If we have a contact point, we can also calculate the degree of penetration using the plane normal and distance from the origin. 

We can add two more lines to Plane::ResolveCollision to get contact force working with planes.
``` c++
actor2->ApplyForce(force, contact - actor2->GetPosition());

float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);

```
Without this, boxes can still have a tendency to sink through planes slowly. This should stop that behaviour.

To test your contact forces, increase drag and decrease elasticity. Try to set up a situation where objects will come to rest on each other, as that is when contact forces come into play.

There may still be some jitter, and we haven’t incorporated sidereal friction in our model, so stacking objects will still be a fraught exercise.

## Next Tutorial:
Springs in Joints and Springs section






