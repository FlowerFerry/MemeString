
#ifndef MEGOPP_ERR_HPP_INCLUDED
#define MEGOPP_ERR_HPP_INCLUDED


#include <mego/err/ec.h>
#include <memepp/string.hpp>
#include <exception>
#include <memory>
#include <deque>
#include <map>

namespace mgpp {

class err; // forward declaration for details::err_payload

namespace details {

    struct fninfo_err
    {
        struct fninfo
        {
            memepp::string name_;
            std::map<int, memepp::string> args_;
        };
    };

    struct err_payload
    {
        err_payload() = default;

        err_payload(const err_payload& _o)
            : message_ (_o.message_)
            , solution_(_o.solution_ ? std::make_unique<memepp::string>(*_o.solution_)         : nullptr)
            , fninfo_  (_o.fninfo_   ? std::make_unique<fninfo_err::fninfo>(*_o.fninfo_)       : nullptr)
            , next_    (_o.next_)
            , userdata_(_o.userdata_)
        {}

        err_payload(err_payload&&) = default;

        err_payload& operator=(const err_payload& _o)
        {
            message_  = _o.message_;
            solution_ = _o.solution_ ? std::make_unique<memepp::string>(*_o.solution_)     : nullptr;
            fninfo_   = _o.fninfo_   ? std::make_unique<fninfo_err::fninfo>(*_o.fninfo_)   : nullptr;
            next_     = _o.next_;
            userdata_ = _o.userdata_;
            return *this;
        }

        err_payload& operator=(err_payload&&) = default;

        memepp::string                      message_;
        std::unique_ptr<memepp::string>     solution_;
        std::unique_ptr<fninfo_err::fninfo> fninfo_;
        std::shared_ptr<mgpp::err>          next_;
        std::shared_ptr<void>               userdata_;
    };

    inline std::unique_ptr<err_payload> make_err(const memepp::string& _msg)
    {
        auto p = std::make_unique<err_payload>();
        p->message_ = _msg;
        return p;
    }

    inline std::unique_ptr<err_payload> make_err(const memepp::string& _msg, const memepp::string& _sln)
    {
        auto p = std::make_unique<err_payload>();
        p->message_  = _msg;
        p->solution_ = std::make_unique<memepp::string>(_sln);
        return p;
    }

    inline std::unique_ptr<err_payload> make_err(const fninfo_err::fninfo& _fninfo)
    {
        auto p = std::make_unique<err_payload>();
        p->fninfo_ = std::make_unique<fninfo_err::fninfo>(_fninfo);
        return p;
    }

    inline std::unique_ptr<err_payload> make_err(const memepp::string& _msg, const fninfo_err::fninfo& _fninfo)
    {
        auto p = std::make_unique<err_payload>();
        p->message_ = _msg;
        p->fninfo_  = std::make_unique<fninfo_err::fninfo>(_fninfo);
        return p;
    }

    inline std::unique_ptr<err_payload> make_err(
        const memepp::string& _msg, const memepp::string& _sln, const fninfo_err::fninfo& _fninfo)
    {
        auto p = std::make_unique<err_payload>();
        p->message_  = _msg;
        p->solution_ = std::make_unique<memepp::string>(_sln);
        p->fninfo_   = std::make_unique<fninfo_err::fninfo>(_fninfo);
        return p;
    }

    inline std::unique_ptr<err_payload> make_err(const std::shared_ptr<void>& _data)
    {
        auto p = std::make_unique<err_payload>();
        p->userdata_ = _data;
        return p;
    }

    inline const memepp::string& err_get_message(const err_payload* _e)
    {
        static const memepp::string empty{};
        if (!_e)
            return empty;
        return _e->message_;
    }

    inline const memepp::string& err_get_solution(const err_payload* _e)
    {
        static const memepp::string empty{};
        if (!_e || !_e->solution_)
            return empty;
        return *_e->solution_;
    }

    inline const fninfo_err::fninfo* err_get_fninfo(const err_payload* _e)
    {
        if (!_e)
            return nullptr;
        return _e->fninfo_.get();
    }

    inline std::shared_ptr<void> err_get_userdata(const err_payload* _e)
    {
        if (!_e)
            return nullptr;
        return _e->userdata_;
    }

    // inline void err_set_message(std::unique_ptr<err_payload>& _e, const memepp::string& _msg)
    // {
    //     if (!_e)
    //         return;
    //     _e->message_ = _msg;
    // }

