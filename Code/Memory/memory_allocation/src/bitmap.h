
#ifndef _BITMAP_H
#define _BITMAP_H

typedef struct bmp {
    UINT32 *pbits;
	ULONG   request_size;
} bmp_t;

#define BMP_BIT_MAX 10000  //for test

#define	BMP_WORD_WIDTH		32
#define	BMP_WIDTH(m)		(((m + BMP_WORD_WIDTH - 1)/BMP_WORD_WIDTH)*BMP_WORD_WIDTH)
#define	BMP_WORD_MAX(m)		\
            ((BMP_WIDTH(m)+ BMP_WORD_WIDTH-1) / BMP_WORD_WIDTH)
        
#define	BMP_WORD_GET(bm, word)		((bm)->pbits[(word)])
#define	BMP_WORD_SET(bm, word, val)	((bm)->pbits[(word)] = (val))

#define	BMP_WENT(bit)		((bit)/BMP_WORD_WIDTH)
#define	BMP_WBIT(bit)		(1U<<((bit) % BMP_WORD_WIDTH))

/* 这个宏定义还存在BUG，其遍历的上限是定死的（BMP_WORD_MAX） */
#define BMP_BMCLEAR(bm)		do { \
		INT32	_w; \
		for (_w = 0; _w < BMP_WORD_MAX; _w++) { \
			BMP_WORD_GET(bm, _w) = 0; \
		} \
	} while (0)


/* generics that use the previously defined helpers */
#define BMP_CLEAR(bm)		BMP_BMCLEAR(bm)
#define BMP_ITER(bm, bit)	\
            for ((bit) = 0; (bit) < BMP_BIT_MAX; (bit)++) \
                if (BMP_MEMBER((bm), (bit)))

//#define BMP_BMCLEAR(bm)		\
//	(BMP_WORD_GET(bm, 0) = BMP_WORD_GET(bm, 1) = 0)
#define BMP_BMNULL(bm)		\
	(BMP_WORD_GET(bm, 0) == 0 && BMP_WORD_GET(bm, 1) == 0)
#define BMP_BMEQ(bma, bmb)	\
	((BMP_WORD_GET(bma, 0) == BMP_WORD_GET(bmb, 0)) && \
	 (BMP_WORD_GET(bma, 1) == BMP_WORD_GET(bmb, 1)))
#define BMP_BMOP(bma, bmb, op)	do { \
	BMP_WORD_GET(bma, 0) op BMP_WORD_GET(bmb, 0); \
	BMP_WORD_GET(bma, 1) op BMP_WORD_GET(bmb, 1); \
	} while (0)

        
#define BMP_IS_NULL(bm)			(BMP_BMNULL(bm))
#define BMP_NOT_NULL(bm)		(!BMP_BMNULL(bm))
#define BMP_EQ(bma, bmb)		(BMP_BMEQ(bma, bmb))
#define BMP_NEQ(bma, bmb)		(!BMP_BMEQ(bma, bmb))
        
/* Assignment operators */
#define BMP_ASSIGN(dst, src)	(dst) = (src)
#define BMP_AND(bma, bmb)		BMP_BMOP(bma, bmb, &=)
#define BMP_OR(bma, bmb)		BMP_BMOP(bma, bmb, |=)
#define BMP_XOR(bma, bmb)		BMP_BMOP(bma, bmb, ^=)
#define BMP_REMOVE(bma, bmb)	BMP_BMOP(bma, bmb, &= ~)
#define BMP_NEGATE(bma, bmb)	BMP_BMOP(bma, bmb, = ~)
        
/* BIT BMP operators */
#define	BMP_ENTRY(bm, bit)	\
            (BMP_WORD_GET(bm,BMP_WENT(bit)))
#define BMP_MEMBER(bm, bit)	\
            ((BMP_ENTRY(bm, bit) & BMP_WBIT(bit)) != 0)
/*#define BMP_BIT_SET(bm, bit)	do { \
                BMP_CLEAR(bm); \
                BMP_BIT_ADD(bm, bit); \
            } while(0)*/
#define BMP_BIT_SET(bm, bit)	do { \
                BMP_BIT_ADD(bm, bit); \
            } while(0)
#define BMP_BIT_ADD(bm, bit)	\
            (BMP_ENTRY(bm, bit) |= BMP_WBIT(bit))
#define BMP_BIT_REMOVE(bm, bit)	\
            (BMP_ENTRY(bm, bit) &= ~BMP_WBIT(bit))
#define BMP_BIT_FLIP(bm, bit)	\
            (BMP_ENTRY(bm, bit) ^= BMP_WBIT(bit))

INT32 bmp_create(UINT32 **bmp, ULONG request_size);
INT32 bmp_set(UINT32 *bmp, ULONG bit);
INT32 bmp_get(UINT32 *bmp, ULONG bit);
INT32 bmp_clear(UINT32 *bmp);
INT32 bmp_add(UINT32 *bmp, ULONG bit);
INT32 bmp_remove(UINT32 *bmp, ULONG bit);
INT32 bmp_delete(UINT32 *bmp);


			
#endif	/* ! BMP_H */

