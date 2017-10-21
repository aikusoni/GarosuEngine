#pragma once
#ifndef __GAROSU_NENE_H__
#define __GAROSU_NENE_H__

#include "GarosuTypedef.h"

#include <iostream>
#include <iomanip>

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
		using INT = i64;	// integer
		using FLT = f64;	// floating
		using BLN = bool;	// boolean

		using STR = String;
		using VEC = std::vector<Nene>;
		using MAP = std::unordered_map<String, Nene>;

		using INIT_LIST_NENE = std::initializer_list<Nene>;
		using PAIR = std::pair<String, Nene>;
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

		template <typename T, i64 N = 0> struct is_supported : set_false { public: is_supported(void) = delete; };
		template <> struct is_supported<INT> : set_true {};
		template <> struct is_supported<FLT> : set_true {};
		template <> struct is_supported<BLN> : set_true {};
		template <> struct is_supported<const char*> : set_true {};
		template <i64 N> struct is_supported<char[N]> : set_true {};
		template <> struct is_supported<STR> : set_true {};
		template <> struct is_supported<VEC> : set_true {};
		template <> struct is_supported<MAP> : set_true {};
		// TODO 

		template <typename T, i64 N = 0> struct nene_type { public: nene_type(void) = delete; };
		template <> struct nene_type<INT> { const static NeneType value = NeneType::INTEGER; static void set(NeneValue& target, const INT& v) { target.i_value = v; } };
		template <> struct nene_type<FLT> { const static NeneType value = NeneType::FLOATING; static void set(NeneValue& target, const FLT& v) { target.f_value = v; } };
		template <> struct nene_type<BLN> { const static NeneType value = NeneType::BOOLEAN; static void set(NeneValue& target, const BLN& v) { target.b_value = v; } };
		template <> struct nene_type<const char*> { const static NeneType value = NeneType::STRING; static void set(NeneValue& target, const char* v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } };
		template <i64 N> struct nene_type<char[N]> { const static NeneType value = NeneType::STRING; static void set(NeneValue& target, const char* v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } }; // char* : string
		template <> struct nene_type<STR> { const static NeneType value = NeneType::STRING; static void set(NeneValue& target, const STR& v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } };
		template <> struct nene_type<VEC> { const static NeneType value = NeneType::VECTOR; static void set(NeneValue& target, const VEC& v) { if (target.v_pointer == nullptr) target.v_pointer = new VEC; *target.v_pointer = v; } };
		template <> struct nene_type<MAP> { const static NeneType value = NeneType::MAP; static void set(NeneValue& target, const MAP& v) { if (target.m_pointer == nullptr) target.m_pointer = new MAP; *target.m_pointer = v; } };
		// TODO

		template <typename T, i64 N = 0> struct is_index_key : set_false { public: is_index_key() = delete; };
		template <> struct is_index_key<const char*> : set_true {};
		template <i64 N> struct is_index_key<char[N]> : set_true {};
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

		Nene& GetVec(const u64 index)
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

		void SetVec(const u64 index, const Nene& nene)
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
				os << "null";
				break;

			case Garosu::Nene::NeneType::INTEGER:
				os << nene.value.i_value;
				break;

			case Garosu::Nene::NeneType::FLOATING:
				os << nene.value.f_value;
				break;

			case Garosu::Nene::NeneType::BOOLEAN:
				os << std::boolalpha << nene.value.b_value;
				break;

			case Garosu::Nene::NeneType::STRING:
			{
				os << "\"";
				if (Garosu::Nene::SafeString::IsUnsafe(*nene.value.s_pointer))
					os << Garosu::Nene::SafeString::GetSafe(*nene.value.s_pointer);
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
				u64 cnt = 0;
				for (auto it = str.begin(); it != str.end(); ++it)
				{
					if (UnsafeCharacters.find(*it) != UnsafeCharacters.end())
						cnt++;
				}
				return cnt > 0 ? true : false;
			}
			inline static STR GetSafe(const STR& str)
			{
				StringStream ss;
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

	const std::map<char, String> Nene::SafeString::UnsafeCharacters = { { '\"', "\\\"" } };

	class NeneParser
	{
	public:
		std::istream& mIS;
		NeneParser(std::istream &is) : mIS(is) {}

		bool Parse(Nene& nene) {
			GetNene(nene);
			return true;
		}

	private:
		bool Match(const char* str, u32 cnt, bool ignoreCase = false)
		{
			char caseBitmask = ignoreCase ? 0xDF : 0xFF;
			u32 i = 0;
			for (i = 0u; mIS && i < cnt; ++i)
			{
				char c = mIS.get();
				char maskss = (c >= 'a' && c <= 'z') ? caseBitmask : 0xFF;
				char maskstr = (str[i] >= 'a' && str[i] <= 'z') ? caseBitmask : 0xFF;
				if ((c & maskss) != (str[i] & maskstr))
				{
					for (u32 j = i + 1; j > 0u; j--) mIS.unget();
					return false;
				}
			}
			if (i < cnt) return false;
			return true;
		}
		bool Has(const char c, const char* str, u32 cnt)
		{
			for (u32 i = 0u; i < cnt; ++i)
				if (str[i] == c) return true;
			return false;
		}

		bool SkipSpaceAndComment(void)
		{
			while (mIS)
			{
				char c = mIS.peek();
				if (Match("//", 2))
				{
					while (mIS)
					{
						c = mIS.get();
						if (Has(c, "\r\n", 2)) break;
					}
				}
				else if (Has(c, " \t\r\n", 4))
					mIS.get();
				else break;
			}

			return true;
		}

		bool GetString(StringStream& ss)
		{
			SkipSpaceAndComment();
			if (!mIS) return false;

			char c = mIS.get();
			if (c != '\"') {
				mIS.unget(); return false;
			}

			bool escapeSeq = false;
			while (mIS)
			{
				c = mIS.get();
				if (escapeSeq)
				{
					ss << c;
					escapeSeq = false;
				}
				else if (c == '\\')
				{
					ss << c;
					escapeSeq = true;
				}
				else if (c == '\"') return true;
				else ss << c;
			}
			return false;
		}

		bool GetNumber(Nene& nene)
		{
			const char numberEnd[] = ", \t]}\r\n";
			SkipSpaceAndComment();
			if (!mIS) return false;

			StringStream ss;

			i32 cntSign = 0;
			// natural
			while (mIS)
			{
				char c = mIS.peek();

				if (Has(c, "-+", 2)) {
					if (cntSign > 0) return false;
					++cntSign;
					ss << (char)mIS.get();
				}
				else if (c >= '0' && c <= '9') ss << (char)mIS.get();
				else if (c == '.') {
					ss << (char)mIS.get();
					break;
				}
				else if (Has(c, "eE", 2)) break;
				else if (Has(c, numberEnd, _G_COUNT_OF(numberEnd))) {
					Nene::INT n = std::stoll(ss.str());
					nene = n;
					return true; // "key" :, -> 0
				}
				else return false;
			}
			// fraction
			while (mIS)
			{
				char c = mIS.peek();

				if (c >= '0' && c <= '9') ss << (char)mIS.get();
				else if (Has(c, "eE", 2))
				{
					ss << (char)mIS.get();
					break;
				}
				else if (Has(c, numberEnd, _G_COUNT_OF(numberEnd)))
				{
					ss << '0';
					Nene::FLT f = std::stold(ss.str());
					nene = f;
					return true; // "key" :., -> 0
				}
				else return false;
			}
			// exponent
			while (mIS)
			{
				char c = mIS.peek();
				if (c >= '0' && c <= '9' || c == '-' || c == '+') ss << (char)mIS.get();
				else if (Has(c, numberEnd, _G_COUNT_OF(numberEnd)))
				{
					Nene::FLT f = std::stold(ss.str());
					nene = f;
					return true;
				}
				else return false;
			}

			return true;
		}

		bool GetBool(Nene& nene)
		{
			SkipSpaceAndComment();
			if (!mIS) return false;

			char c = mIS.peek();
			i32 i = 0;
			switch (c)
			{
			case 'T':
			case 't':
			{
				if (Match("true", 4, true))
				{
					nene = true;
					return true;
				}
			}
			break;

			case 'F':
			case 'f':
				if (Match("false", 5, true))
				{
					nene = false;
					return true;
				}
				break;
			}

			return false;
		}

		bool GetNene(Nene& nene)
		{
			SkipSpaceAndComment();
			if (!mIS) return false;

			char c = mIS.peek();
			if (Match("NULL", 4, true))
			{
				nene = {};
				return true;
			}
			else if (c == '\"')
			{
				StringStream ss;
				if (GetString(ss) == false) return false;
				nene = ss.str();
				return true;
			}
			else if (Has(c, "tTfF", 4))
			{
				if (GetBool(nene) == false) return false;
				return true;
			}
			else if (c == '{')
			{
				if (GetMap(nene) == false) return false;
				return true;
			}
			else if (c == '[')
			{
				if (GetArray(nene) == false) return false;
				return true;
			}
			else
			{
				if (GetNumber(nene) == false) return false;
				return true;
			}

			return false;
		}

		bool GetMap(Nene& nene)
		{
			SkipSpaceAndComment();
			if (!mIS) return false;

			char c = mIS.get();
			if (c != '{') { mIS.unget(); return false; }

			nene = {};

			while (mIS)
			{
				SkipSpaceAndComment();
				StringStream keySS;
				if (GetString(keySS) == false) break;
				SkipSpaceAndComment();

				if (!mIS) return false;
				if (mIS.peek() != ':') break;
				mIS.get();

				SkipSpaceAndComment();
				Nene valueNene;
				if (GetNene(valueNene) == false) break;
				SkipSpaceAndComment();

				nene.SetMap(keySS.str(), valueNene);

				if (!mIS) return false;
				if (mIS.peek() != ',') break;
				mIS.get();
			}
			SkipSpaceAndComment();
			if (mIS.peek() != '}') return false;

			mIS.get();
			return true;
		}

		bool GetArray(Nene& nene)
		{
			SkipSpaceAndComment();
			if (!mIS) return false;

			char c = mIS.get();
			if (c != '[') { mIS.unget(); return false; }

			nene = {};

			while (mIS)
			{
				SkipSpaceAndComment();
				Nene elNene;
				if (GetNene(elNene) == false) return false;
				SkipSpaceAndComment();

				nene.append(elNene);

				if (!mIS) return false;
				if (mIS.peek() != ',') break;
				mIS.get();
			}
			SkipSpaceAndComment();
			if (mIS.peek() != ']') return false;

			mIS.get();

			return true;
		}
	};

}

#endif