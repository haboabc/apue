#ifndef PROTO_H__
#define PROTO_H__


#define KEYPATH		"/etc/hosts"
#define KEYPROJ		'A'

#define PATHSIZE	128	
#define DATASIZE	128

enum
{
	MSG_PATH=1,
	MSG_DATA,
	MSG_ERR,
	MSG_EOT
};

struct msg_path_st
{
	long mtype;				/*must be MSG_PATH*/
	char path[PATHSIZE];	/*ASCIIZ带尾0的串*/
};

struct msg_data_st
{
	long mtype;				/*must be MSG_DATA*/
	int datalen;
	char data[DATASIZE];
};

struct msg_err_st
{
	long mtype;				/*must be MSG_ERR*/
	int errno;
};

struct msg_eot_st
{
	long mtype;				/*must be MSG_EOT*/
};


#endif




