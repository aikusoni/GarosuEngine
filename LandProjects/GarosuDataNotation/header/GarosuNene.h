#pragma once
#ifndef __GAROSU_NENE_H__
#define __GAROSU_NENE_H__

#include "GarosuTypedef.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <exception>

#include <iostream>

namespace Garosu
{
	class NeneException : public std::exception
	{};

	class NeneTypeMismatchException : public NeneException
	{
	public:
		virtual const char* what() const throw()
		{
			return "NeneTypeMismatchException";	// TODO
		}
	} nene_type_mismatch;

	class NeneOutOfRangeException : public NeneException
	{
	public:
		virtual const char* what() const throw()
		{
			return "NeneOutOfRangeException";	// TODO
		}
	} nene_out_of_range;

	class NenePointersNotAllocated : public NeneException
	{
	public:
		virtual const char* what() const throw()
		{
			return "NenePointersNotAllocated";	// TODO
		}
	} nene_pointers_not_allocated;

	class NeneHasNoKey : public NeneException
	{
	public:
		virtual const char* what() const throw()
		{
			return "NeneHasNoKey";	// TODO
		}
	} nene_has_no_key;

	class Nene
	{
	public:
		using INT = int;	// integer
		using FLT = double;	// floating
		using BLN = bool;	// boolean

		using STR = std::string;
		using VEC = std::vector<Nene>;
		using MAP = std::map<std::string, Nene>;

		using INIT_LIST_NENE = std::initializer_list<Nene>;
		using PAIR = std::pair<std::string, Nene>;
		using INIT_LIST_PAIR = std::initializer_list<PAIR>;

	private:
		union NeneValue
		{
			// value type
			INT i_value;
			FLT f_value;
			BLN b_value;
			// pointer type
			STR* s_pointer;
			VEC* v_pointer;
			MAP* m_pointer = nullptr;
		} value;

		enum class NeneType
		{
			EMPTY,
			INTEGER,
			FLOATING,
			BOOLEAN,
			STRING,
			VECTOR,
			MAP
		} type;

		struct set_false { enum { value = false }; };
		struct set_true { enum { value = true }; };

		template <typename T, typename U> struct is_same : set_false {};
		template <typename T> struct is_same<T, T> : set_true {};

		template <typename T> struct remove_const { using type = T; };
		template <typename T> struct remove_const<const T> { using type = T; };

		template <typename T> struct remove_reference { using type = T; };
		template <typename T> struct remove_reference<T&> { using type = T; };
		template <typename T> struct remove_reference<T&&> { using type = T; };

		template <typename T> struct pure_type : remove_const<typename remove_reference<T>::type> {};

		template <typename T, int N = 0> struct is_supported : set_false { private: is_supported(); };
		template <> struct is_supported<INT> : set_true {};
		template <> struct is_supported<FLT> : set_true {};
		template <> struct is_supported<BLN> : set_true {};
		template <> struct is_supported<const char*> : set_true {};
		template <int N> struct is_supported<char[N]> : set_true {};
		template <> struct is_supported<STR> : set_true {};
		template <> struct is_supported<VEC> : set_true {};
		template <> struct is_supported<MAP> : set_true {};
		// TODO 

		template <typename T, int N = 0> struct nene_type { private: nene_type(); };
		template <> struct nene_type<INT> { const static NeneType value = NeneType::INTEGER; static void set(NeneValue& target, const INT& v) { target.i_value = v; } };
		template <> struct nene_type<FLT> { const static NeneType value = NeneType::FLOATING; static void set(NeneValue& target, const FLT& v) { target.f_value = v; } };
		template <> struct nene_type<BLN> { const static NeneType value = NeneType::BOOLEAN; static void set(NeneValue& target, const BLN& v) { target.b_value = v; } };
		template <> struct nene_type<const char*> { const static NeneType value = NeneType::STRING; static void set(NeneValue& target, const char* v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } };
		template <int N> struct nene_type<char[N]> { const static NeneType value = NeneType::STRING; static void set(NeneValue& target, const char* v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } }; // char* : string
		template <> struct nene_type<STR> { const static NeneType value = NeneType::STRING; static void set(NeneValue& target, const STR& v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } };
		template <> struct nene_type<VEC> { const static NeneType value = NeneType::VECTOR; static void set(NeneValue& target, const VEC& v) { if (target.v_pointer == nullptr) target.v_pointer = new VEC; *target.v_pointer = v; } };
		template <> struct nene_type<MAP> { const static NeneType value = NeneType::MAP; static void set(NeneValue& target, const MAP& v) { if (target.m_pointer == nullptr) target.m_pointer = new MAP; *target.m_pointer = v; } };
		// TODO

