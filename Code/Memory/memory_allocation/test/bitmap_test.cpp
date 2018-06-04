// bitmap_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "bitmap.h"
#include "./include/public_headfile/public_def.h"

int _tmain(int argc, _TCHAR* argv[])
{
	UINT32 *bitmap[10] = {0};
	INT32 ix = 0;
	ULONG request_size = 1024*1024/32;
	bmp_t *parent = NULL;
	char key;

	for (ix = 0; ix < 10; ix++) {
		bmp_create(&bitmap[ix], request_size);
		request_size /= 2;
	}

	for (ix = 0; ix < 10; ix++) {
		parent = container_of(bitmap[ix], bmp_t, pbits); 
		printf("ix-%d: parent.pbits=0x%08x pbits.request_size=%llx\n", ix, parent->pbits, parent->request_size);
	}

	scanf("%c", &key);

	for (ix = 0; ix < 10; ix++) {
		parent = container_of(bitmap[ix], bmp_t, pbits); 
		bmp_delete(parent->pbits);
	}
	return 0;
}

