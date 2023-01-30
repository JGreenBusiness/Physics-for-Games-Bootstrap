# Tutorial – Collision Response: Static and Dynamic Friction

## Introduction and Objective:
In our current program, if we turn off gravity and set our rigidbody objects in motion by applying a force, the objects will continue to move forever. We can solve this problem by applying some friction. 
In this tutorial we’ll apply drag, a type of friction, to the movement of our objects in our simulation.

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

Although recommended, it is not necessary to have completed the following previous tutorials:
- ***Projectile Physics part 1: Analytical Solution. Available in the Projectile Physics session.***
- ***Projectile Physics part 2: Numerical Integration. Available in the Projectile Physics session.***

# Adding Drag:
The complexity involved in implementing static and dynamic friction in our custom 2D physics engine is arguably not worth the benefit we would receive. So instead, we will implement linear and angular drag to the movement of our rigidbody objects.

Drag is the friction between a solid and a liquid or gas. Drag is sometimes called air resistance (or fluid resistance for liquids).
We will implement this by defining two variables: one for linear drag, and one for angular (or rotational) drag. This will give us some more control when specifying how our objects should behave, and is akin to implementations in commercial game engines (you may have noticed that Unity3D also allows you to define the linear and angular drag of rigidbody objects).

Create two new variables in your Rigidbody class:

``` c++
class Rigidbody : public PhysicsObject
{
   ...

protected:
   ...

   float m_linearDrag;
   float m_angularDrag;
```

You can create getters/setters to modify these values, or update your constructors to take new arguments if you wish.
I’ve found that a good default value for these drag variables is around 0.3f, but you may wish to experiment to find values that work for you. The range will be between 0 and 1.

We want to apply these drag variables to our calculations for velocity and angular velocity during the Rigidbody’s *FixedUpdate()* function. 
We will multiply the velocity by the drag value so that velocity will decrease slightly each frame. After time, the velocity should become so small that the object has effectively stopped.

Update the velocity calculations in your Rigidbody *FixedUpdate()* function:

``` c++
m_velocity -= m_velocity * m_linearDrag * timeStep;
m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
```

It helps with stability to have a threshold value below which the objects stop moving and rotating. The values for these thresholds are dependent on several factors but we will use 0.01 for angular velocity and 0.1 for linear velocity. The actual values are somewhat arbitrary and really come down to what feels right for a particular simulation - most physics engines allow user to tweak these values.

Add the following checks to your fixedUpdate() function:

``` c++
if (length(m_velocity) < MIN_LINEAR_THRESHOLD) {
	m_velocity = glm::vec2(0, 0);
}
if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) {
	m_angularVelocity = 0;
}
```

MIN_LINEAR_THRESHOLD and MIN_ANGULAR_THRESHOLD can either be #define values, global constant variables, or constant static member variables of the Rigidbody class.

Test your application. You should find that your objects now slow down and stop, rather than maintaining their velocity indefinitely. 

Test different values for the linear drag variable, or specify different drag values for different objects.

# Restitution:
When our objects collide with each other our collision routine currently assumes that 100% of the energy produced by the collision is transformed back into kinetic energy. In other words, the total amount of kinetic energy in the system is the same before and after the collision. 

This is known as 100% elastic collision. In the real world, some energy is always lost in a collision (for example as sound or as heat). 
For our simulation to be accurate we need to model restitution. We do this by removing a fraction of the energy transferred in the collision from the system. 

First, we need a way to specify how much elasticity our objects have. Add a variable to your PhysicsObject and call it m_elasticity. (Elasticity makes sense for Planes as well as Rigidbodies)

A value of 0 will mean that the object loses all of its energy in a collision, like a ball of putty for example. A value of 1 means that no energy is lost, like a rubber super-ball. 

In the constructor, set this value to a default of 1. Billiard balls have a fairly high elasticity so when you add them to the scene set their elasticity to about 0.8. If you’ve added cushions around the table, these will be a much less bouncy so set their elasticity to about 0.6 (you can experiment with other values later.

Next, modify the Rigidbody::ResolveCollision() function so that it uses the m_elasticity member variable from both objects to calculate an elasticity value to use in the equation to calculate the impulse magnitude (j – refer to the slides for the previous Collision Resolution lectures if you need a refresher for the equation used in this function). This will allow our collision handling code to take into account the elasticity of the objects involved in the collision.

The following code segment shows the change to make to the ResolveCollision() function so that the elasticity variable used in the formula is calculated from the properties of the two colliding objects:

``` c++
void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
    // find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->GetPosition() - GetPosition());
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

    ...
    ...

	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / 
    glm::dot(normal, normal * ((1/m_mass) + (1/actor2->GetMass())));

	glm::vec2 force = normal * j;
	ApplyForceToActor(actor2, force, contact);
}
```

As you can see, to combine the elasticity of the two objects involved in the collision we are simply taking the average of the two objects’ elasticity values. For example, if two billiard balls collide we'd expect the combined elasticity to be the same as either ball because they are the same, but what do we do if a ball hits the cushion? 

There are several different ways to calculate this and it comes down to what feels right in the simulation. In this case we are simply taking the average but you can experiment with different algorithms, for example maybe take the smallest elasticity or maybe the largest. 
It's probably better to create a separate function for calculating elasticity so you can apply it to all collisions. You may want to define different algorithms that combine the elasticity in different ways, and allow the user to select which to apply for different objects (as Unity3D does with its physics materials).

You should experiment with different values for the restitution of your objects in the simulation so that your billiards game feels more realistic.

You’ll also want to take into account the elasticity in Plane::ResolveCollision. 


``` c++
void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	// the position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = contact - actor2->GetPosition();

	// the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
	glm::vec2 vRel = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_normal);

	// average the elasticity of the plane and the rigidbody
	float e = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

```

Test the restitution code in a few different situations.

## Activity 1: Bounce To A Stop
Set the elasticity to low values like 0.3f, and watch falling objects (boxes and circles) settle onto a horizontal plane. 

## Activity 2: Set Dressing a Pool Table
In a zero gravity situation, add linear and angular drag and simulate pool balls on a table, where the initial velocity dies down quickly. Implement the values mentioned above.
- Encapsulate the pool balls within 4 planes at the edge of the window.
- Add in a cue ball that is slightly smaller and lighter then the 10 Billiard balls, which are place in a triangle formation.
- On a press of the space bar, shoot the Cue Ball into the Billiards balls.
### Extension
- Have the Cue Ball rotate from it's starting position before shooting in the desired direction using the arrow keys.
- Use a Gizmo line to indicate the direction the force will be applied in.


## Next Tutorial:
Contact Forces in Rotational Force section
