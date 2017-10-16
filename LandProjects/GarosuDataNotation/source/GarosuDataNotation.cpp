#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "GarosuDataNotation.h"

using namespace std;

namespace Garosu
{

	namespace Nene
	{
		void TestFunc(char* fncName)
		{
			//cout << fncName << endl;
		}

		Cat::~Cat(void)
		{
			TestFunc(__FUNCSIG__);
			Clear();
		}

		Cat::Cat(void)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal();
		}
		Cat::Cat(int i)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(i);
		}
		Cat::Cat(float f)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(f);
		}
		Cat::Cat(bool b)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(b);
		}
		Cat::Cat(std::string& str)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(str);
		}
		Cat::Cat(std::vector<Cat>& arr)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(arr);
		}
		Cat::Cat(CatBox& box)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(box);
		}
		Cat::Cat(const Cat& rhs)
		{
			TestFunc(__FUNCSIG__);
			Reset();
			SetVal(rhs);
		}

		Cat& Cat::operator=(int i)
		{
			TestFunc(__FUNCSIG__);
			if (type != CatType::INTEGER)
				Clear();
			SetVal(i);
			return *this;
		}
		Cat& Cat::operator=(float f)
		{
			TestFunc(__FUNCSIG__);
			if (type != CatType::FLOAT)
				Clear();
			SetVal(f);
			return *this;
		}
		Cat& Cat::operator=(bool b)
		{
			TestFunc(__FUNCSIG__);
			if (type != CatType::BOOL)
				Clear();
			SetVal(b);
			return *this;
		}
		Cat& Cat::operator=(std::string& str)
		{
			TestFunc(__FUNCSIG__);
			if (type != CatType::STRING)
				Clear();
			SetVal(str);
			return *this;
		}
		Cat& Cat::operator=(std::vector<Cat>& arr)
		{
			TestFunc(__FUNCSIG__);
			if (type != CatType::ARRAY)
				Clear();
			SetVal(arr);
			return *this;
		}
		Cat& Cat::operator=(CatBox& c)
		{
			TestFunc(__FUNCSIG__);
			if (type != CatType::CATBOX)
				Clear();
			SetVal(c);
			return *this;
		}
		Cat& Cat::operator=(const Cat& rhs)
		{
			TestFunc(__FUNCSIG__);
			if (type != rhs.type)
				Clear();
			SetVal(rhs);
			return *this;
		}

		void Cat::append(int n)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::append(float f)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::append(bool b)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::append(std::string& str)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::append(std::vector<Cat>& arr)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::append(CatBox& c)
		{
			TestFunc(__FUNCSIG__);
		}

		void Cat::operator[](int n)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::operator[](float f)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::operator[](bool b)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::operator[](std::string& str)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::operator[](std::vector<Cat>& arr)
		{
			TestFunc(__FUNCSIG__);
		}
		void Cat::operator[](CatBox& c)
		{
			TestFunc(__FUNCSIG__);
		}

		std::ostream& operator<<(std::ostream& os, const Cat& cat)
		{
			switch (cat.type)
			{
			case Cat::CatType::EMPTY:
				os << "{}";
				break;

			case Cat::CatType::INTEGER:
				os << cat.value.i;
				break;

			case Cat::CatType::FLOAT:
				os << cat.value.f;
				break;

			case Cat::CatType::BOOL:
				os << cat.value.b;
				break;

			case Cat::CatType::STRING:
				os << *cat.value.s;
				break;

			case Cat::CatType::ARRAY:
			{
				auto& arr = *cat.value.a;
				os << "[";
				for (size_t idx = 0; idx < arr.size(); ++idx)
				{
					os << arr[idx];
					if (idx, arr.size() - 1) os << ",";
				}
				os << "]";
			}
			break;

			case Cat::CatType::CATBOX:
				// TODO
				break;
			}
			return os;
		}

		void Cat::Reset(void)
		{
			memset(&value, 0x00, sizeof(value));
		}

		void Cat::Clear(void)
		{
			switch (type)
			{
			case CatType::EMPTY:
				memset(&value, 0x00, sizeof(value));
				break;

			case CatType::INTEGER:
				value.i = 0;
				break;

			case CatType::FLOAT:
				value.f = 0;
				break;

			case CatType::BOOL:
				value.b = 0;
				break;

			case CatType::STRING:
				if (value.s != nullptr)
				{
					delete value.s;
					value.s = nullptr;
				}
				break;

			case CatType::ARRAY:
				if (value.a != nullptr)
				{
					delete value.a;
					value.a = nullptr;
				}
				break;

			case CatType::CATBOX:
				if (value.c != nullptr)
				{
					delete value.c;
					value.c = nullptr;
				}
				break;
			}
		}

		void Cat::SetVal(void)
		{
			type = CatType::EMPTY;
			memset(&value, 0x00, sizeof(value));
		}
		void Cat::SetVal(int i)
		{
			type = CatType::INTEGER;
			value.i = i;
		}
		void Cat::SetVal(float f)
		{
			type = CatType::FLOAT;
			value.f = f;
		}
		void Cat::SetVal(bool b)
		{
			type = CatType::BOOL;
			value.b = b;
		}
		void Cat::SetVal(std::string& str)
		{
			type = CatType::STRING;
			if (value.s == nullptr) value.s = new std::string;
			*value.s = str;
		}
		void Cat::SetVal(std::vector<Cat>& arr)
		{
			type = CatType::ARRAY;
			if (value.a == nullptr) value.a = new std::vector<Cat>;
			*value.a = arr;
		}
		void Cat::SetVal(CatBox& box)
		{
			type = CatType::CATBOX;
			if (value.c == nullptr) value.c = new CatBox;
			*value.c = box;
		}
		void Cat::SetVal(const Cat& cat)
		{
			switch (cat.type)
			{
			case CatType::EMPTY:
				SetVal();
				break;

			case CatType::INTEGER:
				SetVal(cat.value.i);
				break;

			case CatType::FLOAT:
				SetVal(cat.value.f);
				break;

			case CatType::BOOL:
				SetVal(cat.value.b);
				break;

			case CatType::STRING:
				SetVal(*cat.value.s);
				break;

			case CatType::ARRAY:
				SetVal(*cat.value.a);
				break;

			case CatType::CATBOX:
				SetVal(*cat.value.c);
				break;
			}
		}

	}

}