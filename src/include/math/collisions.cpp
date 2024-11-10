#include "collisions.h"
#include "collisions.h"

bool collides1d(cfp& x0, cfp& w0, cfp& x1, cfp& w1)
{
	return
		((x0 + w0) > x1) &&
		((x1 + w1) > x0); //x
}

//returns wether a rectangle intersects with another rectangle
//doesn't collide when the edges touch each other exactly
bool collides2d(crectangle2& r1, crectangle2& r2)
{
	return r1.pos0.x + r1.size.x > r2.pos0.x && r1.pos0.x < r2.pos0.x + r2.size.x &&//x
		r1.pos0.y + r1.size.y > r2.pos0.y && r1.pos0.y < r2.pos0.y + r2.size.y;//y
}

//returns wether a texture intersects with another texture on alpha
bool collides2d(texture* tex1, mat3x3 transform1, texture* img2, mat3x3 transform2)
{
	//inverse transform of img2
	mat3x3 inverse2 = transform2.inverse();//graphics to image2
	mat3x3 tr1to2 = mat3x3::cross(inverse2, transform1);//image1 to graphics to image2
	cint precision = 5;
	//now check for every 5 pixels
	for (fsize_t j = 0; j < tex1->size.y; j += precision)
	{
		for (fsize_t i = 0; i < tex1->size.x; i += precision)
		{
			color* imgi = tex1->baseArray + i + j * tex1->size.x;
			if (imgi->a()) {//filled at img1
				cvec2 projected = tr1to2.multPointMatrix(vec2(i, j));// projected on texture 2
				if (projected.x >= 0 && projected.y >= 0 && projected.x < img2->size.x && projected.y < img2->size.y &&//pixel in bounds of img2
					(img2->baseArray + (int)projected.x + (int)projected.y * img2->size.x)->a())//filled at img2, collision
				{
					return true;//collided
				}
			}
		}
	}
	return false;
}

//returns wether a complex 2d shape intersects with another complex 2d shape
bool collides2d(const std::vector<vec2>& v1, const std::vector<vec2>& v2, cbool& checkinside)
{
	vec2 lastv1 = *(v1.end() - 1);
	for (cvec2& curv1 : v1)
	{
		bool inside = false;
		vec2 lastv2 = *(v2.end() - 1);
		for (cvec2& curv2 : v2)
		{
			if (collides2d(lastv1, curv1, lastv2, curv2))return true;
			if (pointleftofline(curv1, lastv2, curv2))inside = !inside;
			lastv2 = curv2;
		}
		lastv1 = curv1;
		if (inside)return true;
	}
	return false;
}

bool collides3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1)
{
	fp tmin, tmax;
	return collidedistance3d(p0, p1, boxp0, boxp1, tmin, tmax);
}