		template <typename T, int N = 0> struct is_index_key : set_false { private: is_index_key(); };
		template <> struct is_index_key<const char*> : set_true {};
		template <int N> struct is_index_key<char[N]> : set_true {};
		template <> struct is_index_key<STR> : set_true {};

#define ASSERT_TYPE_NOT_SUPPORTED(T) static_assert(is_supported<T>::value, "The type is not supported")
#define ASSERT_TYPE_NOT_MAP_KEY(T) static_assert(is_index_key<T>::value, "The type is not map key")

	public:
		virtual ~Nene(void)
		{
			Clear();
		}
		Nene(void)
		{
			Reset();
			type = NeneType::EMPTY;
		}
		Nene(const Nene& rhs)
		{
			Reset();
			Set(rhs);
		}
		Nene(const Nene&& rhs)
		{
			Reset();
			Set(rhs);
		}
		Nene(const INIT_LIST_NENE& neneList)
		{
			Reset();
			Set(VEC(neneList));
		}
		Nene(const INIT_LIST_NENE&& neneList)
		{
			Reset();
			Set(VEC(neneList));
		}
		Nene(const INIT_LIST_PAIR& pairList)
		{
			Reset();
			Set(MAP(pairList.begin(), pairList.end()));
		}
		Nene(const INIT_LIST_PAIR&& pairList)
		{
			Reset();
			Set(MAP(pairList.begin(), pairList.end()));
		}
		template <typename T>
		Nene(const T& v) {
			ASSERT_TYPE_NOT_SUPPORTED(T);
			Reset();
			Set(v);
		}
		template <typename T>
		Nene(const T&& v) {
			ASSERT_TYPE_NOT_SUPPORTED(T);
			Reset();
			Set(v);
		}

		Nene& operator=(const Nene& rhs)
		{
			if (type != rhs.type) Clear();
			Set(rhs);
			return *this;
		}
		Nene& operator=(const Nene&& rhs)
		{
			if (type != rhs.type) Clear();
			Set(rhs);
			return *this;
		}
		template <typename T>
		Nene& operator=(const T& v) {
			ASSERT_TYPE_NOT_SUPPORTED(T);
			if (nene_type<pure_type<T>::type>::value != type) Clear();
			Set(v);
			return *this;
		}
		template <typename T>
		Nene& operator=(const T&& v) {
			ASSERT_TYPE_NOT_SUPPORTED(T);
			if (nene_type<pure_type<T>::type>::value != type) Clear();
			Set(v);
			return *this;
		}

		void append(const Nene& nene)
		{
			AddToArray(nene);
		}
		void append(const Nene&& nene)
		{
			AddToArray(nene);
		}

		Nene& GetVec(const int index)
		{
			if (type != NeneType::VECTOR)
				throw nene_type_mismatch;
			try {
				return (*value.v_pointer)[index];
			}
			catch (std::out_of_range)
			{
				throw nene_has_no_key;
			}
		}

		void SetVec(const int index, const Nene& nene)
		{
			if (type != NeneType::VECTOR)
				Clear();
			type = NeneType::VECTOR;
			if (value.v_pointer == nullptr) value.v_pointer = new VEC;
			try {
				(*value.v_pointer)[index] = nene;
			}
			catch (std::out_of_range)
			{
				throw nene_out_of_range;
			}
		}

