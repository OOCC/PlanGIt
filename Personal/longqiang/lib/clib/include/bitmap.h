/**
 * @author Long Qiang
 * @date 2018/1/5 15:26:09
 */
#pragma once

#include <stdint.h>


/*
 *8 ^ 10 = 1,073,741,824
 *should be enough
 */
typedef struct bitmap_s bitmap_t;

/**
 * @brief alloc a bitmap
 * @author Long Qiang
 * @date 2018/1/5 15:47:48
 */
extern bitmap_t* bitmap_alloc(const uint32_t num);
/**
 * @brief destory a bimap
 * @author Long Qiang
 * @date 2018/1/5 15:48:28
 */
extern void bitmap_destory(bitmap_t *bmap);
/**
 * @brief set [bn] bit as 1
 * @author Long Qiang
 * @date 2018/1/5 15:48:44
 */
extern void bitmap_bit_unset(bitmap_t * pb, uint32_t bn);
/**
 * @brief set [bn] bit as 0
 * @author Long Qiang
 * @date 2018/1/5 15:49:24
 */
extern void bitmap_bit_set(bitmap_t *pb, uint32_t bn);
/**
 * @brief alloc a free bit in [pb]
 * @note  after alloc, alloced bit will set 1
 * @return -1 if alloc failed
 * @author Long Qiang
 * @date 2018/1/5 15:49:37
 */
extern uint32_t bitmap_bit_alloc(bitmap_t *pb);



