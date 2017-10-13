#include <string>

#include "GarosuEngineMessage.h"

__declspec(dllexport) Garosu::BaseMessage* MakeBaseMessage(Garosu::EngineMessageId messageId)
{
	return new Garosu::BaseMessage(messageId);
}

__declspec(dllexport) Garosu::StringMessage* MakeStringMessage(Garosu::EngineMessageId messageId, char * charArray)
{
	return new Garosu::StringMessage(messageId, std::string(charArray));
}

__declspec(dllexport) bool DeleteMessage(Garosu::BaseMessage* message)
{
	if (message != nullptr)
	{
		delete message;
		return true;
	}
	return false;
}