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
 * Hit-density classification for memepp::searcher.
 *
 * Mirrors the C MemeSearcherDensity_* values but is a scoped enum in C++,
 * so a density value cannot be confused with an algorithm id or a raw
 * integer.  The underlying type is fixed (uint8_t) to keep the ABI stable.
 */
enum class search_density_t : uint8_t
{
    unknown  = MemeSearcherDensity_Unknown,
    rare     = MemeSearcherDensity_Rare,
    sparse   = MemeSearcherDensity_Sparse,
    common   = MemeSearcherDensity_Common,
    frequent = MemeSearcherDensity_Frequent,
};

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
    using density_type       = search_density_t;
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
        if (_needle != nullptr && _len > 0)
        {
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE, _needle, _len,
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
        else
        {
            std::memset(&handle_, 0, sizeof(handle_));
        }
    }

    /* From a null-terminated C string. */
    explicit searcher(const char* _needle,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        if (_needle != nullptr && _needle[0] != '\0')
        {
            const size_type n = static_cast<size_type>(std::strlen(_needle));
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const MemeByte_t*>(_needle), n,
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
        else
        {
            std::memset(&handle_, 0, sizeof(handle_));
        }
    }

    /* From std::string_view. */
    explicit searcher(std::string_view _needle,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        if (!_needle.empty())
        {
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const MemeByte_t*>(_needle.data()),
                static_cast<size_type>(_needle.size()),
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
        else
        {
            std::memset(&handle_, 0, sizeof(handle_));
        }
    }

    /* From memepp::string_view. */
    explicit searcher(memepp::string_view _sv,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
        : valid_(false)
    {
        if (!_sv.empty())
        {
            valid_ = MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const MemeByte_t*>(_sv.data()),
                static_cast<size_type>(_sv.size()),
                static_cast<MemeFlag_CaseSensitivity_t>(_cs)) == 0;
        }
        else
        {
            std::memset(&handle_, 0, sizeof(handle_));
        }
    }

    /*
     * Move semantics: O(1).
     *
     * MemeSearcherStack_move() byte-copies the object and relocates the
     * pointers that referred to the source's own inline storage.  No tables
     * are rebuilt, so this is cheap even for long needles.
     */
    searcher(searcher&& _other) noexcept : valid_(false)
    {
        if (_other.valid_)
        {
            valid_ = MemeSearcherStack_move(
                &handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_) == 0;
            _other.valid_ = false;
        }
        else
        {
            std::memset(&handle_, 0, sizeof(handle_));
        }
    }

    searcher& operator=(searcher&& _other) noexcept
    {
        if (this != &_other)
        {
            if (valid_) MemeSearcherStack_unInit(&handle_);
            valid_ = false;

            if (_other.valid_)
            {
                valid_ = MemeSearcherStack_move(
                    &handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_) == 0;
                _other.valid_ = false;
            }
            else
            {
                std::memset(&handle_, 0, sizeof(handle_));
            }
        }
        return *this;
    }

    /*
     * Copy semantics: deep copy.
     *
     * MemeSearcherStack_initByOther() rebuilds the needle and every table in
     * the destination, so the two searchers share nothing and can be released
     * independently.  This is O(needle_len) — noticeably more expensive than
     * a word copy, so prefer move or a reference when the copy is not needed.
     */
    searcher(const searcher& _other) : valid_(false)
    {
        if (_other.valid_)
        {
            valid_ = MemeSearcherStack_initByOther(
                &handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_) == 0;
        }
        else
        {
            std::memset(&handle_, 0, sizeof(handle_));
        }
    }

    searcher& operator=(const searcher& _other)
    {
        if (this != &_other)
        {
            if (valid_) MemeSearcherStack_unInit(&handle_);
            valid_ = false;

            if (_other.valid_)
            {
                valid_ = MemeSearcherStack_initByOther(
                    &handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_) == 0;
            }
            else
            {
                std::memset(&handle_, 0, sizeof(handle_));
            }
        }
        return *this;
    }

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
            -1, _offset);
    }

    mmint_t find(const MemeByte_t* _text, size_type _len,
                 mmint_t _offset = 0) const noexcept
    {
        if (!valid_ || _text == nullptr) return -1;
        return MemeSearcherStack_find(&handle_, _text, _len, _offset);
    }

    /* --------------------------- hit density -------------------------- */

    /* Density of hits so far.  Fewer than 4 find() calls -> unknown. */
    density_type density() const noexcept
    {
        return valid_
            ? static_cast<search_density_t>(MemeSearcherStack_density(&handle_))
            : search_density_t::unknown;
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
