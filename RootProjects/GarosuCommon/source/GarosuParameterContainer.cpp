#include <GarosuTypedef.h>
#include "GarosuParameterContainer.h"

#include <GarosuNene.h>

namespace Garosu
{
	
	class ParameterContainer::impl
	{
	public:
		std::map<String, Nene> params;
	};

	ParameterContainer::ParameterContainer(void)
		: pImpl(mk_uptr<impl>())
	{

	}

	ParameterContainer::~ParameterContainer(void)
	{

	}

	bool ParameterContainer::SetParam(std::string paramName, const bool paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, const void* paramValue)
	{
		pImpl->params[paramName] = (i64)paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, const long long int paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, const double paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::SetParam(std::string paramName, const char* paramValue)
	{
		pImpl->params[paramName] = paramValue;
		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, bool& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<bool>() == false) return false;

		(*param).second.As<bool>(paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, void*& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<i64>() == false) return false;

		(*param).second.As<i64>((i64&)paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, long long int& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<long long>() == false) return false;

		(*param).second.As<long long>(paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, double& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<double>() == false) return false;

		(*param).second.As<double>(paramValue);

		return true;
	}

	bool ParameterContainer::GetParam(std::string paramName, std::string& paramValue)
	{
		auto& param = pImpl->params.find(paramName);
		if (param == pImpl->params.end()) return false;
		if ((*param).second.HasType<std::string>() == false) return false;

		(*param).second.As<std::string>(paramValue);

		return true;
	}

}