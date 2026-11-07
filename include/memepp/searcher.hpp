#ifndef MEMEPP_SEARCHER_HPP_INCLUDED
#define MEMEPP_SEARCHER_HPP_INCLUDED

#include <meme/searcher.h>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

#include <string_view>
#include <utility>
#include <cstring>
#include <megopp/predef/keyword/noexcept.h>

namespace memepp {
inline namespace MMPP_NAMESPACE {

/*
 * RAII wrapper around MemeSearcherStack_t.
 *
 * A searcher pre-processes a needle once, then reuses the result across
 * many find() calls on different haystacks.  This amortises the
 * Boyer-Moore table-building cost (or the fast-scan setup cost).
 *
 * The MemeSearcherStack_t object is stored inline (a fixed-size byte
 * buffer), so a searcher occupies a known size and needs no allocation of
 * its own; any tables it builds are owned internally.
 *
 * Thread-safety:
 *   - find() is read-only apart from the hit/miss counters, so concurrent
 *     calls are safe in practice but the density numbers may be racy.
 *   - density() and reset_density() touch mutable counters.
 */
class searcher
{
public:
    using native_handle_type = mmsrchstk_t*;
    using density_type       = MemeSearcherDensity_t;
    using size_type          = mmint_t;

    /* Default constructor creates an invalid searcher. */
    searcher() noexcept : valid_(false)
    {
        std::memset(&handle_, 0, sizeof(handle_));
    }

    /* From raw bytes. */
    searcher(const MemeByte_t* _needle, size_type _len,
             case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        std::memset(&handle_, 0, sizeof(handle_));
        if (_needle != nullptr && _len > 0)
        {
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE, _needle, _len,
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
    }

    /* From a null-terminated C string. */
    explicit searcher(const char* _needle,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        std::memset(&handle_, 0, sizeof(handle_));
        if (_needle != nullptr && _needle[0] != '\0')
        {
            const size_type n = static_cast<size_type>(std::strlen(_needle));
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const MemeByte_t*>(_needle), n,
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
    }

    /* From std::string_view. */
    explicit searcher(std::string_view _needle,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        std::memset(&handle_, 0, sizeof(handle_));
        if (!_needle.empty())
        {
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const MemeByte_t*>(_needle.data()),
                static_cast<size_type>(_needle.size()),
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
    }

    /* From memepp::string_view. */
    explicit searcher(memepp::string_view _sv,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        std::memset(&handle_, 0, sizeof(handle_));
        if (!_sv.empty())
        {
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const MemeByte_t*>(_sv.data()),
                static_cast<size_type>(_sv.size()),
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
    }

    /*
     * Move semantics.
     *
     * The object cannot simply be byte-copied: needle_ptr and the table view
     * pointers may refer to the source object's own inline_ storage, so a
     * memcpy would leave them dangling once the source is destroyed.
     * initByOther() rebuilds the tables in the destination instead.
     */
    searcher(searcher&& _other) : valid_(false)
    {
        std::memset(&handle_, 0, sizeof(handle_));
        if (_other.valid_)
        {
            valid_ = MemeSearcherStack_initByOther(
                &handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_) == 0;
            MemeSearcherStack_unInit(&_other.handle_);
            _other.valid_ = false;
        }
    }

    searcher& operator=(searcher&& _other)
    {
        if (this != &_other)
        {
            if (valid_) MemeSearcherStack_unInit(&handle_);
            std::memset(&handle_, 0, sizeof(handle_));
            valid_ = false;

            if (_other.valid_)
            {
                valid_ = MemeSearcherStack_initByOther(
                    &handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_) == 0;
                MemeSearcherStack_unInit(&_other.handle_);
                _other.valid_ = false;
            }
        }
        return *this;
    }

    /* Non-copyable: the object owns pre-built tables. */
    searcher(const searcher&)            = delete;
    searcher& operator=(const searcher&) = delete;

    ~searcher()
    {
        if (valid_) MemeSearcherStack_unInit(&handle_);
    }

    /* ------------------------------ search ---------------------------- */

    mmint_t find(const memepp::string& _text, mmint_t _offset = 0) const noexcept
    {
        if (!valid_) return -1;
        return MemeSearcherStack_find(
            &handle_,
            reinterpret_cast<const MemeByte_t*>(_text.data()),
            static_cast<MemeInteger_t>(_text.size()), _offset);
    }

    mmint_t find(const memepp::string_view& _text, mmint_t _offset = 0) const noexcept
    {
        if (!valid_) return -1;
        return MemeSearcherStack_find(
            &handle_,
            reinterpret_cast<const MemeByte_t*>(_text.data()),
            static_cast<MemeInteger_t>(_text.size()), _offset);
    }

    mmint_t find(const char* _text, mmint_t _offset = 0) const noexcept
    {
        if (!valid_ || _text == nullptr) return -1;
        return MemeSearcherStack_find(
            &handle_,
            reinterpret_cast<const MemeByte_t*>(_text),
            static_cast<MemeInteger_t>(std::strlen(_text)), _offset);
    }

    mmint_t find(const MemeByte_t* _text, size_type _len,
                 mmint_t _offset = 0) const noexcept
    {
        if (!valid_ || _text == nullptr) return -1;
        return MemeSearcherStack_find(&handle_, _text, _len, _offset);
    }

    /* --------------------------- hit density -------------------------- */

    /* Density of hits so far.  Fewer than 4 find() calls -> Unknown. */
    density_type density() const noexcept
    {
        return valid_ ? MemeSearcherStack_density(&handle_)
                      : MemeSearcherDensity_Unknown;
    }

    void reset_density() noexcept
    {
        if (valid_) MemeSearcherStack_resetDensity(&handle_);
    }

    /* ----------------------------- accessors -------------------------- */

    bool valid() const noexcept { return valid_; }

    native_handle_type native_handle() noexcept { return &handle_; }
    const MemeSearcherStack_t* native_handle() const noexcept { return &handle_; }

    void swap(searcher& _other) noexcept
    {
        std::swap(handle_, _other.handle_);
        std::swap(valid_, _other.valid_);
    }

private:
    MemeSearcherStack_t handle_;
    bool valid_;
};

} // namespace MMPP_NAMESPACE
} // namespace memepp

#endif /* MEMEPP_SEARCHER_HPP_INCLUDED */
