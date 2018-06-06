#include <stdlib.h>
#include "type_def.h"
#include "public_def.h"  
#include <string.h>
#include <stdio.h>
#include "bitmap.h"

#define BMP_MAX_LEVEL	20

static bmp_t g_bitmap[BMP_MAX_LEVEL] = {0};
static INT32 g_inited = 0;

/****************************************************************************
   function name :      bmp_create
           input :      request_size     
          output :   	bmp
    return value :		INT32
         history :      
               1 :      2018-6-4 created by MarvinXie
                        创建位图
                        
****************************************************************************/
INT32 bmp_create(UINT32 **bmp, ULONG request_size)
{
	INT32 ix;
	INT32 num = 0;
	
	/* For VS2010 C++ Project Test! */
	if(!g_inited){
		memset(g_bitmap, 0, sizeof(g_bitmap));
		g_inited = 1;
	}

	/* 如果请求宽度小于BMP_WORD_WIDTH，也用BMP_WORD_WIDTH来处理 */
	if(request_size < BMP_WORD_WIDTH)
		request_size = BMP_WORD_WIDTH;
	
	/* 根据request_size，计算出需要的bitmap obj(UINT32)个数 */
	num = BMP_WORD_MAX(request_size); 

	for(ix = 0; ix < BMP_MAX_LEVEL; ix++) {
		if(NULL == g_bitmap[ix].pbits) {
			g_bitmap[ix].pbits = (UINT32 *)malloc(num*sizeof(UINT32));
			if(NULL == g_bitmap[ix].pbits) { 
				goto BMP_CREATE_FAILED;
			} 
			memset(g_bitmap[ix].pbits, 0, num * sizeof(UINT32));

			*bmp = g_bitmap[ix].pbits;
			g_bitmap[ix].request_size = request_size;
			break;
		}
	}

BMP_CREATE_FAILED:	
	*bmp = NULL; /* 楼上的SB，我这里分配失败了 */
	return VOS_ERR;
}

/****************************************************************************
   function name :      bmp_set
           input :      bmp，bit    
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        设置一组位图，如位图宽度为32bits，则本接口直接操作整个32bits
                        
****************************************************************************/
INT32 bmp_set(UINT32 *bmp, ULONG bit)
{
	return VOS_OK;
}

/****************************************************************************
function name :      bmp_set
input :      bmp，bit
output :
return value :
history :
1 :      2018-6-4 created by MarvinXie
设置一组位图，如位图宽度为32bits，则本接口直接操作整个32bits
****************************************************************************/
INT32 bmp_get(UINT32 *bmp, ULONG bit)
{
	return VOS_OK;
}

/****************************************************************************
   function name :      bmp_clear
           input :      bmp     
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        清除一组位图，如位图宽度为32bits，则本接口直接操作整个32bits
                        
****************************************************************************/
INT32 bmp_clear(UINT32 *bmp)
{
	return VOS_OK;
}

/****************************************************************************
   function name :      bmp_set_bit
           input :           
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        在一组位图内将某一位置位
                        
****************************************************************************/
INT32 bmp_set_bit(UINT32 *bmp, ULONG bit)
{
	INT32 ix = 0;

	/* bmp为NULL，还设置个毛线啊 */
	if(NULL == bmp)
		return VOS_ERR;
	
    for (ix = 0; ix < BMP_MAX_LEVEL; ix++) {
        if (bmp == g_bitmap[ix].pbits) {
			if (bit >= g_bitmap[ix].request_size) /* 兄弟，越界了 */
				return VOS_ERR;
            BMP_BIT_ADD(g_bitmap[ix], bit);
            return VOS_OK;
        }
    }

	return VOS_ERR;
}

/****************************************************************************
   function name :      bmp_clear_bit
           input :           
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        在一组位图内将某一位清0
                        
****************************************************************************/
INT32 bmp_clear_bit(UINT32 *bmp, ULONG bit)
{
	INT32 ix = 0;

    if(NULL == bmp)
    		return VOS_ERR;
    	
    for (ix = 0; ix < BMP_MAX_LEVEL; ix++) {
        if (bmp == g_bitmap[ix].pbits) {
			if (bit >= g_bitmap[ix].request_size)
				return VOS_ERR;			
            BMP_BIT_REMOVE(g_bitmap[ix], bit);
            return VOS_OK;
        }
    }

	return VOS_ERR;
}


/****************************************************************************
   function name :      bmp_get_bit_bit
           input :           
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        获取位图中指定位的值
                        
****************************************************************************/
INT32 bmp_get_bit(UINT32 *bmp, ULONG bit)
{
	INT32 ix = 0;
    INT32 ret = 0;

	if(NULL == bmp) 
        return VOS_ERR;

     for (ix = 0; ix < BMP_MAX_LEVEL; ix++) {
        if (bmp == g_bitmap[ix].pbits) {
			if (bit >= g_bitmap[ix].request_size)
				return VOS_ERR;			
			ret = BMP_BIT_GET(g_bitmap[ix], bit);
            return ret;
        }
    }   

    return VOS_ERR;	
}


/****************************************************************************
   function name :      bmp_delete
           input :      bmp     
          output :   	
    return value :		INT32
         history :      
               1 :      2018-6-4 created by MarvinXie
                        释放位图
                        
****************************************************************************/
INT32 bmp_delete(UINT32 *bmp)
{
	INT32 ix = 0;

	if(bmp) {
        for (ix = 0; ix < BMP_MAX_LEVEL; ix++) {
            if (bmp == g_bitmap[ix].pbits) {
                free(g_bitmap[ix].pbits);
                g_bitmap[ix].request_size = 0;
                return VOS_OK;
            }
        }
    }

	return VOS_ERR;	
}


bmp_t *bmp_get_bit_parent(void)
{
    return g_bitmap;
}