#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H

#include <string>
#include <string.h>

#define MAX_CHAT_LEN	256

#define MAX_NAME_LEN	30
#define MAX_COMMENT_LEN	40

enum PacketTypes
{
	PKT_NONE		= 0,
	
	PKT_CS_LOGIN	= 1,
	PKT_SC_LOGIN	= 2,
	
	PKT_CS_CHAT		= 11,
	PKT_SC_CHAT		= 12,

	PKT_MAX	= 1024
} ;

/* simple packet handler */
void packetHandler(std::string packet)
{
	return;
}

#endif