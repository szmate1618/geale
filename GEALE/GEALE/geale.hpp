#ifndef GREATEST_EVOLUTIONARY_ALGORITHM_LIBRARY_EVER
#define GREATEST_EVOLUTIONARY_ALGORITHM_LIBRARY_EVER


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

public:
	void ReadBit(size_t bitPosition, int& value);
	void WriteBit(size_t bitPosition, int value);
	void AppendBit(int value);
	void FlipBit(size_t bitPosition);
};

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::ReadBit(size_t bitPosition, int& value)
{
	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitPosition, index, offset);

	return (container.at(index) && (1 << offset)) >> offset;
}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::WriteBit(size_t bitPosition, int value)
{
	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitPosition, index, offset);

	container.at(index) |= (1 << offset);
}

template <typename Container, typename IndexConverter>
void BitArrayAdapter<Container, IndexConverter>::AppendBit(int value)
{
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
void BitArrayAdapter<Container, IndexConverter>::FlipBit(size_t bitPosition)
{
	size_t index;
	size_t offset;

	IndexConverter::GetElementIndexAndOffset(bitPosition, index, offset);

	container.at(index) ^= (1 << offset);
}

}

#endif
