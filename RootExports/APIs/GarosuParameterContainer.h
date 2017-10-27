#pragma once
#ifndef __GAROSU_PARAMETER_CONTAINER_H__
#define __GAROSU_PARAMETER_CONTAINER_H__

#include <memory>

namespace Garosu
{

	class ParameterContainer
	{
	public:
		ParameterContainer(void);
		virtual ~ParameterContainer(void);

		bool SetParam(std::string paramName, const bool paramValue);
		bool SetParam(std::string paramName, const void* paramValue);
		bool SetParam(std::string paramName, const long long int paramValue);
		bool SetParam(std::string paramName, const double paramValue);
		bool SetParam(std::string paramName, const char* paramValue);

		bool GetParam(std::string paramName, bool& paramValue);
		bool GetParam(std::string paramName, void*& paramValue);
		bool GetParam(std::string paramName, long long int& paramValue);
		bool GetParam(std::string paramName, double& paramValue);
		bool GetParam(std::string paramName, std::string& paramValue);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}

#endif