    // inline void err_set_solution(std::unique_ptr<err_payload>& _e, const memepp::string& _sln)
    // {
    //     if (!_e)
    //         return;
    //     if (!_e->solution_)
    //         _e->solution_ = std::make_unique<memepp::string>(_sln);
    //     else
    //         *_e->solution_ = _sln;
    // }

    // inline void err_set_funcinfo(std::unique_ptr<err_payload>& _e, const fninfo_err::fninfo& _fninfo)
    // {
    //     if (!_e)
    //         return;
    //     if (!_e->fninfo_)
    //         _e->fninfo_ = std::make_unique<fninfo_err::fninfo>(_fninfo);
    //     else
    //         *_e->fninfo_ = _fninfo;
    // }

    // inline void err_set_userdata(std::unique_ptr<err_payload>& _e, const std::shared_ptr<void>& _data)
    // {
    //     if (!_e)
    //         return;
    //     _e->userdata_ = _data;
    // }

}; // namespace details  

class err;
class err_cond;

//! \brief Error category
class err_cat
{
public:
    virtual ~err_cat() = default;

    virtual const char* name() const noexcept = 0;
    virtual memepp::string message(int _errval) const noexcept = 0;
    
	virtual err_cond get_err_cond(int _errval) const noexcept;

	virtual bool equivalent(int _errval, const err_cond& _cond) const noexcept;

	virtual bool equivalent(const err& _code, int _errval) const noexcept;

    inline bool operator==(const err_cat& _rhs) const noexcept
    {
        return this == &_rhs;
    }

    inline bool operator!=(const err_cat& _rhs) const noexcept
    {
        return this != &_rhs;
    }

    inline bool operator<(const err_cat& _rhs) const noexcept
    {
        return this < &_rhs;
    }

    inline bool operator>(const err_cat& _rhs) const noexcept
    {
        return this > &_rhs;
    }
};

class generic_err_cat : public err_cat
{
public:
    const char* name() const noexcept override
    {
        return "generic";
    }

    memepp::string message(int _errval) const noexcept override
    {
        (void)_errval;
        return {};
    }
    
};

inline const err_cat* get_genrc_err_cat() noexcept
{
    static generic_err_cat cat;
    return &cat;
}

class err_cond
{
public:
    err_cond() noexcept
        : errval_{ 0 }
        , cat_{ get_genrc_err_cat() }
    {}

    err_cond(int _errval, const err_cat* _cat) noexcept
        : errval_{ _errval }
        , cat_{ _cat }
    {}

    err_cond(const err_cond& _rhs) noexcept
        : errval_{ _rhs.errval_ }
        , cat_{ _rhs.cat_ }
    {}

    err_cond& operator=(const err_cond& _rhs) noexcept
    {
        errval_ = _rhs.errval_;
        cat_ = _rhs.cat_;
        return *this;
    }

    int value() const noexcept
    {
        return errval_;
    }

    memepp::string message() const noexcept
    {
        return cat_->message(errval_);
    }

    const err_cat* category() const noexcept
    {
        return cat_;
    }

    bool operator==(const err_cond& _rhs) const noexcept
    {
        return errval_ == _rhs.errval_ && cat_ == _rhs.cat_;
    }

    bool operator!=(const err_cond& _rhs) const noexcept
    {
        return !(*this == _rhs);
    }

    int errval_;
    const err_cat* cat_;
};

inline err_cond make_err_cond(int _errval, const err_cat* _cat) noexcept
{
    return err_cond{ _errval, _cat };
}

//! @code
//! class custom_err_cat : public err_cat
//! {
//! public:
//!     const char* name() const noexcept override
//!     {
//!         return "custom";
//!     }
//!
//!     memepp::string message(int _errval) const noexcept override
//!     {
//!         switch (_errval) {
//!         case 1: return "error 1";
//!         case 2: return "error 2";
//!         default: return "unknown error";
//!         }
//!     }
//! };
//!
//! const err_cat* custom_err_cat() noexcept
//! {
//!     static custom_err_cat cat;
//!     return &cat;
//! }
//! 
//! int main() {
//!     auto e = do_something();
//!     if (e) {
//!         std::cout << e.message() << std::endl;
//!     }
//!     
//!     err_cond cond = make_err_cond(1, custom_err_cat());
//!     if (e == cond) {
//!         std::cout << "error 1" << std::endl;
//!     }
//!     return 0;
//! }
//! @endcode
class err
{
public:

    using fninfo = details::fninfo_err::fninfo;

    err ()
        : code_{ MGEC__OK }
        , user_code_{ 0 }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{  }
    {}

