#include "resolutiontexture.h"
#include "math/graphics/brush/brushes.h"
#include "math/mathFunctions.h"
#include "math/graphics/color/colorFunctions.h"
//COPIES THE COLORS, NOT THE POINTER
resolutionTexture::resolutionTexture(const texture& highestResolution, cvec2& size)
{
	fsize_t resolutionSteps = math::getNextPowerOf2((fsize_t)size.minimum());
	//for (fsize_t i = 1; i < highestResolution.size.x && i < highestResolution.size.y; i *= 2)
	//{
	//	resolutionSteps++;
	//}

	scaledTextures = fastArray<texture*>(resolutionSteps, true);
	scaledTextures[0] = new texture(highestResolution);

	recalculateScaledTextures();

	this->defaultSize = size;
}

void resolutionTexture::recalculateScaledTextures() const
{
	if (scaledTextures.size > 1)
	{
		for (size_t i = 1; i < scaledTextures.size; ++i)
		{
			if (scaledTextures[i])
			{
				delete scaledTextures[i];
			}
			scaledTextures[i] = new texture(getHalfResolution(*scaledTextures[i - 1]));
		}
	}
}

resolutionTexture::~resolutionTexture()
{
	for (texture* tex : scaledTextures)
	{
		if (tex)
		{
			delete tex;
		}
	}
}

//transformBrush<texture> resolutionTexture::getAntiAliasedBrush(cmat3x3& transform) const
//{
//	const texture& correctTexture = getTexture(transform);
//
//	//inverse scaling
//	return transformBrush<texture>(mat3x3::scale(cvec2(correctTexture.size) / size), correctTexture);
//}

const texture& resolutionTexture::mipmap(mat3x3& transform, rectangle2& textureRect) const
{
	cfp& renderSizeX = getTransformedTextureWidth(transform);
	const texture& tex = getMipmapTexture(renderSizeX);
	cfp& multiplier = defaultSize.x / tex.size.x;
	transform = mat3x3::cross(transform, mat3x3::scale(vec3(multiplier, multiplier, 1)));
	textureRect.pos0 /= multiplier;
	textureRect.size /= multiplier;
	return tex;
}

texture resolutionTexture::getHalfResolution(const texture& doubleResolution)
{
	texture result(doubleResolution.size / 2);

	//average colors
	for (fsize_t y = 0; y < result.size.y; y++)
	{
		const color* srcY = doubleResolution.baseArray + doubleResolution.size.x * (y + y);
		color* destY = result.baseArray + result.size.x * y;
		for (fsize_t x = 0; x < result.size.x; x++)
		{
			const color* srcX = srcY + (x + x);
			*(destY + x) = averageColor(
				//xy
				*srcX,											//00
				*(srcX + 1),									//10
				*(srcX + doubleResolution.size.x),		//01
				*(srcX + doubleResolution.size.x + 1)	//11
			);
		}
	}
	return result;
}

texture resolutionTexture::getDoubleResolution(const texture& halfResolution)
{
	texture result(halfResolution.size * 2);
	fillTransformedTexture(crectangle2(result.getClientRect()), halfResolution, result);
	return result;
}
