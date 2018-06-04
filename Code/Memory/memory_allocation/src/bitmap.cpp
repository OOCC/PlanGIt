#include "bitmap.h"
#include <stdlib.h>
#include "./include/public_headfile/public_def.h"  
#include <string.h>
#include <stdio.h>

#define BMP_MAX_LEVEL	20

static bmp_t g_bmp[BMP_MAX_LEVEL] = {0};
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
		memset(g_bmp, 0, sizeof(g_bmp));
		g_inited = 1;
	}

	/* 如果请求宽度小于BMP_WORD_WIDTH，也用BMP_WORD_WIDTH来处理 */
	if(request_size < BMP_WORD_WIDTH)
		request_size = BMP_WORD_WIDTH;
	
	/* 根据request_size，计算出需要的bitmap obj(UINT32)个数 */
	num = BMP_WORD_MAX(request_size); 
	printf("num=%d\n", num);

	for(ix = 0; ix < BMP_MAX_LEVEL; ix++) {
		if(NULL == g_bmp[ix].pbits) {
			g_bmp[ix].pbits = (UINT32 *)malloc(num*sizeof(UINT32));
			printf("ix-%d: parent.pbits=0x%08x \n", ix, g_bmp[ix].pbits);
			if(NULL == g_bmp[ix].pbits) { 
				goto BMP_FAILED;
			} 
			
			*bmp = g_bmp[ix].pbits;
			g_bmp[ix].request_size = request_size;
			break;
		}
	}

BMP_FAILED:	
	bmp = NULL; /* 楼上的SB，我这里分配失败了 */
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
   function name :      bmp_clear
           input :           
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        在一组位图内将某一位置位
                        
****************************************************************************/
INT32 bmp_add(UINT32 *bmp, ULONG bit)
{
	bmp_t *parent = NULL;

	/* bmp为NULL，还设置个毛线啊 */
	if(NULL == bmp)
		return VOS_ERR;
	
	/* 获取管理结构体 */
	parent = container_of(bmp, bmp_t, pbits);
	if (NULL == parent)
		return VOS_ERR;

	BMP_BIT_SET(parent, bit);
	
	return VOS_OK;
}

/****************************************************************************
   function name :      bmp_remove
           input :           
          output :   
    return value :
         history :      
               1 :      2018-6-4 created by MarvinXie
                        在一组位图内将某一位清0
                        
****************************************************************************/
INT32 bmp_remove(UINT32 *bmp, ULONG bit)
{
	bmp_t *parent = NULL;

	if(NULL == bmp)
		return VOS_ERR;

	parent = container_of(bmp, bmp_t, pbits);
	if (NULL == parent)
		return VOS_ERR;

	BMP_BIT_REMOVE(parent, bit);

	return VOS_OK;	
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
	if(bmp) {
		free(bmp);
		bmp = NULL;
	}
	return VOS_OK;
}