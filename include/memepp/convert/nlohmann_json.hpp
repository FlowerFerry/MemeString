
#ifndef MEMEPP_CONVERT_NLOHMANN_JSON_HPP_INCLUDED
#define MEMEPP_CONVERT_NLOHMANN_JSON_HPP_INCLUDED

#include <nlohmann/json.hpp>
#include <memepp/variable_buffer.hpp>

namespace memepp {

    class nlohmann_output_adapter : public nlohmann::detail::output_adapter_protocol<MemeByte_t>
    {
    public:
        explicit nlohmann_output_adapter(memepp::variable_buffer& vec) noexcept
            : v(vec)
        {}

        void write_character(MemeByte_t c) override
        {
            v.push_back(c);
        }

        void write_characters(const MemeByte_t* s, std::size_t length) override
        {
            v.append(s, length);
        }

    private:
        memepp::variable_buffer& v;
    };

};

#endif // !MEMEPP_CONVERT_NLOHMANN_JSON_HPP_INCLUDED
