#ifndef GREATEST_EVOLUTIONARY_ALGORITHM_LIBRARY_EVER
#define GREATEST_EVOLUTIONARY_ALGORITHM_LIBRARY_EVER

#include <climits>


namespace geale
{

template <int bitWidth>
class IndexConverter
{
public:
	void GetElementIndexAndOffset(size_t bitPosition, size_t& index, size_t& offset);
};

template <int bitWidth>
void IndexConverter<bitWidth>::GetElementIndexAndOffset(size_t bitPosition, size_t& index, size_t& offset)
{
	index = bitPosition / bitWidth;
	offset = bitPosition % bitWidth;
}

template <typename Container, typename IndexConverter>
class BitArrayAdapter
{
private:
	Container container;
	size_t bitCount;
	size_t lastReadBitPositionPlus1;
	size_t lastWriteBitPositionPlus1;

public:
	BitArrayAdapter();

	void ReadBit(size_t bitPosition, int& value);
	void WriteBit(size_t bitPosition, int value);
	void AppendBit(int value);
	void WriteOrAppendBit(size_t bitPosition, int value);
	void FlipBit(size_t bitPosition);

	size_t GetLastReadBitPositionPlus1();
	size_t GetLastWriteBitPositionPlus1();
};

template <typename Container, typename IndexConverter>
BitArrayAdapter<Container, IndexConverter>::BitArrayAdapter() : bitCount(0), lastReadBitPositionPlus1(0), lastWriteBitPositionPlus1(0) {}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::ReadBit(size_t bitPosition, int& value)
{
	lastReadBitPositionPlus1 = bitPosition + 1;

	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitPosition, index, offset);

	return (container.at(index) && (1 << offset)) >> offset;
}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::WriteBit(size_t bitPosition, int value)
{
	lastWriteBitPositionPlus1 = bitPosition + 1;

	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitPosition, index, offset);

	container.at(index) |= (1 << offset);
}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::AppendBit(int value)
{
	lastWriteBitPositionPlus1 = bitCount + 1;

	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitCount, index, offset);

	if (index >= container.size()) container.push_back(0);

	if (value > 0)
	{
		container.at(index) |= (1 << offset);
	}

	bitCount++;
}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::WriteOrAppendBit(size_t bitPosition, int value)
{
	if (bitPosition < bitCount)
	{
		WriteBit(bitPosition, value);
	}
	else
	{
		AppendBit(value);
	}
}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::FlipBit(size_t bitPosition)
{
	lastWriteBitPositionPlus1 = bitPosition + 1;

	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitPosition, index, offset);

	container.at(index) ^= (1 << offset);
}

template <typename Container, typename IndexConverter>
size_t BitArrayAdapter<Container, IndexConverter>::GetLastReadBitPositionPlus1()
{
	return lastReadBitPositionPlus1;
}

template <typename Container, typename IndexConverter>
size_t BitArrayAdapter<Container, IndexConverter>::GetLastWriteBitPositionPlus1()
{
	return lastWriteBitPositionPlus1;
}


template <typename T, typename BitArray>
class BinaryEncoder
{
public:
	void Encode(size_t startBitPosition, const T& value, BitArray& bitArray);
};

template <typename T, typename BitArray>
void BinaryEncoder<T, BitArray>::Encode(size_t startBitPosition, const T& value, BitArray& bitArray)
{
	for (size_t i = 0; i < CHAR_BIT * sizeof(T); i++)
	{
		bitArray.WriteOrAppendBit(startBitPosition + i, (value & (1 << i)) >> i);
	}
}

template <typename T, typename BitArray>
class BinaryDecoder
{
public:
	void Decode(size_t startBitPosition, T& value, const BitArray& bitArray);
};

template <typename T, typename BitArray>
void BinaryDecoder<T, BitArray>::Decode(size_t startBitPosition, T& value, const BitArray& bitArray)
{
	value = 0;
	for (size_t i = 0; i < CHAR_BIT * sizeof(T); i++)
	{
		int bitValue;
		bitArray.ReadBit(startBitPosition + i, bitValue);
		value |= (bitValue << i);
	}
}

}

#endif
