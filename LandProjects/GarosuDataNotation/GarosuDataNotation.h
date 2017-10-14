#pragma once
#ifndef __GAROSU_DATA_NOTATION_H__
#define __GAROSU_DATA_NOTATION_H__

namespace Garosu
{

	namespace Nene
	{

		class Cat
		{
			using CatBox = std::unordered_map<std::string, Cat>;

		public:
			virtual ~Cat(void);

			Cat(void);
			Cat(int i);
			Cat(float f);
			Cat(bool b);
			Cat(std::string& str);
			Cat(std::vector<Cat>& arr);
			Cat(CatBox& box);
			Cat(const Cat& rhs);

			Cat& operator=(int i);
			Cat& operator=(float f);
			Cat& operator=(bool b);
			Cat& operator=(std::string& str);
			Cat& operator=(std::vector<Cat>& arr);
			Cat& operator=(CatBox& box);
			Cat& operator=(const Cat& rhs);

			// [ 1, 2, "blah blah", true ]
			void append(int i);
			void append(float f);
			void append(bool b);
			void append(std::string& str);
			void append(std::vector<Cat>& arr);
			void append(CatBox& box);

			// { "age" : 1, "money" : "infinite", "name" : "platinum spoon"  }
			void operator[](int i);
			void operator[](float f);
			void operator[](bool b);
			void operator[](std::string& str);
			void operator[](std::vector<Cat>& arr);
			void operator[](CatBox& box);

		private:
			union
			{
				int i;
				float f;
				bool b;
				std::string* s;
				std::vector<Cat>* a;
				std::unordered_map<std::string, Cat>* c;
			} value;

			enum class CatType
			{
				EMPTY = 0,
				INTEGER = 1 << 0,
				FLOAT = 1 << 1,
				BOOL = 1 << 2,
				ARRAY = 1 << 3,
				STRING = 1 << 4,
				CATBOX = 1 << 5
			} type;

		private:
			void Clear(void);

			void SetVal(void);
			void SetVal(int i);
			void SetVal(float f);
			void SetVal(bool b);
			void SetVal(std::string& str);
			void SetVal(std::vector<Cat>& arr);
			void SetVal(CatBox& box);
		};

	}

}

#endif