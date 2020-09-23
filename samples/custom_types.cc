#include <cstdio>
#include <string>
#include <stdexcept>

#include <BinaryLove2.hpp>

using namespace BinaryLove2;

// Trivial parsable type
struct vec2
{
    float x, y;
};

// Nontrivial parsable type
class parsable_string_proxy
{
    std::string m_str;

public:
    void set(const std::string& str_)
    {
        m_str = str_;
    };

    const std::string& get() const noexcept
    {
        return m_str;
    };

public:
    // Mandatory
    // Return true, if size of the parsed data is known during compile time, 
    // false otherwise.
    constexpr static bool is_const_size() noexcept
    {
        return false;
    }

    // Mandatopry if is_const_size() == true
    // Returns compile time size.
    constexpr static size_t static_size() noexcept
    {
        return 0u;
    }

    // Mandatory
    // Returns the required size to unparse the object.
    size_t size() const noexcept
    {
        return sizeof(size_t) + sizeof(std::string::value_type) * m_str.size();
    }

    // Mandatory, returns size of the parsed object. Ex. sizeof(int)
    size_t parse(
        const byte_t* data_,    // Pointer to memory that should be read from
        size_t remaining_size_  // Size of the remaining memory
    )
    {
        std::puts("Parsing parsable_string_proxy.");

        // We don't have to do this if size is known during compile time!
        if (sizeof(size_t) > remaining_size_)
        {
            throw std::out_of_range("Not enough memory left to parse parsable_string_proxy.");
            return 0u;
        }

        size_t size{ 0u };
        memcpy(&size, data_, sizeof(size_t));
        data_ += sizeof(size_t);

        if (sizeof(std::string::value_type) * size > remaining_size_ - sizeof(size_t))
        {
            throw std::out_of_range("Not enough memory left to parse parsable_string_proxy.");
            return 0u;
        }

        m_str = std::string(size, ' ');
        memcpy(m_str.data(), data_, sizeof(std::string::value_type) * size);

        return sizeof(size_t) + sizeof(std::string::value_type) * size;
    }

    // Mandatory, returns size of the unparsed object. Ex. sizeof(int)
    size_t unparse(byte_t* data_,   // Pointer to memory where the element should be written
        size_t remaining_size_      // Size of the remaining memory
    )
    {
        std::puts("Unparsing parsable_string_proxy.");

        // We don't have to do this if size is known during compile time!
        if (sizeof(size_t) > remaining_size_)
        {
            throw std::out_of_range("Not enough memory left to unparse parsable_string_proxy.");
            return 0u;
        }

        size_t size{ m_str.size() };
        memcpy(data_, &size, sizeof(size_t));
        data_ += sizeof(size_t);

        if (sizeof(std::string::value_type) * size > remaining_size_ - sizeof(size_t))
        {
            throw std::out_of_range("Not enough memory left to unparse parsable_string_proxy.");
            return 0u;
        }

        memcpy(data_, m_str.data(), sizeof(std::string::value_type) * size);

        return sizeof(size_t) + sizeof(std::string::value_type) * size;
    }
};

// Generic parsable type
template<class... Args>
class print_size
{
    group<Args...> m_group;

public:
    // Mandatory
    // Return true, if size of the parsed data is known during compile time, 
    // false otherwise.
    constexpr static bool is_const_size() noexcept
    {
        return decltype(m_group)::is_const_size();
    }

    // Mandatopry if is_const_size() == true
    // Returns compile time size.
    constexpr static size_t static_size() noexcept
    {
        return decltype(m_group)::static_size();
    }

    // Mandatory
    // Returns the required size to unparse the object.
    size_t size() const noexcept
    {
        return m_group.size();
    }

    // Mandatory, returns size of the parsed object. Ex. sizeof(int)
    size_t parse(
        const byte_t* data_,                        // Pointer to memory that should be read from
        size_t remaining_size_,                     // Size of the remaining memory
        std::map<uint32_t, size_t>& loop_sizes_      // Handle for type that needs to access the loop
    )
    {
        std::printf("The size is: %i\n", m_group.size());
        return m_group.parse(data_, remaining_size_, loop_sizes_);
    }

    // Mandatory, returns size of the unparsed object. Ex. sizeof(int)
    size_t unparse(byte_t* data_,   // Pointer to memory where the element should be written
        size_t remaining_size_      // Size of the remaining memory
    )
    {
       std::printf("The size is: %i\n", m_group.size());
       return m_group.unparse(data_, remaining_size_);
    }
};

void custom_types_demo()
{
    std::puts("Executing Custom Types Demo.");
    typedef
        layout<
        parsable_string_proxy,
        vec2,
        print_size<vec2, float>
        >
        sample_layout_t;

    sample_layout_t sample_parse_demo;

    sample_parse_demo.get<0>().set(std::string("Piero Umiliani - Magical Moonlight"));
    sample_parse_demo.get<1>() = vec2{ 3.0f, 4.5f };

    std::vector<int8_t> parsed_data = sample_parse_demo.unparse<int8_t>();

    sample_layout_t parsed_object;
    parsed_object.parse(parsed_data);

    std::printf("The string is: %s\nVec2 is: %f, %f\n", parsed_object.get<0>().get().c_str(),
        parsed_object.get<1>().x,
        parsed_object.get<1>().y);

    std::puts("\n\n");
}