#include "math/rectangle/rectangletn.h"
#include "graphics/texture.h"
#include "graphics/brush/brushes.h"
#pragma once
struct bodyPart2D : IDestructable
{
	rectangle2 textureRect = rectangle2();
	vec2 size = vec2();//the size of this body part
	//the position of the joint relative to the rectangular box. mid = vec2(0.5) * scale
	vec2 rotationCentre = vec2();
	int textureAngle = 0;

	fp angle = 0;
	//the translation between the joint of the parent and the current joint
	vec2 translate = vec2();
	bool flipX = false;
	bool flipY = false;

	mat3x3 applied = mat3x3();
	mat3x3 scalecentre = mat3x3();//transformation from texture to rectangle in 2d space
	bool changed = false;
	bodyPart2D* parent = nullptr;
	//drawn from first to last (bottom to top)
	std::vector<bodyPart2D*> children = std::vector<bodyPart2D*>();
	size_t drawOrderIndex = 0;//the z order

	bool hasTransparency = false;

	//FIRST BODY PART ANGLE, THEN TEXTURE ANGLE
	//texture angle to rotate from texture to bodypart in degrees
	bodyPart2D(crectangle2& textureRect, bodyPart2D* const& parent = nullptr, cvec2& translate = vec2(), cvec2& size = vec2(1), cvec2& rotationcentre = vec2(0.5), cfp& bodyPartAngle = 0, cint& textureAngle = 0, cbool& hasTransparency = false);

	bodyPart2D() {}

	void CalculateTransform();
	void CalculateAllTransforms();

	//returns the cumulative transform that will be applied to all children
	mat3x3 getCumulativeTransform();
	//returns the cumulative transform of the parent if it exists
	mat3x3 getCumulativeParentTransform() const;
	//returns the cumulative transform of the texture
	mat3x3 getCumulativeTextureTransform();
	vec2 getRotationCentrePosition() const;
	virtual ~bodyPart2D() override;

	template<typename brush0Type>
	inline void renderRecursive(const texture& renderTarget, mat3x3 transform, const brush0Type& b)
	{
		if (changed)
		{
			CalculateTransform();
		}
		cmat3x3& view = mat3x3::cross(transform, applied);
		for (size_t i = 0; i < children.size(); i++)
		{
			if (i == drawOrderIndex)
			{
				//transformation order
				//scalecentre, applied, transform
				if (hasTransparency)
				{
					fillTransparentRectangle((crectangle2)textureRect, mat3x3::cross(view, scalecentre), b, renderTarget);
				}
				else
				{
					fillTransformedBrushRectangle((crectangle2)textureRect, mat3x3::cross(view, scalecentre), b, renderTarget);
				}
			}
			children[i]->renderRecursive(renderTarget, view, b);
		}
		if (drawOrderIndex == children.size())
		{
			if (hasTransparency)
			{
				fillTransparentRectangle((crectangle2)textureRect, mat3x3::cross(view, scalecentre), b, renderTarget);
			}
			else
			{
				fillTransformedBrushRectangle((crectangle2)textureRect, mat3x3::cross(view, scalecentre), b, renderTarget);
			}
		}
	}
};