    err (int32_t _code)
        : code_{ _code }
        , user_code_{ 0 }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{  }
    {}

    err (int32_t _code, const memepp::string &_message)
        : code_{ _code }
        , user_code_{ 0 }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{ details::make_err(_message) }
    {}

    err (int32_t _code, const memepp::string &_message, const memepp::string &_solution)
        : code_{ _code }
        , user_code_{ 0 }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{ details::make_err(_message, _solution) }
    {}
    
    err (int32_t _code, int32_t _user_code)
        : code_{ _code }
        , user_code_{ _user_code }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{}
    {}

    err (int32_t _code, int32_t _user_code, const memepp::string &_message)
        : code_{ _code }
        , user_code_{ _user_code }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{ details::make_err(_message) }
    {}

    err (int32_t _code, int32_t _user_code, const memepp::string &_message, const memepp::string &_solution)
        : code_{ _code }
        , user_code_{ _user_code }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{ details::make_err(_message, _solution) }
    {}

    err(int32_t _code, err_cat* _cat)
        : code_{ _code }
        , user_code_{ 0 }
        , global_cat_{ _cat }
        , payload_{}
    {}

    err(int32_t _code, err_cat* _cat, const memepp::string& _message)
        : code_{ _code }
        , user_code_{ 0 }
        , global_cat_{ _cat }
        , payload_{ details::make_err(_message) }
    {}

    err(int32_t _code, err_cat* _cat, const memepp::string& _message, const memepp::string& _solution)
        : code_{ _code }
        , user_code_{ 0 }
        , global_cat_{ _cat }
        , payload_{ details::make_err(_message, _solution) }
    {}

    err (const std::shared_ptr<void> &_data)
        : code_{ MGEC__OK }
        , user_code_{ 0 }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{ details::make_err(_data) }
    {}

    err (int32_t code, const std::shared_ptr<void> &_data)
        : code_{ code }
        , user_code_{ 0 }
        , global_cat_{ get_genrc_err_cat() }
        , payload_{ details::make_err(_data) }
    {}

    //err (mgec_t _code, const memepp::string &_message, const std::shared_ptr<void> &_data)
    //    : code_{ _code }
    //    , user_code_{ 0 }
    //    , payload_{ details::make_err(_message, _data) }
    //{}

    //err (mgec_t code, const memepp::string &message, const memepp::string &solution, const std::shared_ptr<void> &data):
    //    impl_{ std::make_shared<detail::err>(code, message, solution, data) }
    //{}

    //err (mgec_t code, int user_code, const std::shared_ptr<void> &data):
    //    impl_{ std::make_shared<detail::err>(code, user_code, data) }
    //{}

    //err (mgec_t code, int user_code, const memepp::string &message, const std::shared_ptr<void> &data):
    //    impl_{ std::make_shared<detail::err>(code, user_code, message, data) }
    //{}

    //err (mgec_t code, int user_code, const memepp::string &message, const memepp::string &solution, const std::shared_ptr<void> &data):
    //    impl_{ std::make_shared<detail::err>(code, user_code, message, solution, data) }
    //{}

    //err (const std::shared_ptr<detail::err> &impl):
    //    impl_{ impl ? impl : std::make_shared<detail::err>() }
    //{}

    // err (const std::shared_ptr<detail::err> &impl, const std::shared_ptr<detail::err> &next):
    //     impl_{ impl }
    // {}

    err (const err &_e)
        : code_{ _e.code_ }
        , user_code_{ _e.user_code_ }
        , global_cat_{ _e.global_cat_ }
        , payload_{ _e.payload_ ? std::make_unique<details::err_payload>(*_e.payload_) : nullptr }
    {}

    err (err &&_e) noexcept
        : code_{ _e.code_ }
        , user_code_{ _e.user_code_ }
        , global_cat_{ _e.global_cat_ }
        , payload_{ std::move(_e.payload_) }
    {}

    err &operator=(const err &_e)
    {
        code_ = _e.code_;
        user_code_ = _e.user_code_;
        global_cat_ = _e.global_cat_;
        payload_ = _e.payload_ ? std::make_unique<details::err_payload>(*_e.payload_) : nullptr;
        return *this;
    }

    err &operator=(err &&_e) noexcept
    {
        code_ = _e.code_;
        user_code_ = _e.user_code_;
        global_cat_ = _e.global_cat_;
        payload_ = std::move(_e.payload_);
        return *this;
    }

