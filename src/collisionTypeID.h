#pragma once
enum class collisionTypeID :int
{
	willNotCollide,
	//entities collide only if they fall on top of this block
	willCollideTop,
	//full collision
	willCollide,
	count
};