		template <typename TStr>
		bool HasKey(const TStr& key)
		{
			ASSERT_TYPE_NOT_MAP_KEY(TStr);
			if (type != NeneType::MAP)
				throw nene_type_mismatch;
			try {
				return (*value.m_pointer)[key];
			}
			catch (std::out_of_range)
			{
				return false;
			}
			return true;
		}

		template <typename TStr>
		bool HasKey(const TStr&& key)
		{
			ASSERT_TYPE_NOT_MAP_KEY(TStr);
			if (type != NeneType::MAP)
				throw nene_type_mismatch;
			try {
				return (*value.m_pointer)[key];
			}
			catch (std::out_of_range)
			{
				return false;
			}
			return true;
		}

		template <typename TStr>
		Nene& GetMap(const TStr& key)
		{
			ASSERT_TYPE_NOT_MAP_KEY(TStr);
			if (type != NeneType::MAP)
				throw nene_type_mismatch;
			try {
				return (*value.m_pointer)[key];
			}
			catch (std::out_of_range)
			{
				throw nene_has_no_key;
			}
		}

		template <typename TStr>
		void SetMap(const TStr& key, const Nene& nene)
		{
			ASSERT_TYPE_NOT_MAP_KEY(TStr);
			if (type != NeneType::MAP)
				Clear();
			type = NeneType::MAP;
			if (value.m_pointer == nullptr) value.m_pointer = new MAP;
			(*value.m_pointer)[key] = nene;
		}

		template <typename T>
		bool hasType(void)
		{
			if (type == nene_type<pure_type<T>::type>::value)
				return true;
			return false;
		}

		bool isSameType(const Nene& rhs)
		{
			if (type == rhs.type)
				return true;
			return false;
		}

		size_t size(void)
		{
			switch (type)
			{
			case NeneType::EMPTY: return (size_t)1; break;
			case NeneType::INTEGER: return (size_t)1;  break;
			case NeneType::FLOATING: return (size_t)1;  break;
			case NeneType::BOOLEAN: return (size_t)1;  break;
			case NeneType::STRING: return (*value.s_pointer).size();  break;
			case NeneType::VECTOR: return (*value.v_pointer).size();  break;
			case NeneType::MAP: return (*value.m_pointer).size();  break;
			}
			return (size_t)0;
		}

		bool Parse(std::istream& file)
		{
			char buf[256];
			Clear();

			while (!file.eof())
			{
				file.get(buf, _G_COUNT_OF(buf));
			}

			return true;
		}

		friend std::ostream& operator<<(std::ostream& os, const Garosu::Nene& nene)
		{
			switch (nene.type)
			{
			case Garosu::Nene::NeneType::EMPTY:
				os << "{}";
				break;

			case Garosu::Nene::NeneType::INTEGER:
				os << nene.value.i_value;
				break;

			case Garosu::Nene::NeneType::FLOATING:
				os << nene.value.f_value;
				break;

			case Garosu::Nene::NeneType::BOOLEAN:
				os << nene.value.b_value;
				break;

			case Garosu::Nene::NeneType::STRING:
			{
				os << "\"";
				if (Garosu::Nene::SafeString::IsUnsafe(*nene.value.s_pointer))
				{
					os << Garosu::Nene::SafeString::GetSafe(*nene.value.s_pointer);
				}
				else
					os << *nene.value.s_pointer;
				os << "\"";
			}
			break;

			case Garosu::Nene::NeneType::VECTOR:
			{
				auto& vec = *nene.value.v_pointer;
				os << "[";
				for (auto& it = vec.begin(); it != vec.end();)
				{
					os << *it;
					if (++it < vec.end()) os << ",";
				}
				os << "]";
			}
			break;

			case Garosu::Nene::NeneType::MAP:
			{
				auto& map = *nene.value.m_pointer;
				os << "{";
				for (auto& it = map.begin(); it != map.end();)
				{
					os << "\"" << (*it).first << "\":" << (*it).second;
					if (++it != map.end()) os << ",";
				}
				os << "}";
			}
			break;
			}
			return os;
		}

