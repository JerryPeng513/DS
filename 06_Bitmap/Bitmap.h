/* Bitmap.h */
#ifndef _BITMAP_H
#define _BITMAP_H

typedef struct
{
	int capacity;
	char *content;
}BITMAP; 

//Bitmap初始化
void BitmapNew(BITMAP *map, int capacity);
//获取Bitmap位数
int BitmapCapacity(BITMAP *map);
//Bitmap销毁
void BitmapDispose(BITMAP *map);
//将第i位置为1
void BitmapSet(BITMAP *map, int i);
//将第i位置为0
void BitmapClear(BITMAP *map, int i);
//测试第i位是否为1
int BitmapTest(BITMAP *map, int i);
//Bitmap复制操作
int BitmapCopy(BITMAP *mapN, BITMAP *mapO);
#endif