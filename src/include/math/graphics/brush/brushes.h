#pragma once
#include "math/graphics/texture.h"
#include "transformbrush.h"
#include "math/graphics/resolutiontexture.h"
#include "math/graphics/color/color.h"
#include "math/graphics/graphicsFunctions.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"



//constexpr bool testB = is_brush_v<solidColorBrush>;



//template<typename inputType>
//struct repeatingBrush final : public brush<color, inputType>
//{
//	//repeats the same brush
//	const brush<color, inputType>& brushToRepeat = nullptr;
//	const inputType& repeatSize = inputType();
//	repeatingBrush(const brush0Type& brushToRepeat, const inputType& repeatSize) :brushToRepeat(brushToRepeat), repeatSize(repeatSize) {}
//	repeatingBrush(const brush0Type& textureToRepeat) :brushToRepeat(textureToRepeat), repeatSize(textureToRepeat.getClientRect().size) {}
//	inline color getValue(const inputType& pos) const
//	{
//		if constexpr (std::is_same_v<inputType, vec2>) {
//			const inputType& remainderPos = vec2(math::mod(pos.x, repeatSize.x), math::mod(pos.y, repeatSize.y));
//			return brushToRepeat.getValue(remainderPos);
//		}
//		else {
//			const inputType& remainderPos = inputType(pos.x % repeatSize.x, pos.y % repeatSize.y);
//			return brushToRepeat.getValue(remainderPos);
//		}
//	}
//};

template<typename brush0Type, typename targetType>
inline void fillTransformedBrushRectangle(crectangle2& brushRect, cmat3x3& transform, const brush0Type& b, const targetType& renderTarget)
{
	//+0.5, * transform, -0.5

	//vec2 testSize = vec2(1, 0);
	//if (result.x < 0) {
	//	roundUp[0] = true;
	//}
	//vec2 testSizeY = vec2(1, 0);
	//vec2 result = transform.multSizeMatrix(testSize);
	//if (result.x < 0) {
	//	roundUp[0] = true;
	//}

	const transformBrush<brush0Type>& brush = transformBrush<brush0Type>(transform.inverse(), b);
	fillTransformedRectangle(renderTarget, brushRect, transform, brush);
}

