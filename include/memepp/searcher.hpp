#ifndef MEMEPP_SEARCHER_HPP_INCLUDED
#define MEMEPP_SEARCHER_HPP_INCLUDED

#include <meme/searcher.h>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/string_span.hpp>
#include <memepp/buffer_span.hpp>
#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/errc.hpp>
#include <mego/predef/lang/version.h>

#include <string>
#include <utility>
#include <cstring>
#include <cstdint>
#if MG_LANG__CXX17_AVAIL
#include <string_view>
#endif
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
 * Lifecycle state of a searcher.
 *
 *   empty  - never held a needle (default-constructed or reset()).
 *   ready  - init succeeded; find() may be called.
 *   failed - init was attempted and rejected; error_code() says why.
 */
enum class searcher_state_t : uint8_t
{
    empty  = MemeSearcherState_Empty,
    ready  = MemeSearcherState_Ready,
    failed = MemeSearcherState_Failed,
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
    using state_type         = searcher_state_t;
    using size_type          = mmint_t;

    /* "not found", matching memepp::string / string_span / std::string. */
#if MG_LANG__CXX17_AVAIL
    inline static constexpr size_type npos = static_cast<size_type>(-1);
#else
    enum : size_type { npos = static_cast<size_type>(-1) };
#endif

    /* Default constructor creates an empty searcher. */
    searcher() noexcept
    {
        MemeSearcherStack_initEmpty(&handle_, MMSEARCHER__OBJ_SIZE);
    }

    /* From raw bytes. */
    searcher(const mmbyte_t* _needle, size_type _len,
             case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(&handle_, MMSEARCHER__OBJ_SIZE, _needle, _len,
                               static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }

    /* From a null-terminated C string. */
    explicit searcher(const char* _needle,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        if (_needle != nullptr)
        {
            MemeSearcherStack_init(
                &handle_, MMSEARCHER__OBJ_SIZE,
                reinterpret_cast<const mmbyte_t*>(_needle),
                static_cast<size_type>(std::strlen(_needle)),
                static_cast<MemeFlag_CaseSensitivity_t>(_cs));
        }
        else
        {
            MemeSearcherStack_init(&handle_, MMSEARCHER__OBJ_SIZE,
                                   nullptr, 0,
                                   static_cast<MemeFlag_CaseSensitivity_t>(_cs));
        }
    }

#if MG_LANG__CXX17_AVAIL
    /* From std::string_view. */
    explicit searcher(std::string_view _needle,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(
            &handle_, MMSEARCHER__OBJ_SIZE,
            reinterpret_cast<const mmbyte_t*>(_needle.data()),
            static_cast<size_type>(_needle.size()),
            static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }
#endif

    /* From memepp::string_view. */
    explicit searcher(memepp::string_view _sv,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(
            &handle_, MMSEARCHER__OBJ_SIZE,
            reinterpret_cast<const mmbyte_t*>(_sv.data()),
            static_cast<size_type>(_sv.size()),
            static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }

    /* From memepp::string_span (non-owning view of any UTF-8 data). */
    explicit searcher(memepp::string_span _span,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(&handle_, MMSEARCHER__OBJ_SIZE,
                               _span.bytes(), _span.size(),
                               static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }

    /* From memepp::buffer_span (generic byte range). */
    explicit searcher(memepp::buffer_span _span,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(&handle_, MMSEARCHER__OBJ_SIZE,
                               _span.data(), _span.size(),
                               static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }

    /* From memepp::buffer. */
    explicit searcher(const memepp::buffer& _buf,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(&handle_, MMSEARCHER__OBJ_SIZE,
                               _buf.data(), _buf.size(),
                               static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }

    /* From memepp::buffer_view. */
    explicit searcher(const memepp::buffer_view& _bv,
                      case_sensitivity_t _cs = case_sensitivity_t::all_sensitive)
    {
        MemeSearcherStack_init(&handle_, MMSEARCHER__OBJ_SIZE,
                               _bv.data(), _bv.size(),
                               static_cast<MemeFlag_CaseSensitivity_t>(_cs));
    }

    /*
     * Move semantics: O(1).
     *
     * MemeSearcherStack_move() byte-copies the object and relocates the
     * pointers that referred to the source's own inline storage.  No tables
     * are rebuilt, so this is cheap even for long needles.
     */
    searcher(searcher&& _other) noexcept
    {
        MemeSearcherStack_move(&handle_, MMSEARCHER__OBJ_SIZE, &_other.handle_);
    }

    searcher& operator=(searcher&& _other) noexcept
    {
        if (this != &_other)
        {
            /* unInit is a no-op on an object that holds nothing. */
            MemeSearcherStack_unInit(&handle_);
            MemeSearcherStack_move(&handle_, MMSEARCHER__OBJ_SIZE,
                                   &_other.handle_);
        }
        return *this;
    }

    /*
     * Copy semantics: deep copy.
     *
     * MemeSearcherStack_initByOther() rebuilds the needle and every table in
     * the destination, so the two searchers share nothing and can be released
     * independently.  This is O(needle_len) -- noticeably more expensive than
     * a word copy, so prefer move or a reference when the copy is not needed.
     */
    searcher(const searcher& _other)
    {
        MemeSearcherStack_initByOther(&handle_, MMSEARCHER__OBJ_SIZE,
                                      &_other.handle_);
    }

    searcher& operator=(const searcher& _other)
    {
        if (this != &_other)
        {
            /* unInit is a no-op on an object that holds nothing. */
            MemeSearcherStack_unInit(&handle_);
            MemeSearcherStack_initByOther(&handle_, MMSEARCHER__OBJ_SIZE,
                                          &_other.handle_);
        }
        return *this;
    }

    ~searcher()
    {
        MemeSearcherStack_unInit(&handle_);
    }

    /* ------------------------------ search ---------------------------- */

    /*
     * Every overload forwards straight to MemeSearcherStack_find(), which
     * already rejects a NULL haystack and an uninitialised object, so no
     * check is repeated here.  _haystack_len < 0 asks the C side for the
     * NUL-terminated mode.
     */
    size_type find(const memepp::string& _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, reinterpret_cast<const mmbyte_t*>(_text.data()),
            static_cast<size_type>(_text.size()), _offset));
    }

    size_type find(const memepp::string_view& _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, reinterpret_cast<const mmbyte_t*>(_text.data()),
            static_cast<size_type>(_text.size()), _offset));
    }

    size_type find(const char* _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, reinterpret_cast<const mmbyte_t*>(_text), -1, _offset));
    }