bool collidedistance3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1, fp& tmin, fp& tmax)
{
	const vec3 normal = (p1 - p0).normalized();
	const vec3 inversenormal = 1 / normal;
	fp tx0 = (boxp0.x - p0.x) * inversenormal.x;//t when intersecting with the boxp0.x plane
	fp tx1 = (boxp1.x - p0.x) * inversenormal.x;//t when intersecting with the boxp1.x plane

	tmin = math::minimum(tx0, tx1);
	tmax = math::maximum(tx0, tx1);

	fp ty0 = (boxp0.y - p0.y) * inversenormal.y;//t when intersecting with the boxp0.y plane
	fp ty1 = (boxp1.y - p0.y) * inversenormal.y;//t when intersecting with the boxp1.y plane

	tmin = math::maximum(tmin, math::minimum(ty0, ty1));
	tmax = math::minimum(tmax, math::maximum(ty0, ty1));

	fp tz0 = (boxp0.z - p0.z) * inversenormal.z;//t when intersecting with the boxp0.z plane
	fp tz1 = (boxp1.z - p0.z) * inversenormal.z;//t when intersecting with the boxp1.z plane

	tmin = math::maximum(tmin, math::minimum(tz0, tz1));
	tmax = math::minimum(tmax, math::maximum(tz0, tz1));

	//nearest point?
	return tmax >= tmin && tmax >= 0;//tmax >= 0 because else you could hit behind you
}
bool collides2d(cvec2& p0, cvec2& p1, crectangle2& box)
{
	fp tmin, tmax;
	return collidedistance2d(p0, p1, box, tmin, tmax);
}
bool collidedistance2d(cvec2& p0, cvec2& p1, crectangle2& box, fp& tmin, fp& tmax)
{
	return collidedistance2d(p0, p1, box.pos0, box.pos1(), tmin, tmax);
}
bool collidedistance2d(cvec2& p0, cvec2& p1, cvec2& boxp0, cvec2& boxp1, fp& tmin, fp& tmax)
{
	cvec2 difference = p1 - p0;
	bool success = collidedistance2dDirection(p0, difference.normalized(), boxp0, boxp1, tmin, tmax);
	return success && tmin < difference.length();//check if it is past the maximum
}
//HAS TO BE NORMALIZED
bool collidedistance2dDirection(cvec2& p0, cvec2& directionNormal, cvec2& boxp0, cvec2& boxp1, fp& tmin, fp& tmax)
{
	cvec2 inversenormal = 1 / directionNormal;
	fp tx0 = (boxp0.x - p0.x) * inversenormal.x;//t when intersecting with the boxp0.x plane
	fp tx1 = (boxp1.x - p0.x) * inversenormal.x;//t when intersecting with the boxp1.x plane

	tmin = math::minimum(tx0, tx1);
	tmax = math::maximum(tx0, tx1);

	fp ty0 = (boxp0.y - p0.y) * inversenormal.y;//t when intersecting with the boxp0.y plane
	fp ty1 = (boxp1.y - p0.y) * inversenormal.y;//t when intersecting with the boxp1.y plane

	tmin = math::maximum(tmin, math::minimum(ty0, ty1));
	tmax = math::minimum(tmax, math::maximum(ty0, ty1));

	//nearest point?
	return tmax >= tmin && tmax >= 0;//tmax >= 0 because else you could hit behind you
}
bool collidedistance2dDirection(cvec2& p0, cvec2& direction, crectangle2& box, fp& tmin, fp& tmax)
{
	return collidedistance2dDirection(p0, direction, box.pos0, box.pos1(), tmin, tmax);
}
//checks if an endless ray from p to the right intersects with the line
bool pointleftofline(cvec2& point, cvec2& p0, cvec2& p1)
{
	return ((p0.y >= point.y) != (p1.y >= point.y)) &&//check y
		(point.x <= (p1.x - p0.x) * (point.y - p0.y) / (p1.y - p0.y) + p0.x);//check x
}
//https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/
fp collideTime2d(crectangle2& box0, crectangle2& box1, cvec2& box0MoveDistance, vect2<bool>& axisCollided)
{
	vec2 invEntry;
	vec2 invExit;

	vec2 entry = invEntry / box0MoveDistance,
		exit = invExit / box0MoveDistance;

	cvec2 box0Pos11 = box0.pos1();
	cvec2 box1Pos11 = box1.pos1();

	axisCollided = vect2<bool>();
	for (int axisIndex = 0; axisIndex < 2; axisIndex++)
	{
		// find the distance between the objects on the near and far sides for both x and y 
		if (box0MoveDistance.axis[axisIndex] > 0)
		{
			invEntry.axis[axisIndex] = box1.pos0.axis[axisIndex] - box0Pos11.axis[axisIndex];
			invExit.axis[axisIndex] = box1Pos11.axis[axisIndex] - box0.pos0.axis[axisIndex];
		}
		else if (box0MoveDistance.axis[axisIndex] < 0)
		{
			invEntry.axis[axisIndex] = box1Pos11.axis[axisIndex] - box0.pos0.axis[axisIndex];
			invExit.axis[axisIndex] = box1.pos0.axis[axisIndex] - box0Pos11.axis[axisIndex];
		}
		else
		{
			//not moving
		}
		//collides instantly
		if (box0.pos0.axis[axisIndex] < box1Pos11.axis[axisIndex] &&
			box1.pos0.axis[axisIndex] < box0Pos11.axis[axisIndex])
		{
			axisCollided.axis[axisIndex] = true;
			entry.axis[axisIndex] = 0;
		}
		else
		{
			entry.axis[axisIndex] = box0MoveDistance.axis[axisIndex] == 0 ? INFINITY : invEntry.axis[axisIndex] / box0MoveDistance.axis[axisIndex];
		}
		if (box0MoveDistance.axis[axisIndex] == 0)
		{
			exit.axis[axisIndex] = INFINITY;
		}
		else
		{
			exit.axis[axisIndex] = invExit.axis[axisIndex] / box0MoveDistance.axis[axisIndex];
		}
	}
	cfp entryTime = math::maximum(entry.x, entry.y);
	cfp exitTime = math::minimum(exit.x, exit.y);
	if (entryTime > exitTime || entry.x < 0 || entry.y < 0 || entry.x > 1 || entry.y > 1)
	{
		axisCollided = vect2<bool>();
		return 1;
	}
	else
	{
		if (axisCollided.x && axisCollided.y)
		{
			//stuck in hitbox
			return entryTime;
		}
		//the last one collides
		//one of the axes was in the hitbox, but will never be there, because the other axis collided immediately
		else if (entry.x > entry.y ||
			(entry.x == entry.y && axisCollided.y))
		{
			axisCollided.x = true;
			axisCollided.y = false;
		}
		else if (entry.y > entry.x ||
			(entry.x == entry.y && axisCollided.x))
		{
			axisCollided.y = true;
			axisCollided.x = false;
		}
		else
		{
			axisCollided.x = true;
			axisCollided.y = true;
		}
		return entryTime;
	}
}
crectangle2 getBoxContaining(crectangle2& box, cvec2& boxTranslate)
{
	rectangle2 containingBox = box;
	for (int i = 0; i < 2; i++)
	{
		if (boxTranslate.axis[i] < 0)
		{
			containingBox.size.axis[i] -= boxTranslate.axis[i];
			containingBox.pos0.axis[i] += boxTranslate.axis[i];
		}
		else if (boxTranslate.axis[i] > 0)
		{
			containingBox.size.axis[i] += boxTranslate.axis[i];
		}
	}
	return containingBox;
}
//https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm
bool PointInPolygon(cvec2& point, const std::vector<vec2>& points) {
	size_t i, j, nvert = points.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (pointleftofline(point, points[i], points[j]))
			c = !c;
	}

	return c;
}
bool collides2d(cvec2& a0, cvec2& a1, cvec2& b0, cvec2& b1)
{
	vec2 intersection;//will be optimized away in release
	return collides2d(a0, a1, b0, b1, intersection);
}
//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1968345#1968345
// Returns true if the lines intersect, otherwise false. In addition, if the lines 
// intersect the intersection point may be stored in 'intersection'.
bool collides2d(cvec2& a0, cvec2& a1, cvec2& b0, cvec2& b1, vec2& intersection)
{
	cvec2 s0 = vec2(a1 - a0);
	cvec2 s1 = vec2(b1 - b0);

	cfp s = (-s0.y * (a0.x - b0.x) + s0.x * (a0.y - b0.y)) / (-s1.x * s0.y + s0.x * s1.y);
	cfp t = (s1.x * (a0.y - b0.y) - s1.y * (a0.x - b0.x)) / (-s1.x * s0.y + s0.x * s1.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		intersection = a0 + (t * s0);
		return true;
	}

	return false; // No collision
}