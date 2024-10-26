#include "magmaCube.h"
magmaCube::magmaCube(dimension* dimensionIn, cvec2& position) :slime(dimensionIn, position, entityID::magma_cube)
{
	//call the derived function
	resize();
}

void magmaCube::createBodyParts()
{
	cvec2 headSize = cvec2(size * cvec2(slimeInnerBodyTextureRect.size) / cvec2(slimeOuterBodyTextureRect.size));
	mainBodyPart = new bodyPart2D(slimeInnerBodyTextureRect, nullptr, position, headSize, headSize * 0.5);
	head = mainBodyPart;
	cvec2 layerSize = cvec2(size, size / magmaCubeLayerCount);
	for (int i = 0; i < magmaCubeLayerCount; i++)
	{
		crectangle2 textureRect = (i == bottomEyeSliceIndex) ? bottomEyeSliceTextureRect : ((i == topEyeSliceIndex) ? topEyeSliceTextureRect :
			crectangle2(slimeOuterBodyTextureRect.pos0 + cvec2(0, magmaCubeLayerThickness * i), cvec2(slimeOuterBodyTextureRect.size.x, magmaCubeLayerThickness)));
		layers[i] = new bodyPart2D(textureRect, head, cvec2(0, size * (((i + 0.5) / magmaCubeLayerCount) - 0.5)), layerSize, layerSize * 0.5);
		head->children.push_back(layers[i]);
	}
}