    size_type find(const mmbyte_t* _text, size_type _len,
                   size_type _offset = 0) const noexcept
    {
        if (_len < 0) return npos;      /* explicit length must be valid */
        return static_cast<size_type>(
            MemeSearcherStack_find(&handle_, _text, _len, _offset));
    }

#if MG_LANG__CXX17_AVAIL
    size_type find(std::string_view _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, reinterpret_cast<const mmbyte_t*>(_text.data()),
            static_cast<size_type>(_text.size()), _offset));
    }
#endif

    size_type find(memepp::string_span _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, _text.bytes(), _text.size(), _offset));
    }

    size_type find(memepp::buffer_span _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, _text.data(), _text.size(), _offset));
    }

    size_type find(const memepp::buffer& _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, _text.data(), _text.size(), _offset));
    }

    size_type find(const memepp::buffer_view& _text, size_type _offset = 0) const noexcept
    {
        return static_cast<size_type>(MemeSearcherStack_find(
            &handle_, _text.data(), _text.size(), _offset));
    }

    /* Convenience: true when the needle occurs anywhere in _text. */
    bool contains(const memepp::string_view& _text) const noexcept
    { return find(_text) != npos; }
    bool contains(memepp::string_span _text) const noexcept
    { return find(_text) != npos; }
    bool contains(memepp::buffer_span _text) const noexcept
    { return find(_text) != npos; }
    bool contains(const memepp::buffer& _text) const noexcept
    { return find(_text) != npos; }
    bool contains(const memepp::buffer_view& _text) const noexcept
    { return find(_text) != npos; }
    bool contains(const char* _text) const noexcept
    { return find(_text) != npos; }

    /* --------------------------- hit density -------------------------- */

    /* Density of hits so far.  Fewer than 4 find() calls -> unknown. */
    density_type density() const noexcept
    {
        return static_cast<search_density_t>(MemeSearcherStack_density(&handle_));
    }

    void reset_density() noexcept
    {
        MemeSearcherStack_resetDensity(&handle_);
    }

    /* ----------------------------- accessors -------------------------- */

    bool valid() const noexcept
    {
        return MemeSearcherStack_state(&handle_) == MemeSearcherState_Ready;
    }

    /* Let `if (srch)` work; valid() stays for explicitness. */
    explicit operator bool() const noexcept { return valid(); }

    /* Length of the needle in bytes; 0 when not ready. */
    size_type needle_size() const noexcept
    {
        return static_cast<size_type>(
            MemeSearcherStack_needleSize(&handle_));
    }

    /*
     * Case-sensitivity the searcher was built with.
     *
     * The C layer stores MemeFlag_CaseSensitivity_t (mmint_t), where
     * MemeFlag_AllSensitive is 0xFFFFFFFF and therefore reads back as
     * 4294967295 after integer promotion.  case_sensitivity_t is a 32-bit
     * enum whose all_sensitive is -1, so the two only agree in their low 32
     * bits.  Normalise here rather than leaking the C representation.
     */
    case_sensitivity_t case_sensitivity() const noexcept
    {
        const MemeFlag_CaseSensitivity_t cs =
            MemeSearcherStack_caseSensitivity(&handle_);

        if (static_cast<uint32_t>(cs) ==
            static_cast<uint32_t>(MemeFlag_AllSensitive))
        {
            return case_sensitivity_t::all_sensitive;
        }
        return static_cast<case_sensitivity_t>(cs);
    }

    /* Error code of the last construction / move / swap. */
    mgec_t error_code() const noexcept
    {
        return static_cast<mgec_t>(MemeSearcherStack_errorCode(&handle_));
    }

    state_type state() const noexcept
    {
        return static_cast<state_type>(MemeSearcherStack_state(&handle_));
    }

    /*
     * Release the needle and every pre-built table, returning to the
     * default-constructed (empty) state.  Safe to call repeatedly and on an
     * object that never held a needle.
     */
    void reset() noexcept
    {
        MemeSearcherStack_unInit(&handle_);
    }

    native_handle_type native_handle() noexcept { return &handle_; }
    const MemeSearcherStack_t* native_handle() const noexcept { return &handle_; }

    /* Swap the two objects' contents. */
    void swap(searcher& _other) noexcept
    {
        MemeSearcherStack_swap(&handle_, &_other.handle_);
    }

private:
    MemeSearcherStack_t handle_;
};

} // namespace MMPP_NAMESPACE
} // namespace memepp

#endif /* MEMEPP_SEARCHER_HPP_INCLUDED */
