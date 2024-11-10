#include "application/initialize.h"
#include "application/control/form/form.h"
#include "include/application/application.h"
#include "gameInfo.h"
#include "math/random/random.h"
#include "math/sphere/sphere.h"
#include "math/sphere/sphereCollisions.h"
#include "math/graphics/brush/brushes.h"

struct gameForm : public form
{
	static constexpr int sphereCount = 0x10;
	sphere spheres[sphereCount]{};
	vec3 cameraPosition = vec3();
	vec3 cameraRight = vec3(1, 0, 0);
	vec3 cameraUp = vec3(0, 1, 0);
	vec3 cameraForward = vec3(0, 0, 1);
	veci3 movementDirections = veci3();
	static constexpr vk movementKeys[6] = {vk::A, vk::D, vk::Q, vk::E, vk::S, vk::W};
	veci2 rectCenter;
	gameForm() : form()
	{
		std::mt19937 gen{};
		std::uniform_real_distribution<fp> positionDistribution(-10, 10);
		std::uniform_real_distribution<fp> sizeDistribution(0, 1);
		for (int i = 0; i < sphereCount; i++)
		{
			spheres[i] = sphere(vec3(positionDistribution(gen), positionDistribution(gen), positionDistribution(gen)), sizeDistribution(gen));
		}
	}
	virtual void keyDown(cvk &keyCode) override
	{
		for (fsize_t dir = 0; dir < directionCount3D; dir++)
		{
			if (keyCode == movementKeys[dir])
				movementDirections[(fsize_t)getAxis((directionID)dir)] += getSign((directionID)dir);
		}
	}
	virtual void keyUp(cvk &keyCode) override
	{
		for (fsize_t dir = 0; dir < directionCount3D; dir++)
		{
			if (keyCode == movementKeys[dir])
				movementDirections[(fsize_t)getAxis((directionID)dir)] -= getSign((directionID)dir);
		}
	}

	// direction should be normalized!
	constexpr fp traceRay(vec3 &rayOrigin, vec3 &rayDirection, cfp &maxDistance, cint &maxIntersectionCount) const
	{
		fp distanceTraveled = 0;
		for (int intersectionCount = 0; intersectionCount < maxIntersectionCount; intersectionCount++)
		{

			fp closestSphereCollision = INFINITY;
			sphere closestSphere;

			// now go over all spheres and calculate intersections
			for (const sphere &s : spheres)
			{
				fp sphereCollisionStart, sphereCollisionEnd;
				if (collideraysphere(rayOrigin, rayDirection, s, sphereCollisionStart, sphereCollisionEnd) &&
					sphereCollisionStart < closestSphereCollision && sphereCollisionStart > 0 && ((distanceTraveled + sphereCollisionStart) < maxDistance))
				{
					closestSphereCollision = sphereCollisionStart;
					closestSphere = s;
				}
			}
			if (closestSphereCollision == INFINITY)
			{
				break;
			}
			rayOrigin += rayDirection * closestSphereCollision;
			// bounce against sphere
			cvec3 &surfaceNormal = (rayOrigin - closestSphere.center).normalized();
			rayDirection = reflect(rayDirection, surfaceNormal);
			distanceTraveled += closestSphereCollision;
		}
		return distanceTraveled;
	}

	void render_part(rectanglei2 pixelRect, texture **rayTracingTexture) const
	{
		*rayTracingTexture = new texture(pixelRect.size, false);
		auto pixelPosIterator = pixelRect.begin();
		for (color &ptr : **rayTracingTexture)
		{
			cveci2 &pixelPos = *pixelPosIterator;
			pixelPosIterator++;

			// ranging from -1 to 1
			cvec2 &unitPixelVector = (pixelPos - rectCenter) / (fp)rectCenter.x;
			vec3 rayDirection = (cameraForward + cameraRight * unitPixelVector.x + cameraUp * unitPixelVector.y).normalized();
			vec3 rayPosition = cameraPosition;

			constexpr int maxIntersectionCount = 0x10;
			constexpr fp maxCollisionDistance = 0x100;

			cfp &distanceTraveled = traceRay(rayPosition, rayDirection, maxCollisionDistance, maxIntersectionCount);

			// constexpr color sphereColor = colorPalette::red;
			vec3 fColorValues = ((rayDirection + vec3(1)) * 127.5);
			const color airColor = color((byte)fColorValues.x, (byte)fColorValues.y, (byte)fColorValues.z);
			constexpr color backGroundColor = color(0x80, 0x80, 0xff);
			if (distanceTraveled < maxCollisionDistance)
			{
				ptr = airColor; // lerpColor(sphereColor, backGroundColor, 1.0 - (distanceTraveled / maxCollisionDistance));
			}
			else
			{
				ptr = backGroundColor;
			}
		}
	}

