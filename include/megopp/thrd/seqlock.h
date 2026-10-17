#ifndef MEGOPP_THRD_SEQLOCK_H_INCLUDED
#define MEGOPP_THRD_SEQLOCK_H_INCLUDED

#include <mego/predef/architecture/x86.h>
#include <mego/predef/architecture/arm.h>

#include <megopp/help/null_mutex.h>

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>

#if MEGO_ARCH__X86 || MEGO_ARCH__X64
#  include <immintrin.h>
#endif

namespace mgpp {
namespace thrd {

namespace seqlock_details {

    MG_INLINE void cpu_relax() noexcept
    {
#if MEGO_ARCH__X86 || MEGO_ARCH__X64
        _mm_pause();
#elif MEGO_ARCH__ARM || MEGO_ARCH__ARM64
#  if MG_COMP__GCC_AVAIL || MG_COMP__CLANG_AVAIL
        __asm__ __volatile__("yield" ::: "memory");
#  endif
#endif
    }

    // Prefer a lock-free 64-bit atomic as the sequence word, and fall back
    // to 32-bit on platforms where 64-bit atomics are not lock-free.
    using default_word_t =
        std::conditional<std::atomic<std::uint64_t>::is_always_lock_free,
                         std::uint64_t, std::uint32_t>::type;

    template <typename _Ty, typename _Word>
    struct word_traits
    {
        static constexpr std::size_t count =
            (sizeof(_Ty) + sizeof(_Word) - 1) / sizeof(_Word);

        using buffer_t = std::array<_Word, count>;
    };

} // namespace seqlock_details

// Seqlock for single-writer / multi-reader scenarios.
//
// Writers are serialized by _Mutex, so a reader never blocks a writer and
// never takes a lock. When exactly one writer is guaranteed, pass
// mgpp::help::null_mutex as _Mutex to strip out the locking overhead.
template <
    typename _Ty,
    typename _Mutex = std::mutex,
    typename _Word = seqlock_details::default_word_t
>
class seqlock
{
    static_assert(std::is_trivially_copyable<_Ty>::value,
        "seqlock<_Ty>: _Ty must be trivially copyable");
    static_assert(std::is_default_constructible<_Ty>::value,
        "seqlock<_Ty>: _Ty must be default constructible");
    static_assert(std::is_unsigned<_Word>::value,
        "seqlock<_Ty>: _Word must be an unsigned integer");
    static_assert(std::atomic<_Word>::is_always_lock_free,
        "seqlock<_Ty>: std::atomic<_Word> must be lock-free");

public:
    using value_type = _Ty;
    using word_type = _Word;
    using seq_type = std::uint32_t;
    using mutex_type = _Mutex;

    static constexpr std::size_t word_count =
        seqlock_details::word_traits<_Ty, _Word>::count;

private:
    using buffer_type = typename seqlock_details::word_traits<_Ty, _Word>::buffer_t;

    std::atomic<seq_type> seq_;
    std::array<std::atomic<_Word>, word_count> words_;
    mutex_type mutex_;

    static buffer_type to_words(const _Ty& _value) noexcept
    {
        buffer_type buffer = {};                    // zero-fill trailing padding
        std::memcpy(buffer.data(), &_value, sizeof(_Ty));
        return buffer;
    }

    static _Ty from_words(const buffer_type& _buffer) noexcept
    {
        _Ty value = {};
        std::memcpy(&value, _buffer.data(), sizeof(_Ty));
        return value;
    }

    // The caller must already hold mutex_.
    void store_locked(const buffer_type& _buffer) noexcept
    {
        const seq_type s = seq_.load(std::memory_order_relaxed);

        // Odd sequence: a write is in progress.
        seq_.store(s + 1, std::memory_order_relaxed);
        // Make the odd sequence visible before the payload is published.
        std::atomic_thread_fence(std::memory_order_release);

        for (std::size_t i = 0; i < word_count; ++i)
            words_[i].store(_buffer[i], std::memory_order_relaxed);

        // Even sequence: the write is complete.
        seq_.store(s + 2, std::memory_order_release);
    }

    // The caller must already hold mutex_, so there is no concurrent writer.
    buffer_type read_locked() const noexcept
    {
        buffer_type buffer = {};
        for (std::size_t i = 0; i < word_count; ++i)
            buffer[i] = words_[i].load(std::memory_order_relaxed);
        return buffer;
    }

public:
    seqlock() noexcept
        : seq_(0)
        , mutex_()
    {
        const buffer_type buffer = to_words(_Ty());
        for (std::size_t i = 0; i < word_count; ++i)
            words_[i].store(buffer[i], std::memory_order_relaxed);
    }

    explicit seqlock(const _Ty& _init)
        : seq_(0)
        , mutex_()
    {
        const buffer_type buffer = to_words(_init);
        for (std::size_t i = 0; i < word_count; ++i)
            words_[i].store(buffer[i], std::memory_order_relaxed);
    }

    seqlock(const seqlock&) = delete;
    seqlock& operator=(const seqlock&) = delete;

    // ---------------- writer ----------------

    void store(const _Ty& _value)
    {
        // Pack outside the lock to keep the critical section minimal.
        const buffer_type buffer = to_words(_value);

        std::lock_guard<mutex_type> locker(mutex_);
        store_locked(buffer);
    }

    // Atomic read-modify-write: _fn(_Ty&) runs under the lock.
    // If _fn throws, nothing is written, because the exception happens
    // before the sequence counter is incremented.
    template <typename _Fn>
    _Ty update(_Fn&& _fn)
    {
        std::lock_guard<mutex_type> locker(mutex_);

        _Ty value = from_words(read_locked());
        std::forward<_Fn>(_fn)(value);
        store_locked(to_words(value));

        return value;
    }

    // ---------------- reader ----------------

    // Single attempt. Returns true when a consistent snapshot was taken.
    bool try_load(_Ty& _out) const noexcept
    {
        const seq_type s1 = seq_.load(std::memory_order_acquire);

        // An odd sequence means a write is in progress.
        if (s1 & 1u)
            return false;

        buffer_type buffer = {};
        for (std::size_t i = 0; i < word_count; ++i)
            buffer[i] = words_[i].load(std::memory_order_relaxed);

        // The payload reads above must not be reordered after the re-read
        // of the sequence counter below.
        std::atomic_thread_fence(std::memory_order_acquire);

        const seq_type s2 = seq_.load(std::memory_order_relaxed);

        // The counter changed, so the snapshot is torn.
        if (s1 != s2)
            return false;

        _out = from_words(buffer);
        return true;
    }

    // Retry until a consistent snapshot is obtained.
    _Ty load() const noexcept
    {
        _Ty out = {};

        for (unsigned spins = 0; !try_load(out); ++spins)
        {
            if (spins < 64)
                seqlock_details::cpu_relax();
            else
                std::this_thread::yield();
        }

        return out;
    }

    // Advisory only: reports whether a writer is currently writing.
    bool write_in_progress() const noexcept
    {
        return (seq_.load(std::memory_order_relaxed) & 1u) != 0;
    }
};

} // namespace thrd
} // namespace mgpp

#endif // !MEGOPP_THRD_SEQLOCK_H_INCLUDED
