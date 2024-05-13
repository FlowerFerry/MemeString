
#ifndef MEGOPP_ERR_HPP_INCLUDED
#define MEGOPP_ERR_HPP_INCLUDED


#include <mego/err/ec.h>
#include <memepp/string.hpp>
#include <megopp/help/singleton.h>

#include <exception>
#include <memory>
#include <deque>
#include <map>

namespace mgpp {
namespace details {

    struct basic_err
    {
        enum class typ {
            none,
            desc,
            sln,
            fninfo,
            fninfo_and_sln,
            list,
            full,
            user
        };

        virtual ~basic_err() = default;

        virtual typ type() const noexcept { return typ::none; }
        
        virtual std::unique_ptr<basic_err> clone() const = 0;
        
    };
    
    struct desc_err : public basic_err
    {
        desc_err() = default;
        desc_err(const memepp::string& _msg) : message_(_msg) {}

        virtual ~desc_err() = default;

        virtual typ type() const noexcept { return typ::desc; }
        
        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<desc_err>(*this);
        }

        memepp::string message_;
    };
    
    struct sln_err : public desc_err
    {
        sln_err() = default;
        sln_err(const memepp::string& _msg) 
            : desc_err(_msg) 
        {}

        sln_err(const memepp::string& _msg, const memepp::string& _sln) 
            : desc_err(_msg)
            , solution_(_sln) 
        {}

        virtual ~sln_err() = default;

        virtual typ type() const noexcept { return typ::sln; }
        
        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<sln_err>(*this);
        }

