#include "texture.h"
#include "color/colorFunctions.h"
// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
//void texture::Barycentric(cvec2& p, cvec2& a, cvec2& b, cvec2& c, fp& u, fp& v, fp& w)
//{
//	cvec2 v0 = b - a, v1 = c - a, v2 = p - a;
//	cfp d00 = vec2::dot(v0, v0);
//	cfp d01 = vec2::dot(v0, v1);
//	cfp d11 = vec2::dot(v1, v1);
//	cfp d20 = vec2::dot(v2, v0);
//	cfp d21 = vec2::dot(v2, v1);
//	cfp denom = d00 * d11 - d01 * d01;
//	v = (d11 * d20 - d01 * d21) / denom;
//	w = (d00 * d21 - d01 * d20) / denom;
//	u = 1.0f - v - w;
//}

inline void fadeTexture(const texture& tex, cfp& weight, const color& fadeto)
{
	color* const endPtr = tex.end();
	for (color* c = tex.begin(); c < endPtr; c++)
	{
		*c = lerpColor(fadeto, *c, weight);
	}
}
