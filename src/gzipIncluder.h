//#pragma once
#define ZLIB_CONST
#pragma warning(push, 0)
#pragma warning(disable : 4068)
#include <zlib.h>

//https://github.com/mapbox/gzip-hpp/blob/master/include/gzip/decompress.hpp
#include <stdint.h>
#pragma once

//#include "zlib-1.2.11/zlib-1.2.11/zlib.h"

/// The major version number
#define GZIP_VERSION_MAJOR 1

/// The minor version number
#define GZIP_VERSION_MINOR 0

/// The patch number
#define GZIP_VERSION_PATCH 0

/// The complete version number
#define GZIP_VERSION_CODE (GZIP_VERSION_MAJOR * 10000 + GZIP_VERSION_MINOR * 100 + GZIP_VERSION_PATCH)

/// Version number as string
#define GZIP_VERSION_STRING "1.0.0"

namespace gzip {

	class Compressor
	{
		std::size_t max_;
		int level_;

	public:
		Compressor(int level = Z_DEFAULT_COMPRESSION,
			std::size_t max_bytes = 2000000000) // by default refuse operation if uncompressed data is > 2GB
			: max_(max_bytes),
			level_(level)
		{
		}

		template <typename InputType>
		void compress(InputType& output,
			const char* data,
			std::size_t size) const
		{

#ifdef DEBUG
			// Verify if size input will fit into unsigned int, type used for zlib's avail_in
			if (size > std::numeric_limits<unsigned int>::max())
			{
				throw std::runtime_error("size arg is too large to fit into unsigned int type");
			}
#endif
			if (size > max_)
			{
				throw std::runtime_error("size may use more memory than intended when decompressing");
			}

			z_stream deflate_s;
			deflate_s.zalloc = Z_NULL;
			deflate_s.zfree = Z_NULL;
			deflate_s.opaque = Z_NULL;
			deflate_s.avail_in = 0;
			deflate_s.next_in = Z_NULL;

			// The windowBits parameter is the base two logarithm of the window size (the size of the history buffer).
			// It should be in the range 8..15 for this version of the library.
			// Larger values of this parameter result in better compression at the expense of memory usage.
			// This range of values also changes the decoding type:
			//  -8 to -15 for raw deflate
			//  8 to 15 for zlib
			// (8 to 15) + 16 for gzip
			// (8 to 15) + 32 to automatically detect gzip/zlib header (decompression/inflate only)
			constexpr int window_bits = 15 + 16; // gzip with windowbits of 15

			constexpr int mem_level = 8;
			// The memory requirements for deflate are (in bytes):
			// (1 << (window_bits+2)) +  (1 << (mem_level+9))
			// with a default value of 8 for mem_level and our window_bits of 15
			// this is 128Kb

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
			if (deflateInit2(&deflate_s, level_, Z_DEFLATED, window_bits, mem_level, Z_DEFAULT_STRATEGY) != Z_OK)
			{
				throw std::runtime_error("deflate init failed");
			}
#pragma GCC diagnostic pop

			deflate_s.next_in = reinterpret_cast<z_const Bytef*>(data);
			deflate_s.avail_in = static_cast<unsigned int>(size);

			std::size_t size_compressed = 0;
			do
			{
				size_t increase = size / 2 + 1024;
				if (output.size() < (size_compressed + increase))
				{
					output.resize(size_compressed + increase);
				}
				// There is no way we see that "increase" would not fit in an unsigned int,
				// hence we use static cast here to avoid -Wshorten-64-to-32 error
				deflate_s.avail_out = static_cast<unsigned int>(increase);
				deflate_s.next_out = reinterpret_cast<Bytef*>((&output[0] + size_compressed));
				// From http://www.zlib.net/zlib_how.html
				// "deflate() has a return value that can indicate errors, yet we do not check it here.
				// Why not? Well, it turns out that deflate() can do no wrong here."
				// Basically only possible error is from deflateInit not working properly
				deflate(&deflate_s, Z_FINISH);
				size_compressed += (increase - deflate_s.avail_out);
			} while (deflate_s.avail_out == 0);

			deflateEnd(&deflate_s);
			output.resize(size_compressed);
		}
	};