        memepp::string solution_;
    };

    struct fninfo_err : public basic_err
    {
        struct fninfo
        {
            memepp::string name_;
            std::map<int, memepp::string> args_;
        };

        fninfo_err() = default;
        fninfo_err(const fninfo& _fninfo) : fninfo_(_fninfo) {}

        virtual ~fninfo_err() = default;

        virtual typ type() const noexcept { return typ::fninfo; }
        
        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<fninfo_err>(*this);
        }

        fninfo fninfo_;
    };
    
    struct fninfo_and_sln_err : public fninfo_err
    {
        fninfo_and_sln_err() = default;
        fninfo_and_sln_err(const memepp::string& _msg)
            : message_(_msg)
        {}

        fninfo_and_sln_err(const memepp::string& _msg, const memepp::string& _sln)
            : message_(_msg)
            , solution_(_sln)
        {}

        fninfo_and_sln_err(const memepp::string& _msg, const fninfo_err::fninfo& _fninfo)
            : fninfo_err(_fninfo)
            , message_(_msg)
        {}

        fninfo_and_sln_err(const memepp::string& _msg, const memepp::string& _sln, const fninfo_err::fninfo& _fninfo)
            : fninfo_err(_fninfo)
            , message_(_msg)
            , solution_(_sln)
        {}
        

        virtual ~fninfo_and_sln_err() = default;

        virtual typ type() const noexcept { return typ::fninfo_and_sln; }

        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<fninfo_and_sln_err>(*this);
        }

        memepp::string message_;
        memepp::string solution_;
    };

    struct list_err : public basic_err
    {
        list_err() = default;
        list_err(std::unique_ptr<basic_err>&& _err)
        {
            list_.push_back(std::move(_err));
        }
        
        list_err(const list_err& _e)
        {
            for (auto& item : _e.list_)
            {
                list_.push_back(item->clone());
            }
        }

        list_err(list_err&& _e) noexcept
            : list_(std::move(_e.list_))
        {
        }
        
        virtual ~list_err() = default;

        virtual typ type() const noexcept { return typ::list; }

        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<list_err>(*this);
        }

        std::deque<std::unique_ptr<basic_err>> list_;
    };

    struct full_err : public fninfo_and_sln_err
    {
        full_err() = default;

        full_err(const full_err& _e)
            : fninfo_and_sln_err(_e)
        {
            for (auto& item : _e.list_)
            {
                list_.push_back(item->clone());
            }
        }

        full_err(const memepp::string& _msg)
            : fninfo_and_sln_err(_msg)
        {}

        full_err(const memepp::string& _msg, const memepp::string& _sln)
            : fninfo_and_sln_err(_msg, _sln)
        {}

        full_err(const memepp::string& _msg, const fninfo_err::fninfo& _fninfo)
            : fninfo_and_sln_err(_msg, _fninfo)
        {}

        full_err(const memepp::string& _msg, const memepp::string& _sln, const fninfo_err::fninfo& _fninfo)
            : fninfo_and_sln_err(_msg, _sln, _fninfo)
        {}

        full_err(std::unique_ptr<basic_err>&& _err)
        {
            list_.push_back(std::move(_err));
        }
        
        virtual ~full_err() = default;

        virtual typ type() const noexcept { return typ::full; }

        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<full_err>(*this);
        }

        std::deque<std::unique_ptr<basic_err>> list_;
        std::shared_ptr<void> data_;
    };

    struct user_err : public basic_err
    {
        user_err() = default;
        user_err(const std::shared_ptr<void>& _data)
            : data_(_data)
        {}

        virtual ~user_err() = default;

        virtual typ type() const noexcept { return typ::user; }

        virtual std::unique_ptr<basic_err> clone() const override
        {
            return std::make_unique<user_err>(*this);
        }

        std::shared_ptr<void> data_;
    };

    inline std::unique_ptr<basic_err> make_err(const memepp::string& _msg)
    {
        return std::make_unique<desc_err>(_msg);
    }
    
    inline std::unique_ptr<basic_err> make_err(const memepp::string& _msg, const memepp::string& _sln)
    {
        return std::make_unique<sln_err>(_msg, _sln);
    }
    
    inline std::unique_ptr<basic_err> make_err(const fninfo_err::fninfo& _fninfo)
    {
        return std::make_unique<fninfo_err>(_fninfo);
    }

    inline std::unique_ptr<basic_err> make_err(const memepp::string& _msg, const fninfo_err::fninfo& _fninfo)
    {
        return std::make_unique<fninfo_and_sln_err>(_msg, _fninfo);
    }
    
    inline std::unique_ptr<basic_err> make_err(
        const memepp::string& _msg, const memepp::string& _sln, const fninfo_err::fninfo& _fninfo)
    {
        return std::make_unique<fninfo_and_sln_err>(_msg, _sln, _fninfo);
    }
    
    inline std::unique_ptr<basic_err> make_err(std::unique_ptr<basic_err>&& _err)
    {
        return std::make_unique<list_err>(std::move(_err));
    }
    
    inline std::unique_ptr<basic_err> make_err(const std::shared_ptr<void>& _data)
    {
        return std::make_unique<user_err>(_data);
    }
    
    inline const memepp::string& err_get_message(const basic_err* _e)
    {
        if (!_e)
           return mgpp::help::singleton<memepp::string>::instance();
        
        switch (_e->type()) {
        case basic_err::typ::desc:
            return dynamic_cast<const desc_err*>(_e)->message_;
        case basic_err::typ::sln:
            return dynamic_cast<const sln_err*>(_e)->message_;
        case basic_err::typ::fninfo_and_sln:
            return dynamic_cast<const fninfo_and_sln_err*>(_e)->message_;
        case basic_err::typ::full:
            return dynamic_cast<const full_err*>(_e)->message_;
        default:
            return mgpp::help::singleton<memepp::string>::instance();
        }
    }

    inline const memepp::string& err_get_solution(const basic_err* _e)
    {
        if (!_e)
            return mgpp::help::singleton<memepp::string>::instance();

        switch (_e->type()) {
        case basic_err::typ::sln:
            return dynamic_cast<const sln_err*>(_e)->solution_;
        case basic_err::typ::fninfo_and_sln:
            return dynamic_cast<const fninfo_and_sln_err*>(_e)->solution_;
        case basic_err::typ::full:
            return dynamic_cast<const full_err*>(_e)->solution_;
        default:
            return mgpp::help::singleton<memepp::string>::instance();
        }
    }

    inline const fninfo_err::fninfo* err_get_fninfo(const basic_err* _e)
    {
        if (!_e)
            return nullptr;

        switch (_e->type()) {
        case basic_err::typ::fninfo:
        case basic_err::typ::fninfo_and_sln:
            return &dynamic_cast<const fninfo_err*>(_e)->fninfo_;
        default:
            return nullptr;
        }
    }

    inline std::shared_ptr<void> err_get_userdata(const basic_err* _e)
    {
        if (!_e)
            return nullptr;

        switch (_e->type()) {
        case basic_err::typ::user:
            return dynamic_cast<const user_err*>(_e)->data_;
        default:
            return nullptr;
        }
    }

    inline void err_set_message(std::unique_ptr<basic_err>& _e, const memepp::string& _msg)
    {
        if (!_e)
            return;

        switch (_e->type()) {
        case basic_err::typ::desc:
            dynamic_cast<desc_err*>(_e.get())->message_ = _msg;
            break;
        case basic_err::typ::sln:
            dynamic_cast<sln_err*>(_e.get())->message_ = _msg;
            break;
        case basic_err::typ::fninfo:
            _e = make_err(_msg, dynamic_cast<fninfo_err*>(_e.get())->fninfo_);
            break;
        case basic_err::typ::fninfo_and_sln:
            dynamic_cast<fninfo_and_sln_err*>(_e.get())->message_ = _msg;
            break;
        case basic_err::typ::full:
            dynamic_cast<full_err*>(_e.get())->message_ = _msg;
            break;
        default:
            break;
        }
    }

    inline void err_set_solution(std::unique_ptr<basic_err>& _e, const memepp::string& _sln)
    {
        if (!_e)
            return;

        switch (_e->type()) {
        case basic_err::typ::desc:
            _e = make_err({}, _sln);
            break;
        case basic_err::typ::sln:
            dynamic_cast<sln_err*>(_e.get())->solution_ = _sln;
            break;
        case basic_err::typ::fninfo:
            _e = make_err({}, _sln, dynamic_cast<fninfo_err*>(_e.get())->fninfo_);
            break;
        case basic_err::typ::fninfo_and_sln:
            dynamic_cast<fninfo_and_sln_err*>(_e.get())->solution_ = _sln;
            break;
        case basic_err::typ::full:
            dynamic_cast<full_err*>(_e.get())->solution_ = _sln;
            break;
        default:
            break;
        }
    }

    inline void err_set_funcinfo(std::unique_ptr<basic_err>& _e, const fninfo_err::fninfo& _fninfo)
    {
        if (!_e)
            return;

        switch (_e->type()) {
        case basic_err::typ::desc:
            _e = make_err(dynamic_cast<desc_err*>(_e.get())->message_, _fninfo);
            break;
        case basic_err::typ::sln:
        {
            auto p = dynamic_cast<sln_err*>(_e.get());
            _e = make_err(p->message_, p->solution_, _fninfo);
        } break;
        case basic_err::typ::fninfo:
            dynamic_cast<fninfo_err*>(_e.get())->fninfo_ = _fninfo;
            break;
        case basic_err::typ::fninfo_and_sln:
            dynamic_cast<fninfo_and_sln_err*>(_e.get())->fninfo_ = _fninfo;
            break;
        case basic_err::typ::full:
            dynamic_cast<full_err*>(_e.get())->fninfo_ = _fninfo;
            break;
        default:
            break;
        }
    }

    inline void err_set_userdata(std::unique_ptr<basic_err>& _e, const std::shared_ptr<void>& _data)
    {
        if (!_e)
            return;

        switch (_e->type()) {
        case basic_err::typ::user:
            dynamic_cast<user_err*>(_e.get())->data_ = _data;
            break;
        case basic_err::typ::full:
            dynamic_cast<full_err*>(_e.get())->data_ = _data;
            break;
        default:
            break;
        }
    }

