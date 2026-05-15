#ifndef MEMEPP_JOIN_SELF_HPP_INCLUDED
#define MEMEPP_JOIN_SELF_HPP_INCLUDED

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#include <iterator>
#include <vector>

namespace memepp {

	template<template<class> class _List>
	inline string join(
		const _List<memepp::string>& _items,
		const string_view& _sep)
	{
		std::vector<mmstrstk_t> raw;
		raw.reserve(_items.size());
		for (const auto& item : _items)
			raw.push_back(item.native_handle());
		mmstrstk_t out;
		mgec_t ec = MemeStringStack_join(
			&out, sizeof(out),
			_sep.data(), _sep.size(),
			raw.data(), static_cast<mmint_t>(raw.size()));
		if (ec)
			mmstrstk_uninit(&out);
		return ec ? string{} : string{ std::move(out) };
	}

	template<template<class, class...> class _List, class... _Arg>
	inline string join(
		const _List<memepp::string, _Arg...>& _items,
		const string_view& _sep)
	{
		std::vector<mmstrstk_t> raw;
		raw.reserve(_items.size());
		for (const auto& item : _items)
			raw.push_back(item.native_handle());
		mmstrstk_t out;
		mgec_t ec = MemeStringStack_join(
			&out, sizeof(out),
			_sep.data(), _sep.size(),
			raw.data(), static_cast<mmint_t>(raw.size()));
		if (ec)
			mmstrstk_uninit(&out);
		return ec ? string{} : string{ std::move(out) };
	}

	template<template<class> class _List>
	inline string join(
		const _List<memepp::string_view>& _items,
		const string_view& _sep)
	{
		std::vector<mmstrstk_t> raw;
		raw.reserve(_items.size());
		for (const auto& item : _items)
			raw.push_back(item.native_handle());
		mmstrstk_t out;
		mgec_t ec = MemeStringStack_join(
			&out, sizeof(out),
			_sep.data(), _sep.size(),
			raw.data(), static_cast<mmint_t>(raw.size()));
		if (ec)
			mmstrstk_uninit(&out);
		return ec ? string{} : string{ std::move(out) };
	}

	template<template<class, class...> class _List, class... _Arg>
	inline string join(
		const _List<memepp::string_view, _Arg...>& _items,
		const string_view& _sep)
	{
		std::vector<mmstrstk_t> raw;
		raw.reserve(_items.size());
		for (const auto& item : _items)
			raw.push_back(item.native_handle());
		mmstrstk_t out;
		mgec_t ec = MemeStringStack_join(
			&out, sizeof(out),
			_sep.data(), _sep.size(),
			raw.data(), static_cast<mmint_t>(raw.size()));
		if (ec)
			mmstrstk_uninit(&out);
		return ec ? string{} : string{ std::move(out) };
	}

} // namespace memepp

#endif // !MEMEPP_JOIN_SELF_HPP_INCLUDED