	private:
		void Reset(void)
		{
			memset(&value, 0x00, sizeof(value));
		}

		void Set(void)
		{
			type = NeneType::EMPTY;
		}
		void Set(const Nene& rhs)
		{
			switch (rhs.type)
			{
			case NeneType::EMPTY: Set(); break;
			case NeneType::INTEGER:	Set(rhs.value.i_value);	break;
			case NeneType::FLOATING: Set(rhs.value.f_value); break;
			case NeneType::BOOLEAN: Set(rhs.value.b_value); break;
			case NeneType::STRING: Set(*rhs.value.s_pointer); break;
			case NeneType::VECTOR: Set(*rhs.value.v_pointer); break;
			case NeneType::MAP: Set(*rhs.value.m_pointer); break;
			}
		}
		template <typename T>
		void Set(const T& newVal)
		{
			if (type != nene_type<pure_type<T>::type>::value)
				type = nene_type<pure_type<T>::type>::value;

			nene_type<pure_type<T>::type>::set(value, newVal);
		}

		void AddToArray(const Nene& nene)
		{
			if (type != NeneType::VECTOR)
			{
				// fixme reduce copying
				Nene beforeNene = *this;
				Clear();
				type = NeneType::VECTOR;
				if (value.v_pointer == nullptr) value.v_pointer = new VEC;
				if (beforeNene.type != NeneType::EMPTY) (*value.v_pointer).push_back(beforeNene);
			}
			(*value.v_pointer).emplace_back(nene);
		}

		void Clear(void)
		{
			switch (type)
			{
			case NeneType::EMPTY:
			case NeneType::INTEGER:
			case NeneType::FLOATING:
			case NeneType::BOOLEAN:
				break;

			case NeneType::STRING:
				if (value.s_pointer != nullptr) delete value.s_pointer;
				break;
			case NeneType::VECTOR:
				if (value.v_pointer != nullptr) delete value.v_pointer;
				break;
			case NeneType::MAP:
				if (value.m_pointer != nullptr) delete value.m_pointer;
				break;
			}
			memset(&value, 0x00, sizeof(value));
			// type = NeneType::EMPTY;
		}
	public:
		class SafeString
		{
			using STR = Nene::STR;
		private:
			static const std::map<char, STR> UnsafeCharacters;

		public:
			inline static bool IsUnsafe(const STR& str)
			{
				int cnt = 0;
				for (auto it = str.begin(); it != str.end(); ++it)
				{
					if (UnsafeCharacters.find(*it) != UnsafeCharacters.end())
						cnt++;
				}
				return cnt > 0 ? true : false;
			}
			inline static STR GetSafe(const STR& str)
			{
				std::stringstream ss;
				for (auto it = str.begin(); it != str.end(); ++it)
				{
					auto& us = UnsafeCharacters.find(*it);
					if (us != UnsafeCharacters.end()) ss << (*us).second;
					else ss << *it;
				}
				return ss.str();
			}
		};
	};

	class Token
	{
	public:
		enum class TokenType
		{
			NONE,	//
		BEGIN_MAP,	// {
		END_MAP,	// }
		BEGIN_ARRAY,	// [
		END_ARRAY,	// ]
		COMMA,	// ,
		COLON,	// :
		STRING,	// "some string"
		INTEGER, // 12345
		FLOATING, // 1.414
		BOOLEAN, // true, false
		} type = TokenType::NONE;
	};

	class Parser
	{
	public:
		static bool parse(Nene& target, std::istream& is)
		{
			target = {}; // clear

			while (is)
			{
				auto token = GetToken(is);
				switch (token.type)
				{
				case Token::TokenType::BEGIN_MAP:
					break;
				}
			}

			return true;
		}

		static Token GetToken(std::istream& is)
		{
			return Token();
		}
	};

	const std::map<char, std::string> Nene::SafeString::UnsafeCharacters = { { '\"', "\\\"" } };

}

#endif