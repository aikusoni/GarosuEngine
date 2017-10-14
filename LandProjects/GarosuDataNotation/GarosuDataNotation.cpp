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
			cout << fncName << endl;
		}

		Cat::~Cat(void)
		{
			TestFunc(__FUNCSIG__);
		}

		Cat::Cat(void)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::EMPTY;
			memset(&value, 0x00, sizeof(value));
		}
		Cat::Cat(int i)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::INTEGER;
			value.i = i;
		}
		Cat::Cat(float f)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::FLOAT;
			value.f = f;
		}
		Cat::Cat(bool b)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::BOOL;
			value.b = b;
		}
		Cat::Cat(std::string& str)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::STRING;
			std::string* s = new std::string(str);
			value.s = s;
		}
		Cat::Cat(std::vector<Cat>& arr)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::ARRAY;
			auto a = new std::vector<Cat>(arr);
			value.a = a;
		}
		Cat::Cat(CatBox& box)
		{
			TestFunc(__FUNCSIG__);
			type = CatType::CATBOX;
			CatBox* c = new CatBox(box);
			value.c = c;
		}
		Cat::Cat(const Cat& rhs)
		{
			TestFunc(__FUNCSIG__);
		}

		Cat& Cat::operator=(int n)
		{
			TestFunc(__FUNCSIG__);
			return *this;
		}
		Cat& Cat::operator=(float f)
		{
			TestFunc(__FUNCSIG__);
			return *this;
		}
		Cat& Cat::operator=(bool b)
		{
			TestFunc(__FUNCSIG__);
			return *this;
		}
		Cat& Cat::operator=(std::string& str)
		{
			TestFunc(__FUNCSIG__);
			return *this;
		}
		Cat& Cat::operator=(std::vector<Cat>& arr)
		{
			TestFunc(__FUNCSIG__);
			return *this;
		}
		Cat& Cat::operator=(CatBox& c)
		{
			TestFunc(__FUNCSIG__);
			return *this;
		}
		Cat& Cat::operator=(const Cat& rhs)
		{
			TestFunc(__FUNCSIG__);
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

	}

}