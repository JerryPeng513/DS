/* Bitmap.c */
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "Bitmap.h"

//Bitmap初始化
void BitmapNew(BITMAP *map, int capacity)
{
	assert(capacity > 0);
	map->capacity = capacity;
	int charCount = (capacity + 7) / 8;
	map->content = (char *)malloc(charCount);
	assert(map->content);
	memset(map->content, 0, charCount);
}

int BitmapCapacity(BITMAP *map)
{
	return map->capacity;
}

//Bitmap销毁
void BitmapDispose(BITMAP *map)
{
	free(map->content);
}

//将第i位置为1
void BitmapSet(BITMAP *map, int i)
{
	if (i <= 0 || i > BitmapCapacity(map))
	{
		return ;
	}
	map->content[i >> 3] |= (0x80 >> (i & 0x07));
}

//将第i位置为0
void BitmapClear(BITMAP *map, int i)
{
	if (i <= 0 || i > BitmapCapacity(map))
	{
		return ;
	}
	map->content[i >> 3] &= ~(0x80 >> (i & 0x07));
}

//测试第i位是否为1
int BitmapTest(BITMAP *map, int i)
{
	if (i <= 0 || i > BitmapCapacity(map))
	{
		return 0;
	}
	return map->content[i >> 3] & (0x80 >> (i & 0x07));
}

//Bitmap复制操作
int BitmapCopy(BITMAP *mapN, BITMAP *mapO)
{
	int charCountN = (mapN->capacity + 7) / 8;
	int charCountO = (mapO->capacity + 7) / 8;
	if (charCountN <= charCountO)
	{
		memcpy(mapN->content, mapO->content, charCountN);
	}
	else
	{
		memcpy(mapN->content, mapO->content, charCountO);
		memset(mapN->content + charCountO, 0, charCountN - charCountO);
	}
}