#ifndef TACTICSERRORS_H
#define TACTICSERRORS_H

typedef __int32 ReturnCode;

//For checking success/failure of any function returning type ReturnCode
#define RC_SUCCESS(rc)(((int)(rc)) >= 0)
#define RC_FAILED(rc)(((int)(rc)) < 0)

#define RET_IF_FAILED(rc) {int ret = rc; if(RC_FAILED(ret))return ret;}
#define RET_RC_IF_FAILED(rc, ret) {if(RC_FAILED(rc))return ret;}

#define DEFINE_RC_ERROR(rc)(-rc)

//Success!

#define RC_OK 0x00000000
#define RC_TRUE 0x00000001
#define RC_FALSE 0x00000002

//Failure... :(

#define RC_ERR_GENERAL				DEFINE_RC_ERROR(1)
#define RC_ERR_NOT_IMPLEMENTED		DEFINE_RC_ERROR(2)
#define RC_ERR_INVALID_ARGS			DEFINE_RC_ERROR(3)
#define RC_ERR_INVALID_STATE		DEFINE_RC_ERROR(4)
#define RC_ERR_ELEVATION_REQUIRED	DEFINE_RC_ERROR(5)
#define RC_ERR_TIMED_OUT			DEFINE_RC_ERROR(6)
#define RC_ERR_ACCESS_DENIED		DEFINE_RC_ERROR(7)
#define RC_ERR_ALREADY_EXISTS		DEFINE_RC_ERROR(8)
#define RC_ERR_NOT_FOUND			DEFINE_RC_ERROR(9)

#endif //#ifndef TACTICSERRORS_H