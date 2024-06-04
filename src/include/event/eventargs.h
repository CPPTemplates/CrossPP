#pragma once

template<typename senderType>
struct eventArgs
{
	senderType& sender;
	eventArgs(senderType& sender) : sender(sender)
	{

	}
};