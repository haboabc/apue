#ifndef PROTO_H__
#define PROTO_H__


#define KEYPATH		"/etc/hosts"
#define KEYPROJ		'A'

#define PATHSIZE	128	
#define DATASIZE	128

enum
{
	MSG_PATH=1,
	MSG_S2C
};

struct msg_path_st
{
	long mtype;				/*must be MSG_PATH*/
	char path[PATHSIZE];	/*ASCIIZ带尾0的串*/
};

struct msg_s2c_st
{
	long mtype;				/*must be MSG_S2C*/
	int datalen;
	/*
 *	datalen > 0			data
 *			= 0			eot
 *			< 0			-errno
 * */

	char data[DATASIZE];
};


#endif




