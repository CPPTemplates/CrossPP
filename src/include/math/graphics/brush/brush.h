#include "math/graphics/color/color.h"
//#include "interface/idestructable.h"
#include <type_traits>
#pragma once

template<typename r, typename i>
struct brush
{
	typedef i inputType;
	typedef r resultingType;
	//non-virtual!!
	//virtual resultingType getValue(const inputType& pos) const = 0;
	//virtual ~brush() {};
};

typedef brush<color, vec2> colorBrush;
typedef brush<color, vect2<fsize_t>> colorBrushSizeT;

//template <typename, typename = void>
//constexpr bool is_brush_v = false;
//template <typename t, typename = t::resultingType>
//constexpr bool is_brush_v = true;

//https://devblogs.microsoft.com/oldnewthing/20190710-00/?p=102678
template<typename, typename = void>
constexpr bool is_brush_v = false;

template<typename t>
constexpr bool is_brush_v
<t, std::void_t<typename t::resultingType>> = true;
  
//template<typename r, typename i>
//constexpr bool is_brush_v<brush<r, i>> = std::is_base_of<brush<r, i>>;


//struct colorBrushSizeT : public brush<color, vect2<fsize_t>>
//{
//	//typedef vect2<fsize_t> inputType;
//	//typedef color resultingType;
//	//inline virtual color getValue(cvect2<fsize_t>& pos) const override = 0;
//
//	//inline virtual color getValue(cvec2& pos) const
//	//{
//	//
//	//	if constexpr (isDebugging)
//	//	{
//	//		if (pos.x < 0)
//	//		{
//	//			return colorPalette::red;
//	//		}
//	//		else if (pos.y < 0)
//	//		{
//	//			return colorPalette::green;
//	//		}
//	//	}
//	//	else 
//	//	{
//	//		assumeInRelease((pos.x >= 0) && (pos.y >= 0));
//	//	}
//	//	return getValue(floorVector<fsize_t>(pos));
//	//}
//};