    bool operator==(const err& _e) const noexcept
    {
        return category() == _e.category() && code() == _e.code() && usercode() == _e.usercode();
    }
    
    bool operator!=(const err& _e) const noexcept
    {
        return !(*this == _e);
    }

    const char* what() const noexcept;

    constexpr int32_t code() const noexcept;
    constexpr int32_t usercode() const noexcept;
    const memepp::string &message() const noexcept;

    memepp::string solution() const noexcept;

    bool has_funcinfo() const noexcept;
    const fninfo* get_funcinfo() const noexcept;

    std::shared_ptr<void> userdata() const noexcept;
    err copy() const;
    bool has_next() const noexcept;
    err  next()     const;
    void set_next(const err& _next);

    bool ok() const noexcept;
    explicit operator bool() const noexcept;

    void set_message (const memepp::string &message);
    void set_solution(const memepp::string &solution);

    void set_funcinfo(const fninfo&info);
    void set_userdata(const std::shared_ptr<void> &data);

    err_cond get_err_cond() const noexcept;
    inline const err_cat* category() const noexcept { return global_cat_; }

    inline static err make_ok()
    {
        static err e;
        return e;
    }

    inline static err make_unknown()
    {
        static err e { MGEC__ERR, "unknown" };
        return e;
    }
private:
    details::err_payload& ensure_payload() {
        if (!payload_) {
            payload_ = std::make_unique<details::err_payload>();
        }
        return *payload_;
    }

    int32_t  code_;
    int32_t  user_code_;
    const err_cat* global_cat_;
    std::unique_ptr<details::err_payload> payload_;
};

inline const char* err::what() const noexcept
{
    return message().data();
}

inline constexpr int32_t err::code() const noexcept
{
    return code_;
}

inline constexpr int32_t err::usercode() const noexcept
{
    return user_code_;
}

inline const memepp::string &err::message() const noexcept
{
    return details::err_get_message(payload_.get());
}

inline memepp::string err::solution() const noexcept
{
    return details::err_get_solution(payload_.get());
}

inline bool err::has_funcinfo() const noexcept
{
    return details::err_get_fninfo(payload_.get()) != nullptr;
}

inline const err::fninfo* err::get_funcinfo() const noexcept
{
    return details::err_get_fninfo(payload_.get());
}

inline std::shared_ptr<void> err::userdata() const noexcept
{
    return details::err_get_userdata(payload_.get());
}

inline err err::copy() const
{
    return err{ *this };
}
    
inline bool err::ok() const noexcept
{
    return code() == MGEC__OK;
}

inline err::operator bool() const noexcept
{
    return !ok();
}

inline void err::set_message(const memepp::string &_message)
{
    ensure_payload().message_ = _message;
}

inline void err::set_solution(const memepp::string &_solution)
{
    ensure_payload().solution_ = std::make_unique<memepp::string>(_solution);
}

inline void err::set_funcinfo(const fninfo &info)
{
    ensure_payload().fninfo_ = std::make_unique<fninfo>(info);
}

inline void err::set_userdata(const std::shared_ptr<void> &data)
{
    ensure_payload().userdata_ = data;
}

inline bool err::has_next() const noexcept
{
    return payload_ && payload_->next_ != nullptr;
}

inline err err::next() const
{
    if (!payload_ || !payload_->next_)
        return make_ok();
    return *payload_->next_;
}

inline void err::set_next(const err& _next)
{
    ensure_payload().next_ = std::make_shared<err>(_next);
}


inline err_cond err_cat::get_err_cond(int _errval) const noexcept
{
    return err_cond{ _errval, this };
}

inline bool err_cat::equivalent(int _errval, const err_cond& _cond) const noexcept
{
    return get_err_cond(_errval) == _cond;
}

inline bool err_cat::equivalent(const err& _code, int _errval) const noexcept
{
    return _code.code() == _errval && _code.category() == this;
}

inline bool operator==(const err& _lhs, const err_cond& _rhs) noexcept
{
    return _lhs.category() == _rhs.category() && _lhs.code() == _rhs.value();
}

inline bool operator==(const err_cond& _lhs, const err& _rhs) noexcept
{
    return _rhs == _lhs;
}

inline bool operator!=(const err& _lhs, const err_cond& _rhs) noexcept
{
    return !(_lhs == _rhs);
}

inline bool operator!=(const err_cond& _lhs, const err& _rhs) noexcept
{
    return !(_lhs == _rhs);
}

}; // namespace mgpp

#endif // !MEGOPP_ERR_HPP_INCLUDED
