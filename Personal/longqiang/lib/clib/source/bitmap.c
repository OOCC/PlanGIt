#include "libcommon.h"
#include "bitmap.h"

#define BIT_SIZE        (sizeof(BIT_TYPE)<<3)
#define LAYER_MAX_NUM   10
#define BIT_TYPE        uint8_t

struct bitmap_s
{
    uint32_t        totalcount;
    uint32_t        deep;
    uint32_t        width[LAYER_MAX_NUM];
    BIT_TYPE        *bitmap[LAYER_MAX_NUM];
} ;

#ifdef __x86_64__
static inline
uint bit_offset(uint8_t nt)
{
    uint n = (0x000000FFUL & (~nt));
    __asm__ __volatile__("bsf %1,%0":"=r"(n):"r"(n));
    return n;

}
#endif


static
uint deep_calc(uint block_num, bitmap_t *pb)
{
    uint deep_tmp = 0;
    uint bitoff;

    do
    {
        bitoff = block_num % BIT_SIZE;
        block_num = (block_num / BIT_SIZE) + ((block_num % BIT_SIZE) ? 1 : 0);

        pb->bitmap[deep_tmp] = malloc(block_num);

        memset(pb->bitmap[deep_tmp], 0, block_num);

        pb->bitmap[deep_tmp][block_num - 1] =
            (0xFF << (bitoff == 0 ? 8 : bitoff));

        pb->width[deep_tmp] = block_num;

        ++deep_tmp;

    } while (block_num != 1);

    pb->deep = deep_tmp;

    return 0;
}


bitmap_t* bitmap_alloc(const uint num)
{
    bitmap_t *p = NULL;


    if (num)
        p = malloc(sizeof(bitmap_t));
    else
        return NULL;

    if (p && (0 == deep_calc(num, p)))
    {
        p->totalcount = num;
        return p;
    }
    else
    {
        return NULL;
    }

}


void bitmap_bit_unset(bitmap_t *pb, uint bn)
{
    uint            deep = 0;
    uint            width = bn;
    uint8_t         bitoff;


    if (bn > pb->totalcount)
    {
        return ;
    }

    do
    {
        bitoff = width % BIT_SIZE ;
        width = width / BIT_SIZE ;

        if (pb->bitmap[deep][width] == 0XFF)
        {
            pb->bitmap[deep][width] &= (uint8_t)(~ (0x1 << bitoff));
        }
        else
        {
            pb->bitmap[deep][width] &= (uint8_t)(~ (0x1 << bitoff));
            break;
        }

        deep++;
    } while (deep != pb->deep);
}


void bitmap_bit_set(bitmap_t *pb, uint bn)
{
    uint        deep = 0;
    uint        width = bn;
    uint8_t     bitoff;


    if (bn > pb->totalcount)
    {
        return ;
    }

    do
    {
        bitoff = width % BIT_SIZE ;
        width = width / BIT_SIZE ;

        pb->bitmap[deep][width] |= (0x1 << bitoff);

        if (pb->bitmap[deep][width] != 0XFF)
        {
            break;
        }

        deep++;

    } while (deep != pb->deep);

}


uint bitmap_bit_alloc(bitmap_t *pb)
{
    uint        deep = pb->deep;
    uint        width = 0;
    uint8_t     u8 = 0;


    if (0xFF == (pb->bitmap[pb->deep - 1][0]))
    {
        return -1;
    }
    while (deep--)
    {
        u8 = pb->bitmap[deep][width];
        width = width * BIT_SIZE + bit_offset(u8);
    }

    bitmap_bit_set(pb, width);

    return width;
}


void bitmap_destory(bitmap_t *bmap)
{
    if (bmap)
    {
        while (bmap->deep--)
        {
            free(bmap->bitmap[bmap->deep]);
        }

        free(bmap);

    }
}
