#ifndef PROTO_H__
#define PROTO_H__

#define RCVPORT		"65535"	

#define MSGMAX		(512-8) //控制包的大小

#define NAMEMAX		(MSGMAX-8)

struct msg_st
{
	uint32_t math;
	uint32_t chinese;
	uint8_t name[1];
}__attribute__((packed));



#endif



