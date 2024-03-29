
#ifndef MEGOPP_UTIL_SIMPLE_COUNTER_H_INCLUDED
#define MEGOPP_UTIL_SIMPLE_COUNTER_H_INCLUDED

#include <megopp/help/null_mutex.h>
#include <megopp/util/scope_cleanup.h>
#include <megopp/util/scope_locker.h>

#include <meme/string_fwd.h>

#include <memory>
#include <functional>
#include <mutex>

namespace mgpp {
namespace util {

	template<typename _Mtx = megopp::help::null_mutex>
	struct ref_counter
	{
		ref_counter() : count_(0), compare_value_(0) {}
		ref_counter(mmint_t _count):
			count_(_count)
		{}

        ref_counter(mmint_t _count, mmint_t _compare_value):
            count_(_count),
            compare_value_(_compare_value)
        {}

        inline mmint_t count(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_;
        }

        inline mmint_t count(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_;
        }

        inline mmint_t count(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_;
        }

        inline mmint_t compare_value(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return compare_value_;
        }

        inline mmint_t compare_value(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return compare_value_;
        }

        inline mmint_t compare_value(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return compare_value_;
        }

        inline bool is_equal(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_ == compare_value_;
        }

        inline bool is_equal(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_ == compare_value_;
        }

        inline bool is_equal(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_ == compare_value_;
        }

        inline bool is_greater(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_ > compare_value_;
        }

        inline bool is_greater(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_ > compare_value_;
        }

        inline bool is_greater(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_ > compare_value_;
        }

        inline bool is_less(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_ < compare_value_;
        }

        inline bool is_less(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_ < compare_value_;
        }

        inline bool is_less(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_ < compare_value_;
        }

        inline bool is_greater_or_equal(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_ >= compare_value_;
        }

        inline bool is_greater_or_equal(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_ >= compare_value_;
        }

        inline bool is_greater_or_equal(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_ >= compare_value_;
        }

        inline bool is_less_or_equal(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_ <= compare_value_;
        }

        inline bool is_less_or_equal(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_ <= compare_value_;
        }

        inline bool is_less_or_equal(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_ <= compare_value_;
        }

        inline bool is_not_equal(_Mtx& _mtx) const
        {
            std::unique_lock<_Mtx> locker(_mtx);
            return count_ != compare_value_;
        }

        inline bool is_not_equal(std::unique_lock<_Mtx>& _locker) const
        {
            scope_unique_locker<_Mtx> locker(_locker);
            return count_ != compare_value_;
        }

        inline bool is_not_equal(std::shared_lock<_Mtx>& _locker) const
        {
            scope_shared_locker<_Mtx> locker(_locker);
            return count_ != compare_value_;
        }

        inline void set_count(_Mtx& _mtx, mmint_t _count)
        {
            std::unique_lock<_Mtx> locker(_mtx);
            count_ = _count;
        }

        inline void set_count(std::unique_lock<_Mtx>& _locker, mmint_t _count)
        {
            scope_unique_locker<_Mtx> locker(_locker);
            count_ = _count;
        }

        inline void set_compare_value(_Mtx& _mtx, mmint_t _compare_value)
        {
            std::unique_lock<_Mtx> locker(_mtx);
            compare_value_ = _compare_value;
        }

        inline void set_compare_value(std::unique_lock<_Mtx>& _locker, mmint_t _compare_value)
        {
            scope_unique_locker<_Mtx> locker(_locker);
            compare_value_ = _compare_value;
        }

        inline void set_callback(_Mtx& _mtx, const std::function<void(const ref_counter&)>& _cb)
        {
            std::unique_lock<_Mtx> locker(_mtx);
            if (_cb) {
                cb_ = std::make_shared< std::function<void(const ref_counter&)> >(_cb);
            }
        }

        inline void set_callback(std::unique_lock<_Mtx>& _locker, const std::function<void(const ref_counter&)>& _cb)
        {
            scope_unique_locker<_Mtx> locker(_locker);
            if (_cb) {
                cb_ = std::make_shared< std::function<void(const ref_counter&)> >(_cb);
            }
        }
		
		inline ref_counter& increment(_Mtx& _mtx)
		{
			std::unique_lock<_Mtx> locker(_mtx);
			auto count = count_++;
                
            if (count < compare_value_ && count_ == compare_value_) 
            {
                auto cb = cb_;
                locker.unlock();
    
                if (cb)
                    (*cb)(*this);
            }
			return *this;
		}

		inline ref_counter& increment(std::unique_lock<_Mtx>& _locker)
		{
            scope_unique_locker<_Mtx> locker(_locker);

			auto count = count_++;
            
            if (count < compare_value_ && count_ == compare_value_) 
            {
                auto cb = cb_;
                locker.unlock();

                if (cb)
                    (*cb)(*this);
            }
			return *this;
		}

		inline ref_counter& decrement(_Mtx& _mtx)
		{
			std::unique_lock<_Mtx> locker(_mtx);
            auto count = count_--;

			if (count > compare_value_ && count_ == compare_value_) 
			{
                auto cb = cb_;
				locker.unlock();

				if (cb)
					(*cb)(*this);
			}
			return *this;
		}

		inline ref_counter& decrement(std::unique_lock<_Mtx>& _locker)
		{
            scope_unique_locker<_Mtx> locker(_locker);

			auto count = count_--;

			if (count > compare_value_ && count_ == compare_value_) 
			{
                auto cb = cb_;
				locker.unlock();

				if (cb)
					(*cb)(*this);
			}
			return *this;
		}

        // inline ref_counter& operator++()
        // {
        //     auto mtx = megopp::auxiliary::null_mutex{};
        //     return increment(mtx);
        // }

        // inline ref_counter& operator--()
        // {
        //     auto mtx = megopp::auxiliary::null_mutex{};
        //     return decrement(mtx);
        // }



    private:

		mmint_t count_;
		mmint_t compare_value_;
		std::shared_ptr< std::function<void(const ref_counter&)> > cb_;
	};

}; // namespace util
}; // namespace mgpp

#endif // !MEGOPP_UTIL_SIMPLE_COUNTER_H_INCLUDED
