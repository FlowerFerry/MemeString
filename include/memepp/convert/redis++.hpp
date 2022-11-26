
#ifndef MEMEPP_CONVERT_REDISXX_HPP_INCLUDED
#define MEMEPP_CONVERT_REDISXX_HPP_INCLUDED

#include <memepp/string.hpp>
#include <sw/redis++/reply.h>

namespace sw {
namespace redis {
    namespace reply {

        inline ::memepp::string parse(ParseTag<memepp::string>, redisReply& reply)
        {
            if (!reply::is_string(reply) && !reply::is_status(reply))
            {
                throw ProtoError("Expect STRING reply");
            }

            if (reply.str == nullptr) {
                throw ProtoError("A null string reply");
            }

            return ::memepp::string(reply.str, reply.len);
        }

    };
};
};

#endif // !MEMEPP_CONVERT_REDISXX_HPP_INCLUDED