//struct err : public basic_err
//{
//    err():
//        code_{ MGEC__OK },
//        user_code_{ 0 }
//    {}
//
//    err(mgec_t code):
//        code_{ code },
//        user_code_{ 0 }
//    {}
//
//    err(mgec_t code, const memepp::string &message):
//        code_{ code },
//        user_code_{ 0 },
//        message_{ message }
//    {}
//
//    err(mgec_t code, const memepp::string &message, const memepp::string &solution):
//        code_{ code },
//        user_code_{ 0 },
//        message_{ message },
//        solution_{ std::make_unique<memepp::string>(solution) }
//    {}
//    
//    err(mgec_t code, int user_code):
//        code_{ code },
//        user_code_{ user_code }
//    {}
//
//    err(mgec_t code, int user_code, const memepp::string &message):
//        code_{ code },
//        user_code_{ user_code },
//        message_{ message }
//    {}
//
//    err(mgec_t code, int user_code, const memepp::string &message, const memepp::string &solution):
//        code_{ code },
//        user_code_{ user_code },
//        message_{ message },
//        solution_{ std::make_unique<memepp::string>(solution) }
//    {}
//
//    err(const std::shared_ptr<void> &data):
//        code_{ MGEC__OK },
//        user_code_{ 0 },
//        data_{ data }
//    {}
//
//    err(mgec_t code, const std::shared_ptr<void> &data):
//        code_{ code },
//        user_code_{ 0 },
//        data_{ data }
//    {}
//
//    err(mgec_t code, const memepp::string &message, const std::shared_ptr<void> &data):
//        code_{ code },
//        user_code_{ 0 },
//        message_{ message },
//        data_{ data }
//    {}
//
//    err(mgec_t code, const memepp::string &message, const memepp::string &solution, const std::shared_ptr<void> &data):
//        code_{ code },
//        user_code_{ 0 },
//        message_{ message },
//        solution_{ std::make_unique<memepp::string>(solution) },
//        data_{ data }
//    {}
//
//    err(mgec_t code, int user_code, const std::shared_ptr<void> &data):
//        code_{ code },
//        user_code_{ user_code },
//        data_{ data }
//    {}
//
//    err(mgec_t code, int user_code, const memepp::string &message, const std::shared_ptr<void> &data):
//        code_{ code },
//        user_code_{ user_code },
//        message_{ message },
//        data_{ data }
//    {}
//
//    err(mgec_t code, int user_code, const memepp::string &message, const memepp::string &solution, const std::shared_ptr<void> &data):
//        code_{ code },
//        user_code_{ user_code },
//        message_{ message },
//        solution_{ std::make_unique<memepp::string>(solution) },
//        data_{ data }
//    {}
//
//    err(const err &e):
//        code_{ e.code_ },
//        user_code_{ e.user_code_ },
//        message_{ e.message_ },
//        solution_{ e.solution_ ? std::make_unique<memepp::string>(*e.solution_) : nullptr },
//        data_{ e.data_ },
//        func_info_{ e.func_info_ ? std::make_unique<func_info>(*e.func_info_) : nullptr }
//    {
//    }
//
//    err(err &&e) noexcept:
//        code_{ e.code_ },
//        user_code_{ e.user_code_ },
//        message_{ std::move(e.message_) },
//        solution_{ std::move(e.solution_) },
//        data_{ std::move(e.data_) },
//        func_info_{ std::move(e.func_info_) }
//    {
//    }
//
//    err &operator=(const err &e)
//    {
//        code_       = e.code_;
//        user_code_  = e.user_code_;
//        message_    = e.message_;
//        solution_   = (e.solution_ ? std::make_unique<memepp::string>(*e.solution_) : nullptr);
//        data_       = e.data_;
//        func_info_  = (e.func_info_ ? std::make_unique<func_info>(*e.func_info_) : nullptr);
//        return *this;
//    }
//
//    err &operator=(err &&e) noexcept
//    {
//        code_       = e.code_;
//        user_code_  = e.user_code_;
//        message_    = std::move(e.message_);
//        solution_   = std::move(e.solution_);
//        data_       = std::move(e.data_);
//        func_info_  = std::move(e.func_info_);
//        return *this;
//    }
//
//    ~err() {
//    }
//    
//    memepp::string message_;
//    std::unique_ptr<memepp::string> solution_;
//    std::unique_ptr<func_info> func_info_;
//
//    std::shared_ptr<detail::err> next_;
//    std::shared_ptr<void> data_;
//};

}; // namespace detail  

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

