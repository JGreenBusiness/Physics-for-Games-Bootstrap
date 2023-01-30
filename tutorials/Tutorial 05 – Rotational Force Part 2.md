# Tutorial – Rotational Force, part 2

## Introduction and Objective:
In the previous tutorial we implemented rotations using circles. But watching a circle rotate isn’t that interesting. 

In this tutorial we will extend our physics engine by a Box class. Because these will be oriented bounding boxes capable of arbitrary rotation, we’ll spend a lot of time focusing on the collision detection algorithms needed for this class.

By the end of this tutorial you will have a physics engine capable of performing simulation using planes, circles and boxes.
 
RotationalForces01.PNG


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

Although recommended, it is not necessary to have completed the following previous tutorials:
- ***Projectile Physics part 1: Analytical Solution. Available in the Projectile Physics session.***
- ***Projectile Physics part 2: Numerical Integration. Available in the Projectile Physics session.***

## The Box Class:
We will implement an OBB (oriented bounding box). This is much like an AABB (axis-aligned bounding box), but with an arbitrary rotation. 

Although the data needed to represent a box object may seem fairly self-explanatory, there are actually some different ways we can store the data that will influence how we implement the collision detection and response.

For the box dimensions, we can store the width and height of the box and use the position to refer to one of the corner points of the box. Alternatively, we could use the position to refer to the centre point and store the halfedge lengths (i.e., half the width and half the height – how far the box is extending in any direction). The implementation in this tutorial stores the halfedge lengths.

As for the rotation, we could either store the rotation in radians, an orientation matrix, or just the two axis of the rotation matrix that we care about (i.e., the x and y axis). The trade-off is between storing more data and doing more calculations when resolving collisions.

When performing an OBB-OBB intersection test, we must convert any rotation stored in radians into a matrix for use in the effective separating axis test, which is a very expensive operation. For this reason we will store two of the rotation matrix axes. (And although we won’t need to for 2D, we could always compute the third from a cross product of the other two at test time). This is a good compromise between the amount of data stored and CPU operations.

Define a Box class derived from Rigidbody with the following member variable

``` c++
glm::vec2 m_extents;			// the halfedge lengths 
```

And add these local axes to the Rigidbody class. These are the x and y axes during the physics frame
``` c++
// store the local x,y axes of the box based on its angle of rotation
glm::vec2 m_localX;
glm::vec2 m_localY;
void CalculateAxes();
```

The m_localX and m_localY are the two axes from the rotation matrix calculated from the Rigidbody’s m_orientation variable. Note we are still using m_orientation. It will be updated each frame and modified according to the angular velocity. We calculate them like this:
``` c++
void Rigidbody::CalculateAxes()
{
	float sn = sinf(m_orientation);
	float cs = cosf(m_orientation);
	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}
```

Call this function in Rigidbody::FixedUpdate at the very top.

Base the constructor for Box on the Circle constructor, but pass the extents through rather than the radius.

The Draw() functions for the box look as follows:
``` c++
void Box::Draw(float alpha) 
{
    CalculateSmoothedPosition(alpha);
	// draw using local axes
    glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extents.x 
-	m_smoothedLocalY * m_extents.y;
	glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extents.x 
-	m_smoothedLocalY * m_extents.y;
	glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
                                     + m_smoothedLocalY * m_extents.y;
	glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extents.x 
                                     + m_smoothedLocalY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);
}
```

If you are storing the full width and height of the box (as opposed to the halfedge lengths) then adjust the m_extents values accordingly.

We need to update CalculateSmoothedPosition() to calculate the smoothed axes from the current and previous orientation:
``` c++
void Rigidbody::CalculateSmoothedPosition(float alpha)
{
	m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

	float smoothedOrientation = alpha * m_orientation + (1 - alpha) * m_lastOrientation;

	float cs = cosf(smoothedOrientation);
	float sn = sinf(smoothedOrientation);

	m_smoothedLocalX = glm::vec2( cs, sn);
	m_smoothedLocalY = glm::vec2(-sn, cs);
}
```
---
## Box-to-Plane Collisions:
For a box-plane collision, we want to check the four corners of the box and look at their distance to the plane. If their sign is opposite to the velocity of the box along the normal of the plane, we have penetrated it and need to resolve the collision.

Using m_localX and m_localY as calculated during the fixedUpdate() function, we check the four corners. At each corner, we calculate the velocity of that point, based on the sum of the object’s linear velocity and the velocity of that point due to rotation.

