#include <libcommon.h>
#include <utils.h>

#include <thread.h>

typedef struct error_msg_s
{
    const char *msg;
    int         error_no;
} error_msg_t;


static error_msg_t error_msg[] =
{
    {"success",         RET_OK},
    {"error",           RET_ERROR},
    {"null pointer",    RET_NULL_PTR},
    {"not exist",       RET_NOT_EXIST},
    {"exist",           RET_EXIST},
    {"empty",           RET_EMPTY},
    {"full",            RET_FULL},
    {"busy",            RET_BUSY},
    {"idle",            RET_IDLE},
    {"system error",    RET_SYS},
    {"uninitialized",   RET_INIT},
    {"no resources",    RET_RESOURCE},
};


const char* rc2msg(const int code)
{
    int     i, num;


    num = ARRAR_COUNT(error_msg);

    for (i = 0; i < num; ++i)
    {
        if (code == error_msg[i].error_no)
        {
            return error_msg[i].msg;
        }
    }

    return "unknow error";
}