const err_cat* generic_err_cat() noexcept;

class err_cond
{
public:
    err_cond() noexcept
        : errval_{ 0 }
        , cat_{ generic_err_cat() }
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

    int errval_;
    const err_cat* cat_;
};

err_cond make_err_cond(int _errval, const err_cat* _cat) noexcept;

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
        , err_{  }
    {}

    err (mgec_t _code)
        : code_{ _code }
        , user_code_{ 0 }
        , err_{  }
    {}

    err (mgec_t _code, const memepp::string &_message)
        : code_{ _code }
        , user_code_{ 0 }
        , err_{ details::make_err(_message) }
    {}

    err (mgec_t _code, const memepp::string &_message, const memepp::string &_solution)
        : code_{ _code }
        , user_code_{ 0 }
        , err_{ details::make_err(_message, _solution) }
    {}
    
    err (mgec_t _code, int32_t _user_code)
        : code_{ _code }
        , user_code_{ _user_code }
        , err_{}
    {}

    err (mgec_t _code, int32_t _user_code, const memepp::string &_message)
        : code_{ _code }
        , user_code_{ _user_code }
        , err_{ details::make_err(_message) }
    {}

    err (mgec_t _code, int32_t _user_code, const memepp::string &_message, const memepp::string &_solution)
        : code_{ _code }
        , user_code_{ _user_code }
        , err_{ details::make_err(_message, _solution) }
    {}

    err (const std::shared_ptr<void> &_data)
        : code_{ MGEC__OK }
        , user_code_{ 0 }
        , err_{ details::make_err(_data) }
    {}

    err (mgec_t code, const std::shared_ptr<void> &_data)
        : code_{ code }
        , user_code_{ 0 }
        , err_{ details::make_err(_data) }
    {}

    //err (mgec_t _code, const memepp::string &_message, const std::shared_ptr<void> &_data)
    //    : code_{ _code }
    //    , user_code_{ 0 }
    //    , err_{ details::make_err(_message, _data) }
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
        , err_{ _e.err_ ? _e.err_->clone() : nullptr }
    {}

    err (err &&e) noexcept
        : code_{ e.code_ }
        , user_code_{ e.user_code_ }
        , err_{ std::move(e.err_) }
    {}

    err &operator=(const err &_e)
    {
        code_ = _e.code_;
        user_code_ = _e.user_code_;
        err_ = _e.err_ ? _e.err_->clone() : nullptr;
        return *this;
    }

    err &operator=(err &&e) noexcept
    {
        code_ = e.code_;
        user_code_ = e.user_code_;
        err_ = std::move(e.err_);
        return *this;
    }

    const char* what() const noexcept;

    constexpr mgec_t code() const noexcept;
    constexpr mmint_t usercode() const noexcept;
    const memepp::string &message() const noexcept;

    memepp::string solution() const noexcept;

    bool has_funcinfo() const noexcept;
    const fninfo* get_funcinfo() const noexcept;

    std::shared_ptr<void> userdata() const noexcept;
    err copy() const;
    err next() const;

    bool ok() const noexcept;
    explicit operator bool() const noexcept;

    void set_last(const err &e);
    void set_message(const memepp::string &message);
    void set_solution(const memepp::string &solution);

    void set_funcinfo(const fninfo&info);
    void set_userdata(const std::shared_ptr<void> &data);

    err_cond get_err_cond() const noexcept;
    inline err_cat* category() const noexcept { return global_cat_; }

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
    int32_t code_;
    int32_t user_code_;
    err_cat* global_cat_;
    std::unique_ptr<details::basic_err> err_;
};

