
#ifndef MEGOPP_ERR_HPP_INCLUDED
#define MEGOPP_ERR_HPP_INCLUDED

#include <mego/err/ec.h>

#include <memepp/string.hpp>

#include <exception>
#include <memory>

namespace mgpp {

namespace detail {
struct err
{
    err():
        code_{ MGEC__OK }
    {}

    err(mgec_t code):
        code_{ code }
    {}

    err(mgec_t code, const memepp::string &message):
        code_{ code },
        message_{ message }
    {}

    err(mgec_t code, const memepp::string &message, const memepp::string &solution):
        code_{ code },
        message_{ message },
        solution_{ solution }
    {}
    
    err(const std::shared_ptr<void> &data):
        code_{ MGEC__OK },
        data_{ data }
    {}

    err(mgec_t code, const std::shared_ptr<void> &data):
        code_{ code },
        data_{ data }
    {}

    err(mgec_t code, const memepp::string &message, const std::shared_ptr<void> &data):
        code_{ code },
        message_{ message },
        data_{ data }
    {}

    err(mgec_t code, const memepp::string &message, const memepp::string &solution, const std::shared_ptr<void> &data):
        code_{ code },
        message_{ message },
        solution_{ solution },
        data_{ data }
    {}

    err(const err &e):
        code_{ e.code_ },
        message_{ e.message_ },
        solution_{ e.solution_ },
        data_{ e.data_ }
    {
    }

    err(err &&e) noexcept:
        code_{ e.code_ },
        message_{ std::move(e.message_) },
        solution_{ std::move(e.solution_) },
        data_{ std::move(e.data_) }
    {
    }

    err &operator=(const err &e)
    {
        code_       = e.code_;
        message_    = e.message_;
        solution_   = e.solution_;
        data_       = e.data_;
        return *this;
    }

    err &operator=(err &&e) noexcept
    {
        code_       = e.code_;
        message_    = std::move(e.message_);
        solution_   = std::move(e.solution_);
        data_       = std::move(e.data_);
        return *this;
    }

    ~err() {
    }

    mgec_t code_;
    memepp::string message_;
    memepp::string solution_;

    std::shared_ptr<void> data_;
};
}; // namespace detail  

class err : public std::exception 
{
public:
    err ():
        impl_{ std::make_shared<detail::err>() }
    {}

    err (mgec_t code):
        impl_{ std::make_shared<detail::err>(code) }
    {}

    err (mgec_t code, const memepp::string &message):
        impl_{ std::make_shared<detail::err>(code, message) }
    {}

    err (mgec_t code, const memepp::string &message, const memepp::string &solution):
        impl_{ std::make_shared<detail::err>(code, message, solution) }
    {}

    err (const std::shared_ptr<void> &data):
        impl_{ std::make_shared<detail::err>(data) }
    {}

    err (mgec_t code, const std::shared_ptr<void> &data):
        impl_{ std::make_shared<detail::err>(code, data) }
    {}

    err (mgec_t code, const memepp::string &message, const std::shared_ptr<void> &data):
        impl_{ std::make_shared<detail::err>(code, message, data) }
    {}

    err (mgec_t code, const memepp::string &message, const memepp::string &solution, const std::shared_ptr<void> &data):
        impl_{ std::make_shared<detail::err>(code, message, solution, data) }
    {}

    err (const std::shared_ptr<detail::err> &impl):
        impl_{ impl }
    {}

    err (const std::shared_ptr<detail::err> &impl, const std::shared_ptr<detail::err> &next):
        impl_{ impl },
        next_{ next }
    {}

    err (const err &e):
        impl_{ e.impl_ },
        next_{ e.next_ }
    {}

    err (err &&e) noexcept:
        impl_{ std::move(e.impl_) },
        next_{ std::move(e.next_) }
    {}

    err &operator=(const err &e)
    {
        impl_ = e.impl_;
        next_ = e.next_;
        return *this;
    }

    err &operator=(err &&e) noexcept
    {
        impl_ = std::move(e.impl_);
        next_ = std::move(e.next_);
        return *this;
    }

    const char* what() const noexcept override;

    mgec_t code() const noexcept;
    const memepp::string &message () const noexcept;
    const memepp::string &solution() const noexcept;

    std::shared_ptr<void> user_data() const noexcept;
    err copy() const;
    err next() const;

    bool ok() const noexcept;
    explicit operator bool() const noexcept;

    void set_next(const err &e);
    void set_message(const memepp::string &message);
    void set_solution(const memepp::string &solution);
    void set_user_data(const std::shared_ptr<void> &data);

    inline static err make_ok()
    {
        static err e;
        return e;
    }

    inline static err unknown_result()
    {
        static err e { MGEC__ERR };
        return e;
    }
private:
    std::shared_ptr<detail::err> impl_;
    std::shared_ptr<detail::err> next_;
};

    inline const char* err::what() const noexcept
    {
        return message().data();
    }

    inline mgec_t err::code() const noexcept
    {
        return impl_->code_;
    }

    inline const memepp::string &err::message() const noexcept
    {
        return impl_->message_;
    }

    inline const memepp::string &err::solution() const noexcept
    {
        return impl_->solution_;
    }

    inline std::shared_ptr<void> err::user_data() const noexcept
    {
        return impl_->data_;
    }

    inline err err::copy() const
    {
        auto impl = impl_;
        return { std::make_shared<detail::err>(*impl), next_ };
    }
    
    inline err err::next() const
    {
        auto next = next_;
        return next ? err{ next } : make_ok();
    }

    inline bool err::ok() const noexcept
    {
        return code() == MGEC__OK;
    }

    inline err::operator bool() const noexcept
    {
        return ok();
    }

    inline void err::set_next(const err &e)
    {
        if (impl_ != e.impl_)
            next_  = e.impl_;
    }

    inline void err::set_message(const memepp::string &message)
    {
        auto err = copy();
        err.impl_->message_ = message;
        *this = err;
    }

    inline void err::set_solution(const memepp::string &solution)
    {
        auto err = copy();
        err.impl_->solution_ = solution;
        *this = err;
    }

    inline void err::set_user_data(const std::shared_ptr<void> &data)
    {
        auto err = copy();
        err.impl_->data_ = data;
        *this = err;
    }

}; // namespace mgpp

#endif // !MEGOPP_ERR_HPP_INCLUDED
