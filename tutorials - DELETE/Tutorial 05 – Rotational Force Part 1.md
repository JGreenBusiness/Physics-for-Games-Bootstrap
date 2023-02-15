# Tutorial – Rotational Force, part 1

## Introduction and Objective:
In this tutorial we will add rotation to our shapes. This will involve adding variables to hold an object’s current rotation and rotational velocity, as well as calculating torque when two objects collide. (Applying torque after a collision will influence an object’s rotational velocity in the same way that applying a force after a collision modifies an object’s linear velocity).

Implementing rotational forces can be challenging. For circle-to-circle and circle-to-plane collisions, integrating rotations into our collision resolution algorithms will be relatively simple. For boxes, this becomes substantially more difficult.

We’ll start in this tutorial by adding variables for rotation and angular acceleration to the Rigidbody class, and implementing rotation for circles.

In future tutorials we will add a box class (for those of you who haven’t already attempted this), and add rotations to these boxes (giving us an implementation for oriented-bounding-boxes). 

## Prerequisites:
We will be using the DIY Physics engine that you have been writing.
Ensure you have completed the following tutorials:
- ***Fixed Timestep Tutorial. Available in the Introduction to Physics session.***
- ***Linear Force and Momentum Tutorial. Available in the Linear Force and Momentum session.***
- ***Collision Detection Tutorial. Available in the Collision Detection session.***
- ***Collision Resolution: Introduction Tutorial. Available in the Collision Resolution session.***
- ***Collision Resolution: Circle-to-Circle Tutorial. Available in the Collision Resolution session.***
- ***Collision Resolution: Circle-to-Plane Tutorial. Available in the Collision Resolution session.***

Although recommended, it is not necessary to have completed the following previous tutorials:
- ***Projectile Physics part 1: Analytical Solution. Available in the Projectile Physics session.***
- ***Projectile Physics part 2: Numerical Integration. Available in the Projectile Physics session.***

## Adding Rotation Variables to the Rigidbody Class:
To add rotation to our shapes, we’ll add the rotational variables to the Rigidbody class – being the base class of our various shapes.

Add the following variables to the Rigidbody class, and ensure they are initialized in the constructor(s).

``` c++
float m_orientation; 
float m_angularVelocity;
float m_moment;

float m_lastOrientation;
```

The m_orientation variable stores how much the object is rotated in radians (and m_lastOrientation is the angle from the last physics frame).

The m_moment variable represents the moment of inertia, the rotational equivalent of mass. The moment will need to be initialized based on the object’s mass and its shape – the specific formula used will be different for each type of shape.

For a circle:
```c++
 	moment = 0.5f * mass * radius * radius;
```

For a box:
```c++
 	moment = 1.0f / 12.0f * mass * width * height;
```

While we’re updating the Rigidbody header, we’ll also update some function declarations now (since we’ll need to update these functions when adding rotation information into the collision response algorithms).

Update the declarations of the following functions:

``` c++
void ApplyForce(glm::vec2 force, glm::vec2 pos);
~~void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);~~
void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);
```

(I’ve removed the *ApplyForceToActor()* function, since it’s a little redundant, but feel free to append a glm::vec2 position argument if you wish to keep it.)

The *FixedUpdate()* function needs to update the rotation according to the angular velocity. Modify this function as follows:

``` c++
void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);

	m_orientation += m_angularVelocity * timeStep;
}
```
We need to pass a contact point into *ApplyForce()* to say where the force is applied, so that we can calculate the torque applied to the rigidbody. Later we’ll update our collision detection algorithm so that it identifies the contact point.

Note the use of *GetMass()* and *GetMoment()* accessors here instead of directly reading m_mass and m_moment. When we come to implement kinematic bodies this will be important.

We also need to update the *ResolveCollision()* function so that rotational velocity is accounted for when calculating the collision response force.

``` c++
void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	// use GetMass() and GetMoment() here in case we ever get it to do something more than just return mass... (Hint Hint)
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}
```

Our approach with the *ResolveCollision()* function is a little different from the simpler version we implemented previously.

We take their velocity components along the direction of a line of force (the collision normal). The collision normal can be calculated by either joining the centres of mass, or it could be provided by the calling code as an optional parameter. We only need to apply a corrective force if the contact points are moving closer along this line. 

