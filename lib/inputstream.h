#pragma  once
#include <vector>
#include <string>

class input_stream
{
public:
	explicit input_stream(const std::vector<char>& buffer) : m_buf(buffer), m_streampos(0) { }

	template<class T>
	T read();

	template<class T>
	T read(unsigned size);

	template<>
	std::string read();

	auto length() const { return m_buf.size(); }
	void set_pos(auto i) {
		m_streampos = i;
	}

private:
	const std::vector<char>& m_buf;
	unsigned m_streampos;
};



template<class T>
inline T input_stream::read()
{
	return read<T>(sizeof(T));
}

template<class T>
inline T input_stream::read(unsigned size)
{
	T val = 0;
	for(int i = size - 1; i >= 0; i--)
	{
		val = val | (m_buf[m_streampos++] & 0xFF) << (i * 8);
	}

	return val;
}

template<>
inline std::string input_stream::read()
{
	std::string val;
    while(true)
    {
        char c = read<char>();
        if(c == '\0')
            break;
            
        val += c;
    }
    
    return val;
}
