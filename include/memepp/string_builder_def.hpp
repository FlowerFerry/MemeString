
#ifndef MEMEPP_STRING_BUILDER_DEF_HPP_INCLUDED
#define MEMEPP_STRING_BUILDER_DEF_HPP_INCLUDED

#include "string_def.hpp"
#include "string_view_fwd.hpp"
#include "string_builder_fwd.hpp"

#include "meme/string_builder_fwd.h"

namespace memepp { 

	class string_builder
	{
	public:

		using native_handle_type = ::MemeStringBuilderStack_t;

		string_builder();
		string_builder(string_builder& _other);
		string_builder(string_builder&& _other);

		~string_builder();

		string_builder& operator=(string_builder&& _other);
		string_builder& operator=(string_builder& _other);
		//string_builder& operator=(const string& _other);
		//string_builder& operator=(const string_view& _other);
		//string_builder& operator=(const char* _other);

        string_builder& operator+(const string& _other);
        string_builder& operator+(const string_view& _other);
        string_builder& operator+(const char* _other);
		//string_builder& operator+(char _other);
        //string_builder  operator+(const string_builder& _other) const;

		//string_builder& operator/(const string& _other);
  //      string_builder& operator/(const string_view& _other);
  //      string_builder& operator/(const char* _other);
  //      string_builder& operator/(char _other);

		string_builder& operator+=(const string& _other);
		string_builder& operator+=(const string_view& _other);
		string_builder& operator+=(const char* _other);
		//string_builder& operator+=(char _other);

        //string_builder& operator/=(const string& _other);
        //string_builder& operator/=(const string_view& _other);
        //string_builder& operator/=(const char* _other);
        //string_builder& operator/=(char _other);

		string release ();
        string generate() const;
        
		const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	//bool operator==(const string_builder& _lhs, const string& _rhs);
	//bool operator==(const string& _lhs, const string_builder& _rhs);
	//bool operator==(const string_builder& _lhs, const string_view& _rhs);
	//bool operator==(const string_view& _lhs, const string_builder& _rhs);

	//bool operator!=(const string_builder& _lhs, const string& _rhs);
	//bool operator!=(const string& _lhs, const string_builder& _rhs);
	//bool operator!=(const string_builder& _lhs, const string_view& _rhs);
	//bool operator!=(const string_view& _lhs, const string_builder& _rhs);

    string_builder& operator+(const string& _lhs, string_builder& _rhs);
    string_builder  operator+(const string& _lhs, string_builder&& _rhs);
    string_builder& operator+(const string_view& _lhs, string_builder& _rhs);
    string_builder  operator+(const string_view& _lhs, string_builder&& _rhs);
    string_builder& operator+(const char* _lhs, string_builder& _rhs);
    string_builder  operator+(const char* _lhs, string_builder&& _rhs);
	//template<size_t _Size>
	//inline string_builder operator+(const char _lhs[_Size], string_builder&& _rhs)
	//{
	//	memepp::string_view{ _lhs } + _rhs;
 //       return std::move(_rhs);
	//}
    
    //string_builder& operator+(char _lhs, string_builder& _rhs);

    //string_builder& operator/(const string& _lhs, string_builder& _rhs);
    //string_builder& operator/(const string_view& _lhs, string_builder& _rhs);
    //string_builder& operator/(const char* _lhs, string_builder& _rhs);
    //string_builder& operator/(char _lhs, string_builder& _rhs);
    
};

#endif // !MEMEPP_STRING_BUILDER_DEF_HPP_INCLUDED
