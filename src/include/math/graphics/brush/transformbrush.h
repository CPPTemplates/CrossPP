#pragma once
constexpr int brushFixMaxTextureSize = 0x1000;
constexpr int brushFixMaxTextureScaleDivider = 0x1000;//for extreme mipmapping
constexpr int brushFixMaxTextureMargin = 0x2;
constexpr fp brushFixTranslateAmount = 1.0 / brushFixMaxTextureSize / brushFixMaxTextureMargin;
constexpr fp brushFixScaleFactor = (1.0 - (brushFixTranslateAmount / brushFixMaxTextureSize));

constexpr mat3x3 brushFixTransform = mat3x3();//mat3x3::translate(cvec2(brushFixTranslateAmount));
//	mat3x3::cross(
//	mat3x3::translate(cvec2(1.0 / (fp)0x10000)), //translate the point a bit forward so when it rotates it won't access the end of the array (it's like the pixels are in the center)
//	mat3x3::scale(cvec2(0xffffff / (fp)0x1000000)) //scales a bit so the last pixels don't get out of bounds
//);
//transforms a point (after offsetting it a bit for rotation) and then calls a base brush
template<typename brush0Type>
struct transformBrush final : public colorBrush
{
//private:
	//transform: from screen pixel (or other transform) to texture pixel
	mat3x3 modifiedTransform;//cannot access the transform, as it is modified
//public:
	const brush0Type& baseBrush;
	vecb2 roundUp;
	transformBrush(const mat3x3& transform, const brush0Type& baseBrush) :
		modifiedTransform(mat3x3::cross(brushFixTransform, transform)),//transformed pixel -> scale (make smaller), translate
		baseBrush(baseBrush)
	{
		vec2 add = vec2();
		for (int i = 0; i < 2; i++) {
			vec2 testSize = vec2(1);//directionVectors2D[i * 2 + 1];
			vec2 result = transform.multSizeMatrix(testSize);
			roundUp[i] = result[i] < 0;
			if (roundUp[i]) {
				add[i] -= 1.0 / 0x1000;
			}
		}
		this->modifiedTransform = mat3x3::cross(mat3x3::translate(add), transform);
	}

	inline color getValue(cvec2& pos) const
	{
		//if constexpr (std::is_integral_v<brush0Type::inputType>) {
		//vec2 baseBrushPos = transform.multPointMatrix(pos);
		//for (int i = 0; i < 2; i++) {
		//	baseBrushPos[i] -= (roundUp[i] & (baseBrushPos[i] == (int)baseBrushPos[i]));
		//	//if (roundUp[i] && baseBrushPos[i] == (int)baseBrushPos[i]) {
		//	//	baseBrushPos[i]--;
		//	//}
		//}
		//for (int i = 0; i < 2; i++) {
		//	if (roundUp[i] && baseBrushPos[i] == (int)baseBrushPos[i]) {
		//		baseBrushPos[i]--;
		//	}
		//}
		//if (pos.y == (int)pos.y) {
		//	pos.y--;
		//}
	//}
		//return baseBrush.getValue(baseBrushPos);
		return baseBrush.getValue((typename brush0Type::inputType)modifiedTransform.multPointMatrix(pos));
	}
};