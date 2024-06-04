#include "iserializable.h"
#include "fileio.h"

void ISerializable::clone(ISerializable &cloneTo)
{
	std::stringstream buf = std::stringstream();
	auto iFace = createStreamInterface(buf);
	streamSerializer s = streamSerializer(iFace, true);
	serialize(s); // save in string stream
	s.write = false;
	buf.seekp(0);		  // reset position
	cloneTo.serialize(s); // read from string stream
}
bool ISerializable::serialize(const stdPath &path, cbool &write, const std::endian &fileEndianness)
{
	std::shared_ptr<char[]> buffer;
	std::fstream stream = getAcceleratedFstream(path, write, buffer, 0x1000);
	if (stream.good())
	{
		auto iFace = createStreamInterface(stream);
		streamSerializer s = streamSerializer(iFace, write, fileEndianness);
		serialize(s);
		stream.close();
		return true;
	}
	else
	{
		return false;
	}
}