	virtual void render(cveci2 &position, const texture &renderTarget) override
	{
		renderTarget.fill(colorPalette::green);
		// lets build a ray tracer.
		// for each pixel, shoot a ray from the camera.

		constexpr int pixelScale = 2;
		// texture rayTracingTexture = texture(renderTarget.size / pixelScale, false);

		// downscaled rectangle
		crectanglei2 &pixelRect = rectanglei2(veci2(), ceilVector((vec2)renderTarget.getClientRect().size / pixelScale));
		rectCenter = pixelRect.getCenter();

		// move the camera the way we move the rays too

		// cameraPosition += movementDirections * 0.1;
		cfp &speed = 0.1;
		if (movementDirections.lengthSquared() > 0)
		{
			cvec3 &movementRayDirection = (cameraRight * movementDirections.x + cameraUp * movementDirections.y + cameraForward * movementDirections.z).normalized();
			vec3 finalDirection = movementRayDirection;

			vec3 finalCameraPosition = cameraPosition;

			cfp &distanceTraveled = traceRay(finalCameraPosition, finalDirection, speed, 0x10);
			finalCameraPosition += (speed - distanceTraveled) * finalDirection;
			// to obtain up and right, cast a ray from a point with a small offset to up and right
			constexpr fp offset = 0.01f;

			for (vec3 *direction : {&cameraForward, &cameraRight, &cameraUp})
			{
				vec3 offsetPos = cameraPosition + *direction * offset;
				vec3 finalDirection = movementRayDirection;
				cfp &distanceTraveled = traceRay(offsetPos, finalDirection, speed, 0x10);
				offsetPos += (speed - distanceTraveled) * finalDirection;
				*direction = (offsetPos - finalCameraPosition).normalized();
			}
			// now correct the vectors, so they'll keep being straight
			// they should be like axes: all vectors should have a 90 degrees angle between each other
			cvec3 &newRight = vec3::cross(cameraForward, cameraUp);
			cameraRight = (vec3::dot(cameraRight, newRight) > 0) ? // facing the same way
							  newRight
																 : -newRight;
			cvec3 &newUp = vec3::cross(cameraRight, cameraForward);
			cameraUp = (vec3::dot(cameraUp, newUp) > 0) ? newUp : -newUp;

			// cameraUp = vec3::cross(cameraForward, cameraRight);
			cameraPosition = finalCameraPosition;
		}

		constexpr int tileSize = 0x200;
		cveci2 &tileCount = ceilVector((vec2)pixelRect.size / tileSize);
		texture **results = new texture *[tileCount.volume()];
		std::thread *threads = new std::thread[tileCount.volume()];
		auto *resultPtr = &results[0];
		auto *threadPtr = &threads[0];
		for (int renderY = 0; renderY < tileCount.y; renderY++)
		{
			for (int renderX = 0; renderX < tileCount.x; renderX++, resultPtr++, threadPtr++)
			{
				rectanglei2 tileRect = pixelRect.cropClientRectUnsafe(rectanglei2(renderX * tileSize, renderY * tileSize, tileSize, tileSize));
				*threadPtr = std::thread(&gameForm::render_part, this, tileRect, resultPtr);
			}
		}
		crectanglei2 &screenRect = renderTarget.getClientRect();
		constexpr int tileSizePrecision = tileSize * pixelScale;
		resultPtr = results;
		threadPtr = threads;
		for (int renderY = 0; renderY < tileCount.y; renderY++)
		{
			for (int renderX = 0; renderX < tileCount.x; renderX++)
			{
				threadPtr->join();
				texture *result = *resultPtr++;
				crectangle2 &tileScreenrect = screenRect.cropClientRectUnsafe(rectanglei2(renderX * tileSizePrecision, renderY * tileSizePrecision, tileSizePrecision, tileSizePrecision));
				crectangle2 &sourceTileRect = rectangle2(vec2(), tileScreenrect.size / pixelScale);
				fillTransformedBrushRectangle(sourceTileRect, tileScreenrect, *result, renderTarget);
				delete result;
				threadPtr++;
			}
		}
		delete[] results;
		delete[] threads;

		// fillTransformedTexture(rayTracingTexture.getClientRect().multiplied(pixelScale), rayTracingTexture, renderTarget);
	}
};
gameForm *mainForm = new gameForm();
int main(int argc, char *argv[])
{
	// execute this function before you do anything,
	initialize();
	return application(mainForm, gameName).run();
}