	inline std::string compress(const char* data,
		std::size_t size,
		int level = Z_DEFAULT_COMPRESSION)
	{
		Compressor comp(level);
		std::string output;
		comp.compress(output, data, size);
		return output;
	}

	class Decompressor
	{
		std::size_t max_;

	public:
		Decompressor(std::size_t max_bytes = 1000000000) // by default refuse operation if compressed data is > 1GB
			: max_(max_bytes)
		{
		}

		template <typename OutputType>
		void decompress(OutputType& output,
			const char* data,
			std::size_t size) const
		{
			z_stream inflate_s;

			inflate_s.zalloc = Z_NULL;
			inflate_s.zfree = Z_NULL;
			inflate_s.opaque = Z_NULL;
			inflate_s.avail_in = 0;
			inflate_s.next_in = Z_NULL;

			// The windowBits parameter is the base two logarithm of the window size (the size of the history buffer).
			// It should be in the range 8..15 for this version of the library.
			// Larger values of this parameter result in better compression at the expense of memory usage.
			// This range of values also changes the decoding type:
			//  -8 to -15 for raw deflate
			//  8 to 15 for zlib
			// (8 to 15) + 16 for gzip
			// (8 to 15) + 32 to automatically detect gzip/zlib header
			constexpr int window_bits = 15 + 32; // auto with windowbits of 15

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
			if (inflateInit2(&inflate_s, window_bits) != Z_OK)
			{
				throw std::runtime_error("inflate init failed");
			}
#pragma GCC diagnostic pop
			inflate_s.next_in = reinterpret_cast<z_const Bytef*>(data);

#ifdef DEBUG
			// Verify if size (long type) input will fit into unsigned int, type used for zlib's avail_in
			std::uint64_t size_64 = size * 2;
			if (size_64 > std::numeric_limits<unsigned int>::max())
			{
				inflateEnd(&inflate_s);
				throw std::runtime_error("size arg is too large to fit into unsigned int type x2");
			}
#endif
			if (size > max_ || (size * 2) > max_)
			{
				inflateEnd(&inflate_s);
				throw std::runtime_error("size may use more memory than intended when decompressing");
			}
			inflate_s.avail_in = static_cast<unsigned int>(size);
			std::size_t size_uncompressed = 0;
			do
			{
				std::size_t resize_to = size_uncompressed + 2 * size;
				if (resize_to > max_)
				{
					inflateEnd(&inflate_s);
					throw std::runtime_error("size of output string will use more memory then intended when decompressing");
				}
				output.resize(resize_to);
				inflate_s.avail_out = static_cast<unsigned int>(2 * size);
				inflate_s.next_out = reinterpret_cast<Bytef*>(&output[0] + size_uncompressed);
				int ret = inflate(&inflate_s, Z_FINISH);
				if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR)
				{
					std::string error_msg = inflate_s.msg;
					inflateEnd(&inflate_s);
					throw std::runtime_error(error_msg);
				}

				size_uncompressed += (2 * size - inflate_s.avail_out);
			} while (inflate_s.avail_out == 0);
			inflateEnd(&inflate_s);
			output.resize(size_uncompressed);
		}
	};

	inline std::string decompress(const char* data, std::size_t size)
	{
		Decompressor decomp;
		std::string output;
		decomp.decompress(output, data, size);
		return output;
	}

	// These live in gzip.hpp because it doesnt need to use deps.
	// Otherwise, they would need to live in impl files if these methods used
	// zlib structures or functions like inflate/deflate)
	inline bool is_compressed(const char* data, std::size_t size)
	{
		return size > 2 &&
			(
				// zlib
				(
					static_cast<uint8_t>(data[0]) == 0x78 &&
					(static_cast<uint8_t>(data[1]) == 0x9C ||
						static_cast<uint8_t>(data[1]) == 0x01 ||
						static_cast<uint8_t>(data[1]) == 0xDA ||
						static_cast<uint8_t>(data[1]) == 0x5E)) ||
				// gzip
				(static_cast<uint8_t>(data[0]) == 0x1F && static_cast<uint8_t>(data[1]) == 0x8B));
	}
} // namespace gzip

//#include <gzip/decompress.hpp>
#pragma warning(pop)