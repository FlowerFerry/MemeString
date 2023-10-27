
#ifndef MEGOPP_ERR_HPP_INCLUDED
#define MEGOPP_ERR_HPP_INCLUDED


#include <mego/err/ec.h>

#include <memepp/string.hpp>

#include <exception>
#include <memory>
#include <map>

namespace mgpp {

namespace detail {
struct err
{
    struct func_info
    {
        memepp::string name;
        std::map<int, memepp::string> args;
    };

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
        solution_{ std::make_unique<memepp::string>(solution) }
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
        solution_{ std::make_unique<memepp::string>(solution) },
        data_{ data }
    {}

    err(const err &e):
        code_{ e.code_ },
        message_{ e.message_ },
        solution_{ e.solution_ ? std::make_unique<memepp::string>(*e.solution_) : nullptr },
        data_{ e.data_ },
        func_info_{ e.func_info_ ? std::make_unique<func_info>(*e.func_info_) : nullptr }
    {
    }

    err(err &&e) noexcept:
        code_{ e.code_ },
        message_{ std::move(e.message_) },
        solution_{ std::move(e.solution_) },
        data_{ std::move(e.data_) },
        func_info_{ std::move(e.func_info_) }
    {
    }

    err &operator=(const err &e)
    {
        code_       = e.code_;
        message_    = e.message_;
        solution_   = (e.solution_ ? std::make_unique<memepp::string>(*e.solution_) : nullptr);
        data_       = e.data_;
        func_info_  = (e.func_info_ ? std::make_unique<func_info>(*e.func_info_) : nullptr);
        return *this;
    }

    err &operator=(err &&e) noexcept
    {
        code_       = e.code_;
        message_    = std::move(e.message_);
        solution_   = std::move(e.solution_);
        data_       = std::move(e.data_);
        func_info_  = std::move(e.func_info_);
        return *this;
    }

    ~err() {
    }

    mgec_t code_;
    memepp::string message_;
    std::unique_ptr<memepp::string> solution_;
    std::unique_ptr<func_info> func_info_;

    std::shared_ptr<void> data_;
};
}; // namespace detail  

class err : public std::exception 
{
public:

    using func_info = detail::err::func_info;

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

    memepp::string solution() const noexcept;

    bool has_func_info() const noexcept;
    const func_info* get_func_info() const noexcept;

    std::shared_ptr<void> user_data() const noexcept;
    err copy() const;
    err next() const;

    bool ok() const noexcept;
    explicit operator bool() const noexcept;

    void set_next(const err &e);
    void set_message(const memepp::string &message);
    void set_solution(const memepp::string &solution);

    void set_func_info(const func_info &info);
    void set_user_data(const std::shared_ptr<void> &data);

    inline static err make_ok()
    {
        static err e;
        return e;
    }

    inline static err make_unknown()
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

    inline memepp::string err::solution() const noexcept
    {
        return impl_->solution_ ? *impl_->solution_ : memepp::string{};
    }

    inline bool err::has_func_info() const noexcept
    {
        return impl_->func_info_ != nullptr;
    }

    inline const err::func_info* err::get_func_info() const noexcept
    {
        return impl_->func_info_.get();
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
        return !ok();
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
        err.impl_->solution_ = std::make_unique<memepp::string>(solution);
        *this = err;
    }

    inline void err::set_func_info(const func_info &info)
    {
        auto err = copy();
        err.impl_->func_info_ = std::make_unique<func_info>(info);
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