For each corner that makes a contact, we store its position and, add to the total number of contacts. At the end of the loop we find an average point of contact to apply the force to. (In general, only one, contact will occur when the box hits a plane on its corner. Two contacts occur if a face of the box hits the plane side on.)

We can then use the same ResolveCollision function we used before to reverse the velocity of the box at the point of contact.

``` c++
bool PhysicsScene::Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	//if we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		// Get a representative point on the plane
		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

		// check all four corners to see if we've hit the plane
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				// Get the position of the corner in world space
				glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

				// this is the total velocity of the point in world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
				// and this is the component of the point velocity into the plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

				// and moving further in, we need to resolve the collision
				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		// we've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}
```

GetWidth() and GetHeight() above are public accessors for getting the full width and height of the box ie twice the extents.

---

## Updating Plane-to-Rigidbody collision resolution
When a rotating rigidbody hits a plane, the velocity at the point of impact is a combination of the rigidbody’s linear velocity and the velocity of its point relative to its center caused by its rotation. 

As with the purely linear case, we want to apply a force to reverse the component of velocity into the plane.

Application of a force at the point of contact will now have two consequences. It will change the linear velocity of the whole rigidbody, but also change the angular velocity of the rigidbody through the application of torque if the contact point is off-center. The code should look like this.


``` c++
void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	// the position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = contact - actor2->GetPosition();

	// the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
	glm::vec2 vRel = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_normal);

	// perfectly elasticity collisions for now
	float e = 1;

	// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	// work out the "effective mass" - this is a combination of moment of
	// inertia and mass, and tells us how much the contact point velocity 
	// will change with the force we're applying
	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());

	float j = -(1 + e) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	float kePre = actor2->GetKineticEnergy();

	actor2->applyForce(force, contact - actor2->GetPosition());

	float kePost = actor2->GetKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}
```

Consider a long thin rectangle being struck at a point along it’s long face. If it’s struck in the center (r=0) then the point of contact will start to move away linearly with the whole object. If it’s struck near one end, then the point of contact moves away both from linear acceleration and the torque induced by the off-center force. Less force is required to push the point of contact away, almost as if the object has less mass!

The “effective mass” of the object based on its contact position is calculated above as

M0 = 1/ (1/M + r^2/I)

We can see that if r=0, this reduces down to M0 = M. If r is non-zero, either positive or negative, then the effective mass reduces, because the contact point will rotate away with an application of force at that point.

When we collide a Circle with a Plane, r will always be zero, and no torque will be induced. If we drop a Box on to a plane so that it hits with two contact points, then the average contact point is the centre of the colliding face, and no torque is induced.

If we drop a box that starts off at an angle (not 45 degrees on a square!), so that it collides with a horizontal plane by one point, then torque will be induced.

## Activity 1: Falling on a Plane

Test that the following Rigidbody to Plane collisions do the following:
- A circle does not gain any angular velocity when dropped on a plane.
- A box aligned with world axes (m_orientation =0) dropped on to a horizontal plane does not gain any angular velocity.
- A box dropped at an angle does gain angular velocity. The more it spins after each bounce, the lower its bounce will be. If it hits the plane with a large value of r, then a smaller force will be applied due to the lower “effective mass”, giving rise to less linear acceleration. This also makes sense in terms of energy, because the total kinetic energy is being divided between linear kinetic energy and rotational kinetic energy. The split is different with each bounce.
- The energy diagnostics do not show a significant change in the total energy in the system when the off-centre box hits the plane.

It may be useful at this point to start adding various “scene” functions to your Application class, rather than changing out the code in startup(). Give them descriptive names like bouncyBall(), boxTest() and so on to make it easier to keep testing your physics engine as you add more features.

---

## Box-to-Circle Collisions:
How do we determine when a circle and a box have collided? 

We need to find the closest point to the circle center that is on or inside the box. If we convert the circle’s position to the coordinate space of the box, then we just need to clamp the circle’s position to the box’s extents in both x and y, and convert back into world coordinates.

If the closest point is a corner, then we’re clamping both coordinates. If it’s on an edge, we only clamp one of the coordinates.

``` c++
bool PhysicsScene::Box2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Circle* circle = dynamic_cast<Circle*>(obj2);

	if (box != nullptr && circle != nullptr) 
	{
		// transform the circle into the box's coordinate space
		glm::vec2 circlePosWorld = circle->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

		// find the closest point to the circle centre on the box by clamping the coordinates in box-space to the box's extents
		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();
		if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
		if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
		if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
		if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;
		// and convert back into world coordinates
		glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x * box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
		glm::vec2 circleToBox = circle->GetPosition() - closestPointOnBoxWorld;
        if (glm::length(circleToBox)< circle->GetRadius())
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(circle, contact, &direction);
		}
	}

	return false;
}
```
To determine if a collision has occurred, we just compared the distance from the circle center to this closest point with the circle’s radius. If it’s smaller, then they have collided.