template<typename targetType, typename = std::enable_if_t<!std::is_same_v<targetType, resolutionTexture>>>
inline void fillTransformedBrushRectangle(rectangle2 brushRect, mat3x3 transform, const resolutionTexture& b, const targetType& renderTarget)
{
	const texture& tex = b.mipmap(transform, brushRect);
	fillTransformedBrushRectangle(brushRect, transform, tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline static void fillTransparentRectangle(crectangle2& brushRect, cmat3x3& transform, const brush0Type& b, const targetType& renderTarget)
{
	const auto& matrixBrush = transformBrush<brush0Type>(transform.inverse(), b);
	const auto& transparencyBrush = colorMixer<transformBrush<brush0Type>, targetType>(matrixBrush, renderTarget);
	fillTransformedRectangle(renderTarget, brushRect, transform, transparencyBrush);
}

template<typename targetType, typename = std::enable_if_t<!std::is_same_v<targetType, resolutionTexture>>>
inline void fillTransparentRectangle(rectangle2 brushRect, mat3x3 transform, const resolutionTexture& b, const targetType& renderTarget)
{
	const texture& tex = b.mipmap(transform, brushRect);
	fillTransparentRectangle(brushRect, transform, tex, renderTarget);
}



template<typename brush0Type, typename targetType>
inline void fillTransformedBrushRectangle(crectangle2& brushRect, cvec2& drawOffset, const brush0Type& b, const targetType& renderTarget)
{
	fillTransformedBrushRectangle(brushRect, crectangle2(drawOffset, brushRect.size), b, renderTarget);
}

template<typename brush0Type, typename targetType>
inline void fillTransformedBrushRectangle(crectangle2& brushRect, crectangle2& rectangleToTransform, cmat3x3& transform, const brush0Type& b, const targetType& renderTarget)
{
	//from brush rect to rectangleToTransform to transformed rectangle
	fillTransformedBrushRectangle(brushRect, mat3x3::cross(transform, mat3x3::fromRectToRect(brushRect, rectangleToTransform)), b, renderTarget);
}


template<typename brush0Type, typename targetType>
inline void fillTransformedBrushRectangle(crectangle2& brushRect, crectangle2& drawRect, const brush0Type& b, const targetType& renderTarget)
{
	fillTransformedBrushRectangle(brushRect, mat3x3::fromRectToRect(brushRect, drawRect), b, renderTarget);
}

template<typename brush0Type, typename targetType>
inline void fillTransformedTexture(mat3x3 transform, const brush0Type& tex, const targetType& renderTarget)
{
	fillTransformedBrushRectangle(crectangle2(tex.getClientRect()), transform, tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline void fillTransformedTexture(crectangle2& rect, const brush0Type& tex, const targetType& renderTarget)
{
	fillTransformedBrushRectangle(crectangle2(tex.getClientRect()), rect, tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline void fillTransformedTexture(cvec2& pos, const brush0Type& tex, const targetType& renderTarget)
{
	fillTransformedBrushRectangle(crectangle2(tex.getClientRect()), mat3x3::translate(pos), tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline void fillTransformedTexture(const brush0Type& tex, const targetType& renderTarget)
{
	fillTransformedBrushRectangle(crectangle2(tex.getClientRect()), mat3x3(), tex, renderTarget);
}


template<typename brush0Type, typename targetType>
inline static void fillTransparentRectangle(crectangle2& brushRect, crectangle2& drawRect, const brush0Type& b, const targetType& renderTarget)
{
	fillTransparentRectangle(brushRect, mat3x3::fromRectToRect(brushRect, drawRect), b, renderTarget);
}

template<typename brush0Type, typename targetType>
inline static void fillTransparentTexture(crectangle2& drawRect, const brush0Type& tex, const targetType& renderTarget)
{
	fillTransparentRectangle(crectangle2(tex.getClientRect()), drawRect, tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline static void fillTransparentTexture(cmat3x3& transform, const brush0Type& tex, const targetType& renderTarget)
{
	fillTransparentRectangle(crectangle2(tex.getClientRect()), transform, tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline static void fillTransparentTexture(crectangle2& textureRect, cvec2& drawPosition, const brush0Type& tex, const targetType& renderTarget)
{
	fillTransparentRectangle(crectangle2(textureRect), crectangle2(drawPosition, textureRect.size), tex, renderTarget);
}

template<typename brush0Type, typename targetType>
inline static void fillTransparentRectangle(crectangle2& brushRect, cint& rotation, crectangle2& drawRect, const brush0Type& b, const targetType& renderTarget)
{
	fillTransparentRectangle(brushRect, mat3x3::fromRectToRotatedRect(brushRect, rotation, drawRect), b, renderTarget);
}
template<typename brush0Type, typename targetType>
inline static void fillTransparentRectangle(crectangle2& drawRect, const brush0Type& b, const targetType& renderTarget)
{
	fillRectangle(renderTarget, drawRect, colorMixer<brush0Type, targetType>(b, renderTarget));
}

template<typename targetType>
inline static void fillTransparentRectangle(crectangle2& drawRect, const color& c, const targetType& renderTarget)
{
	fillTransparentRectangle(drawRect, solidColorBrush(c), renderTarget);
}
template<typename brush0Type, typename targetType>
inline static void fillTransparentRectangle(crectangle2& brushRect, crectanglei2& drawRect, const brush0Type& b, const targetType& renderTarget)
{
	fillTransparentRectangle(brushRect, crectangle2(drawRect), b, renderTarget);
}
template<typename brush0Type, typename targetType>
inline static void fillTransparentRectangle(crectangle2& brushRect, crectangle2& rectangleToTransform, cmat3x3& transform, const brush0Type& b, const targetType& renderTarget)
{
	//from brush rect to rectangleToTransform to transformed rectangle
	fillTransparentRectangle(brushRect, mat3x3::cross(transform, mat3x3::fromRectToRect(brushRect, rectangleToTransform)), b, renderTarget);
}



