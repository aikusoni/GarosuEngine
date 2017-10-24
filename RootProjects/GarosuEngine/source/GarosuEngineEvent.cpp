#include <string>

#include "GarosuEngineEvent.h"

__declspec(dllexport) bool GetEventId(Garosu::BaseEvent* eventInstance, Garosu::EngineEventId* eventId)
{
	if (eventInstance == nullptr) return false;
	if (eventId == nullptr) return false;

	*eventId = eventInstance->mEventId;

	return true;
}

__declspec(dllexport) bool GetStringSize(Garosu::StringEvent* eventInstance, size_t* pNeedBufferSize)
{
	if (eventInstance == nullptr) return false;
	if (pNeedBufferSize == nullptr) return false;
	
	*pNeedBufferSize = eventInstance->mStr.size();

	return true;
}

__declspec(dllexport) bool GetString(Garosu::StringEvent* eventInstance, char* buffer, size_t bufferSize)
{
	if (eventInstance == nullptr) return false;
	if (bufferSize <= 1) return false;

	size_t copySize = eventInstance->mStr.size() < bufferSize ? eventInstance->mStr.size() : bufferSize;
	strncpy(buffer, eventInstance->mStr.c_str(), copySize);
	buffer[bufferSize - 1] = 0;

	return true;
}