---

## Box-to-Box Collisions:
Box to box collision is determined as a special case of Separating Axes theorem. Each box has two axes - localX and localY.  The projection of the other box on that axis is found, and we find the case where the overlap is smallest, and resolve the collision using that normal and penetration.

We do this by transforming the corners of Box B into the space of Box A, and vice versa. The following function does just this, and updates a sum total of contact points in world space, the number of contacts. It returns the normal to the edge which has just been penetrated by a corner.

``` c++
// check if any of the other box's corners are inside this box
bool Box::CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float &pen, glm::vec2& edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = box.GetExtents().x * 2;
	float boxH = box.GetExtents().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	// loop over all corners of the other box
	for (float x = -box.GetExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.GetExtents().y; y < boxH; y += boxH)
		{
			// Get the position in worldspace
			glm::vec2 p = box.GetPosition() + x * box.m_localX + y * box.m_localY;
			// Get the position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX),
				glm::dot(p - m_position, m_localY));

			// update the extents in each cardinal direction in our box's space
			// (ie extents along the separating axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// if this corner is inside the box, add it to the list of contact points
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}

	// if we lie entirely to one side of the box along one axis, we've found a separating
	// axis, and we can exit
	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.y || minY >= m_extents.y)
		return false;
    if (numLocalContacts == 0)
		return false;

	bool res = false;
	contact += m_position + (localContact.x*m_localX + localContact.y*m_localY) /
		(float)numLocalContacts;
	numContacts++;

	// find the minimum penetration vector as a penetration amount and normal
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}
	return res;
}
```
To check a Box-Box collision, we apply this to both boxes against each other, and call our ResolveCollision() function as before.

We’re looking here for the minimum penetration vector as defined in Separating Axis Theorem.

The parameters open and norm are passed as references, and so may be modified in the first call to CheckBoxCorners for comparison against any values we generate in the second call. Note also that if the second call finds the smallest penetration value, we flip the normal, so that the normal is always directed from box1 to box2.

Once we’ve found a collision normal and contact point we can use the same Rigidbody::ResolveCollision function that we’ve used for Circle2Circle and Circle2Box

``` c++
bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	if (box1 != nullptr && box2 != nullptr) {
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm)) {
			norm = -norm;
		}
		if (pen > 0) {
			box1->ResolveCollision(box2, contact / float(numContacts), &norm);
		}
		return true;
	}
	return false;
}
```

After you have finished integrating the Box class and the associated collision response algorithm into your program, create a few of each kind of shape to test the engine’s behaviour.

You may find it useful to test each shape-pair collision independently (either once you complete all functions, or after you complete each individual function) so that any problems are easier to identify and debug.

Make sure that your PhysicsScene table of cuntion pointers has been updated to include all the box variants. (And Circle2Box calls Box2Circle with reversed parameter order, and so on.)


``` c++
static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, 	 PhysicsScene::Plane2Circle,  PhysicsScene::Plane2Box,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Box,
	PhysicsScene::Box2Plane,    PhysicsScene::Box2Circle,    PhysicsScene::Box2Box,
};
```

---

## Rotational Kinetic Energy
Now that a rigidbody can rotate, it can have zero linear velocity and still have kinetic energy from its rotation.

You can calculate update the kinetic energy calculation like this:

``` c++
float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass*glm::dot(m_velocity, m_velocity) + 
m_moment * m_angularVelocity * m_angularVelocity);
}
```

As you test your various shape collision, make sure to incorporate this and check that the total kinetic energy before and after collisions is being preserved.

When this is done, you have a full set of working primitives that can perform perfect elastic collisions with each other.

Note that we haven’t yet discussed contract forces and penetration. 

If two Rigidbodys are overlapping, but have no velocity (say they’ve been initialized this way), then our force-based collision detection and resolution can do nothing to separate them. Our engine can also sometimes behave strangely if one object is penetrating another over multiple calls to fixedUpdate(). 

We can fix this problem by applying “contact forces” that directly move the objects so that they don’t overlap, and do this along with the application of forces. This will be the topic of a future tutorial.

---

## Next Tutorial:
Collision Resolution – Static and Dynamic Friction in Collision Response section