inline const char* err::what() const noexcept
{
    return message().data();
}

inline constexpr mgec_t err::code() const noexcept
{
    return code_;
}

inline constexpr mmint_t err::usercode() const noexcept
{
    return user_code_;
}

inline const memepp::string &err::message() const noexcept
{
    return details::err_get_message(err_.get());
}

inline memepp::string err::solution() const noexcept
{
    return details::err_get_solution(err_.get());
}

inline bool err::has_funcinfo() const noexcept
{
    return details::err_get_fninfo(err_.get()) != nullptr;
}

inline const err::fninfo* err::get_funcinfo() const noexcept
{
    return details::err_get_fninfo(err_.get());
}

inline std::shared_ptr<void> err::userdata() const noexcept
{
    return details::err_get_userdata(err_.get());
}

inline err err::copy() const
{
    return err{ *this };
}
    
    //inline err err::next() const
    //{
    //    auto next = impl_->next_;
    //    return next ? err{ next } : make_ok();
    //}

inline bool err::ok() const noexcept
{
    return code() == MGEC__OK;
}

inline err::operator bool() const noexcept
{
    return !ok();
}

    //inline void err::set_last(const err &e)
    //{
    //    if (!e) {
    //        return;
    //    }

    //    auto inner = impl_;
    //    if (inner == e.impl_) {
    //        return;
    //    }

    //    if (ok()) {
    //        *this = e;
    //        return;
    //    }

    //    while (inner->next_) {
    //        if (inner->next_ == e.impl_) 
    //            return;
    //        
    //        inner = inner->next_;
    //    }
    //    inner->next_ = e.impl_;
    //}

inline void err::set_message(const memepp::string &_message)
{
    details::err_set_message(err_, _message);
}

inline void err::set_solution(const memepp::string &_solution)
{
    details::err_set_solution(err_, _solution);
}

inline void err::set_funcinfo(const fninfo &info)
{
    details::err_set_funcinfo(err_, info);
}

inline void err::set_userdata(const std::shared_ptr<void> &data)
{
    details::err_set_userdata(err_, data);
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

}; // namespace mgpp

#endif // !MEGOPP_ERR_HPP_INCLUDED
