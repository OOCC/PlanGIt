#pragma once

#define RET_OK                  0
#define RET_ERROR               -1
#define RET_NULL_PTR            -2
#define RET_NOT_EXIST           -3
#define RET_EXIST               -4
#define RET_EMPTY               -5
#define RET_FULL                -6
#define RET_BUSY                -7
#define RET_IDLE                -8
#define RET_SYS                 -9
#define RET_INIT                -10
#define RET_RESOURCE             -11

#define RET_FALIURE(ret)        ((ret) < 0)
#define RET_SUCCESS(ret)        ((ret) >= 0)

/**
 * @brief code to error string
 * @return
 * @author Long Qiang
 * @date 2018/2/21 20:38:21
 */
const char* rc2msg(const int code);

