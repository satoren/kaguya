
#include <string>
#include <cstdint>

namespace utfconv
{
	using std::string;
	using std::u32string;
	//typedef uint32_t char32_t;
	//typedef std::basic_string<uint32_t> u32string;



	size_t bitsize(char32_t a)
	{
		for (int shift = 31; shift > 0; shift--)
		{
			if ((a >> shift)){ return shift+1; }
		}
		return 0;
	}
	
	int u8_seq_size(size_t bitsize)
	{
		if (bitsize <= 7)
		{
			return 1;
		}
		else
		{
			return ((bitsize - 2) / 5) + 1;
		}
	}
	int u8_seq_bit_size(size_t bitsize)
	{
		size_t s= u8_seq_size(bitsize);
		if (s == 1)
		{
			return 7;
		}
		else
		{
			return s * 5 + 1;
		}
	}


	char32_t bitrange(char32_t d, size_t s, size_t e)
	{
		uint32_t mask = ((1 << (s - e)) - 1);
		return (d >> e) & mask;
	}
	string to_utf8(const u32string& data)
	{
		string result;
		result.reserve(data.size()*5);

		for (auto c : data)
		{
			size_t bsize = bitsize(c);
			size_t seqbsize = u8_seq_bit_size(bsize);
			int size = u8_seq_size(seqbsize);
			if (size == 1)
			{
				result.push_back(static_cast<char>(c));
			}
			else
			{
				size_t start = seqbsize;
				char head = ~((1 << (8-size))-1);
				size_t bitlen = 7 - size;
				char h = head + (0xFF & bitrange(c, start, start - bitlen));
				start = start - bitlen;
				result.push_back(h);
				size--;
				while (size)
				{
					size_t bitlen = 6;
					char bin = bitrange(c, start, start - bitlen);
					result.push_back(0x80 + bin);
					start = start - bitlen;
					size--;
				}
			}
		}
		return result;
	}

	int u8_headder_get_data_count(unsigned char c)
	{
		if (c <= 0x7f){ return 1; }
		if ((c & 0xC0) == 0x80){ throw std::runtime_error("bad utf8 sequence"); }
		for (int shift = 6; shift > 0;shift--)
		{
			if (!((c >> shift)&1)){ return 8 - shift -1;}
		}
		throw std::runtime_error("bad utf8 sequence");
	}
	u32string to_ucs4(const string& data)
	{
		u32string result;
		result.reserve(data.size() * 5);
		int remain = 0;
		char32_t current = 0;
		for (auto c : data)
		{
			int bit = 0;
			if (remain == 0)
			{
				remain = u8_headder_get_data_count(c);
				bit = 7 - remain;
			}
			else
			{
				if ((c & 0xC0) == 0xC0){ throw std::runtime_error("bad utf8 sequence"); }
				bit = 6;
			}

			current = (current << bit) + (c & ((1 << (bit + 1)) - 1));
			if (remain == 1)
			{
				result.push_back(current);
				current = 0;
			}
			remain--;
		}
		return result;

	}
}

