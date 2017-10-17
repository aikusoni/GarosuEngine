#pragma once
#ifndef __GAROSU_NENE_H__
#define __GAROSU_NENE_H__

namespace Garosu
{
#define ASSERT_TYPE_NOT_SUPPORTED static_assert(is_supported<T>::value, "This type is not supported")
	class Nene
	{
		using STR = std::string;
		using VEC = std::vector<Nene>;
		using MAP = std::unordered_map<std::string, Nene>;

	private:
		union NeneValue
		{
			int i_value;
			float f_value;
			bool b_value;
			STR* s_pointer;
			VEC* v_pointer;
			MAP* m_pointer;
		} value;

		enum class NeneType
		{
			EMPTY,
			INTEGER,
			FLOAT,
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
		template <> struct is_supported<int> : set_true {};
		template <> struct is_supported<float> : set_true {};
		template <> struct is_supported<bool> : set_true {};
		template <int N> struct is_supported<char[N]> : set_true {};
		template <> struct is_supported<STR> : set_true {};
		template <> struct is_supported<VEC> : set_true {};
		template <> struct is_supported<MAP> : set_true {};
		// TODO 

		template <typename T, int N = 0> struct nene_type { private: nene_type(); };
		template <> struct nene_type<int> { const static NeneType value = NeneType::INTEGER; using type = int; static void set(NeneValue& target, const int& v) { target.i_value = v; } };
		template <> struct nene_type<float> { const static NeneType value = NeneType::FLOAT; using type = float; static void set(NeneValue& target, const float& v) { target.f_value = v; } };
		template <> struct nene_type<bool> { const static NeneType value = NeneType::BOOLEAN; using type = bool; static void set(NeneValue& target, const bool& v) { target.b_value = v; } };
		template <int N> struct nene_type<char[N]> { const static NeneType value = NeneType::STRING; using type = STR; static void set(NeneValue& target, const char* v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } }; // char* : string
		template <> struct nene_type<pure_type<STR>::type> { const static NeneType value = NeneType::STRING; using type = STR; static void set(NeneValue& target, const STR& v) { if (target.s_pointer == nullptr) target.s_pointer = new STR; *target.s_pointer = v; } };
		template <> struct nene_type<pure_type<VEC>::type> { const static NeneType value = NeneType::VECTOR; using type = VEC; static void set(NeneValue& target, const VEC& v) { if (target.v_pointer == nullptr) target.v_pointer = new VEC; *target.v_pointer = v; } };
		template <> struct nene_type<pure_type<MAP>::type> { const static NeneType value = NeneType::MAP; using type = MAP; static void set(NeneValue& target, const MAP& v) { if (target.m_pointer == nullptr) target.m_pointer = new MAP; *target.m_pointer = v; } };
		// TODO

	public:
		Nene(void)
		{
			Reset();
			type = NeneType::EMPTY;
		}
		Nene(const Nene& rhs)
		{
			Reset();
			switch (rhs.type)
			{
			case NeneType::EMPTY:
			case NeneType::INTEGER:
				Set(rhs.value.i_value);
				break;
			case NeneType::FLOAT:
				Set(rhs.value.f_value);
				break;
			case NeneType::BOOLEAN:
				Set(rhs.value.b_value);
				break;
			case NeneType::STRING:
				Set(*rhs.value.s_pointer);
				break;
			case NeneType::VECTOR:
				Set(*rhs.value.v_pointer);
				break;
			case NeneType::MAP:
				Set(*rhs.value.m_pointer);
				break;
			}
		}
		template <typename T>
		Nene(const T& v) {
			ASSERT_TYPE_NOT_SUPPORTED;
			Reset();
			Set(v);
		}
		template <typename T>
		Nene(const T&& v) {
			ASSERT_TYPE_NOT_SUPPORTED;
			Reset();
			Set(v);
		}

		friend std::ostream& operator<<(std::ostream& os, const Nene& nene)
		{
			switch (nene.type)
			{
			case Nene::NeneType::EMPTY:
				os << "{}";
				break;

			case Nene::NeneType::INTEGER:
				os << nene.value.i_value;
				break;

			case Nene::NeneType::FLOAT:
				os << nene.value.f_value;
				break;

			case Nene::NeneType::BOOLEAN:
				os << nene.value.b_value;
				break;

			case Nene::NeneType::STRING:
				os << "\"" << *nene.value.s_pointer << "\"";
				break;

			case Nene::NeneType::VECTOR:
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

			case Nene::NeneType::MAP:
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

		template <typename T>
		void Set(const T& newVal)
		{
			if (type != nene_type<pure_type<T>::type>::value)
				type = nene_type<pure_type<T>::type>::value;

			nene_type<pure_type<T>::type>::set(value, newVal);
		}
		template <typename T>
		void Set(const T&& newVal)
		{
			if (type != nene_type<pure_type<T>::type>::value)
				type = nene_type<pure_type<T>::type>::value;

			nene_type<pure_type<T>::type>::set(value, newVal);
		}

		void Clear(void)
		{
			switch (type)
			{
			case NeneType::EMPTY:
			case NeneType::INTEGER:
			case NeneType::FLOAT:
			case NeneType::BOOLEAN:
				break;

			case NeneType::STRING:
				if (value.s_pointer != nullptr) delete value.s_pointer;
			case NeneType::VECTOR:
				if (value.v_pointer != nullptr) delete value.v_pointer;
			case NeneType::MAP:
				if (value.m_pointer != nullptr) delete value.m_pointer;
				break;
			}
			memset(&value, 0x00, sizeof(value));
		}
	};

}

#endif