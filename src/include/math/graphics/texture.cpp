#include "texture.h"

#include "stbincluder.h"


//COPIES THE POINTER, NOT THE COLORS
texture::texture(const stdPath& path, cbool& flip) : texture(cveci2(), nullptr)
{
	stbi_set_flip_vertically_on_load(flip);
	int channelCount;
	veci2 intSize;

	colorb* byteArray = (colorb*)stbi_load(path.string().c_str(), &intSize.x, &intSize.y, &channelCount, bgraColorChannelCount);
	size = (vect2<fsize_t>)intSize;
	if (byteArray)
	{
		//rgba to bgra 
		switchChannels(byteArray, 0, 2);

		if constexpr (color::isByteColor)
		{
			baseArray = (color*)byteArray;
		}
		else
		{
			const size_t arraySize = size.volume();
			baseArray = new color[arraySize];
			std::copy(byteArray, byteArray + arraySize, baseArray);
			delete[] byteArray;
		}
	}
	else
	{
		throw std::runtime_error("file not found");
	}
}

void texture::switchChannels(colorb* byteArray, cint& channelIndex0, cint& channelIndex1) const
{
	cfsize_t& colorIDCount = size.volume();
	const colorb* const& endPtr = byteArray + colorIDCount;
	for (colorb* ptr = byteArray; ptr < endPtr; ptr++)
	{
		std::swap((*ptr)[channelIndex0], (*ptr)[channelIndex1]);
	}
}

void texture::Flip() const
{
	color* swap0 = baseArray;
	//swap until the mid point, because else it would be swapped back
	//integer division, because the mid row does not have to be swapped
	color* swap0EndPtr = swap0 + (size.x / 2) * size.y;
	color* swap1 = swap0 + (size.y - 1) * size.x;
	while (swap0 < swap0EndPtr)
	{
		color* swap0X = swap0;
		color* swap1X = swap1;
		color* swap0XEndPtr = swap0X + size.x;
		while (swap0X < swap0XEndPtr)
		{
			std::swap(*swap0X, *swap1X);
			swap0X++;
			swap1X++;
		}
		swap0 += size.x;
		swap1 -= size.x;
	}
}

bool texture::Save(const stdPath& path) const
{
	colorb* byteArray;
	if constexpr (color::isByteColor)
	{
		byteArray = (colorb*)baseArray;
	}
	else
	{
		byteArray = new colorb[size.volume()];
		std::copy(baseArray, baseArray + (size.volume()), byteArray);
	}
	//bgra to rgba
	switchChannels(byteArray, 0, 2);
	bool success = false;
	if (path.extension() == stdPath(L"png"))
	{
		success = stbi_write_png(path.string().c_str(), (int)size.x, (int)size.y, bgraColorChannelCount, byteArray, 0);
	}
	else if (path.extension() == stdPath(L"bmp"))
	{
		success = stbi_write_bmp(path.string().c_str(), (int)size.x, (int)size.y, bgraColorChannelCount, byteArray);
	}
	if constexpr (!color::isByteColor)
	{
		delete byteArray;
	}
	return success;
}

veci2 getImageSize(const stdPath& path)
{
	veci2 size;
	int comp;
	stbi_info(path.string().c_str(), &size.x, &size.y, &comp);
	return size;
}