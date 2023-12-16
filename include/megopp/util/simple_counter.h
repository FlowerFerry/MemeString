
#ifndef MEGOPP_UTIL_SIMPLE_COUNTER_H_INCLUDED
#define MEGOPP_UTIL_SIMPLE_COUNTER_H_INCLUDED

#include <megopp/help/null_mutex.h>
#include <megopp/util/scope_cleanup.h>

#include <memory>
#include <functional>
#include <mutex>

namespace mgpp {
namespace util {

	template<typename _Mtx = megopp::auxiliary::null_mutex>
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

        inline mmint_t count() const
        {
            return count_;
        }

        inline mmint_t compare_value() const
        {
            return compare_value_;
        }

        inline bool is_equal() const
        {
            return count_ == compare_value_;
        }

        inline bool is_greater() const
        {
            return count_ > compare_value_;
        }

        inline bool is_less() const
        {
            return count_ < compare_value_;
        }

        inline bool is_greater_or_equal() const
        {
            return count_ >= compare_value_;
        }

        inline bool is_less_or_equal() const
        {
            return count_ <= compare_value_;
        }

        inline bool is_not_equal() const
        {
            return count_ != compare_value_;
        }

        inline void set_count(mmint_t _count)
        {
            count_ = _count;
        }

        inline void set_compare_value(mmint_t _compare_value)
        {
            compare_value_ = _compare_value;
        }

        inline void set_callback(const std::function<void(const ref_counter&)>& _cb)
        {
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
                locker.unlock();
    
                if (cb_)
                    (*cb_)(*this);
            }
			return *this;
		}

		inline ref_counter& increment(std::unique_lock<_Mtx>& _locker)
		{
			auto owns = _locker.owns_lock();
			auto cleanup = megopp::util::scope_cleanup__create([&]
			{
                if (owns && owns != _locker.owns_lock())
                    _locker.lock();
			});

			if (!owns)
				_locker.lock();

			auto count = count_++;
            
            if (count < compare_value_ && count_ == compare_value_) 
            {
                _locker.unlock();

                if (cb_)
                    (*cb_)(*this);
            }
			return *this;
		}

		inline ref_counter& decrement(_Mtx& _mtx)
		{
			std::unique_lock<_Mtx> locker(_mtx);
            auto count = count_--;

			if (count > compare_value_ && count_ == compare_value_) 
			{
				locker.unlock();

				if (cb_)
					(*cb_)(*this);
			}
			return *this;
		}

		inline ref_counter& decrement(std::unique_lock<_Mtx>& _locker)
		{
			auto owns = _locker.owns_lock();
			auto cleanup = megopp::util::scope_cleanup__create([&]
			{
                if (owns && owns != _locker.owns_lock())
                    _locker.lock();
			});

			if (!owns)
				_locker.lock();

			auto count = count_--;

			if (count > compare_value_ && count_ == compare_value_) 
			{
				_locker.unlock();

				if (cb_)
					(*cb_)(*this);
			}
			return *this;
		}

        inline ref_counter& operator++()
        {
            auto mtx = megopp::auxiliary::null_mutex{};
            return increment(mtx);
        }

        inline ref_counter& operator--()
        {
            auto mtx = megopp::auxiliary::null_mutex{};
            return decrement(mtx);
        }



    private:

		mmint_t count_;
		mmint_t compare_value_;
		std::shared_ptr< std::function<void(const ref_counter&)> > cb_;
	};

}; // namespace util
}; // namespace mgpp

#endif // !MEGOPP_UTIL_SIMPLE_COUNTER_H_INCLUDED