Newton’s Third Law says we will have to apply equal and opposite forces to the two objects. The “effective mass” is calculated for each object (at the contact point) and used to calculate the force to apply to the objects. The effective mass determines how the object will respond to the force both linearly and rotationally.

``` c++
void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, 
glm::vec2* collisionNormal)
{	
	// find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
		actor2->m_position - m_position);
	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// determine the total velocity of the contact points for the two objects, 
// for both linear and rotational 		

		// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);		
	float r2 = glm::dot(contact - actor2->m_position, perp);		
		// velocity of the contact point on this object 
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
		// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + 
    r2 * actor2->m_angularVelocity;
    if (v1 > v2) // they're moving closer
    {
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2*r2) / actor2->m_moment);

		float elasticity = 1;

		glm::vec2 force = (1.0f + elasticity)*mass1*mass2 / 
        (mass1 + mass2)*(v1 - v2)*normal;

		//apply equal and opposite forces
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);
	}	
}
```

## Updating the Collision Detection Functions:
For now, we’ll focus on implementing rotations with circles only. 

Since the only shapes our application currently implements are circles and planes, we’ll need to update the circle-to-circle and circle-to-plane collision detection functions so that a contact point is calculated and passed to the ResolveCollision() function.

In the next tutorial we’ll take a look at how we can get oriented boxes implemented.

For circle-to-plane collisions, the contact point is simply the point on the circumference that lies along the collision normal. Since our function already calculates the collision normal, calculating this point is straightforward:

``` c++
glm::vec2 contact = circle->GetPosition() + (collisionNormal * -circle->GetRadius());
```

Remember that we created an override of the ResolveCollision() function especially for the Plane class. This is because we treat planes as static, and therefore give them an infinite mass. 

We could continue with this special case override if we wish. Rather than using the angular velocity in our force calculation, we could simply ignore it if we wanted to – if we’re only simulating solid circles then we typically won’t be able to tell if they’re rotating or not, so we can fudge things a little.
 
Our Plane::ResolveCollision() function then becomes:
``` c++
void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	// the plane isn't moving, so the relative velocity is just actor2's velocity
	glm::vec2 vRel = actor2->GetVelocity();		
	float e = 1;
	float j = glm::dot(-(1 + e) * (vRel), m_normal) / (1 / actor2->GetMass());

	glm::vec2 force = m_normal * j;

	actor2->ApplyForce(force, contact - actor2->GetPosition());
}

```

Ideally though you should be updating the Rigidbody::ResolveCollision() function so that the function checks to see if one of the collision objects is static (or kinematic) and, if so, uses an infinite mass in the calculations.

For circle-to-circle collisions the contact point is even easier to calculate – it is simply half the sum of the positions. (It’s a little abstract to think of it in those terms, but by adding the positions and dividing by 2 we get the position that’s half-way between the two circles.):

``` c++
circle1->ResolveCollision(circle2, 0.5f * (circle1->GetPosition() + circle2->GetPosition()));
```

Unless you have added more shapes to your engine, you should now be able to compile and test the application.

## Updating the Sphere Class:
In order to see the circles rotating we can add a line from the centre to the end of thir local x-axis. First, we need to add variables to Rigidbody to represent the local x- and y- axes at draw time:

``` c++
    glm::vec2 m_smoothedPosition;
	glm::vec2 m_smoothedLocalX;
	glm::vec2 m_smoothedLocalY;
```

We can calculate them in CalculateSmoothedPosition().
First calculate an interpolated orientation and then get the axis vectors from there using a bit of trigonometry.


``` c++
void Rigidbody::CalculateSmoothedPosition(float alpha)
{
	m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

	float smoothedOrientation = alpha * m_orientation 
                             + (1 - alpha) * m_lastOrientation;
	float sn = sinf(smoothedOrientation);
	float cs = cosf(smoothedOrientation);
	m_smoothedLocalX = glm::vec2(cs, sn);
	m_smoothedLocalY = glm::vec2(-sn, cs);
}
```

We then draw a white line over the top of the circle like so:

``` c++
void Sphere::draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

	aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(1,1,1,1));
}
```

You should see the white lines remain horizontal in all cases with the circles, since no collision with another circle or a plane is able to induce any torque in them.

In order to really test our rotation code we need to introduce Boxes in the next tutorial.

## Next Tutorial:
Rotational Force Part 2 in Rotational Force section
