/* BitmapTest.c */
#include <stdio.h>
#include "Bitmap.h"

int main()
{
	BITMAP map;
	BitmapNew(&map, 10);
	printf("map capacity is %d\n", BitmapCapacity(&map));
	int i = 1;
	for (; i <= 10; i ++)
	{
		if (BitmapTest(&map, i))
		{
			printf("map %d th bit is 1\n", i);
		}
		else
		{
			printf("map %d th bit is 0\n", i);			
		}
	}
	for (i = 1; i <= 10; i ++)
	{
		BitmapSet(&map, i);
	}
	for (i = 1; i <= 10; i ++)
	{
		if (BitmapTest(&map, i))
		{
			printf("map %d th bit is 1\n", i);
		}
		else
		{
			printf("map %d th bit is 0\n", i);			
		}
	}
	BITMAP mapN;
	BitmapNew(&mapN, 11);
	BitmapCopy(&mapN, &map);
	for (i = 1; i <= 11; i ++)
	{
		if (BitmapTest(&mapN, i))
		{
			printf("mapN %d th bit is 1\n", i);
		}
		else
		{
			printf("mapN %d th bit is 0\n", i);			
		}
	}
	for (i = 1; i <= 10; i ++)
	{
		BitmapClear(&map, i);
	}
	for (i = 1; i <= 10; i ++)
	{
		if (BitmapTest(&map, i))
		{
			printf("map %d th bit is 1\n", i);
		}
		else
		{
			printf("map %d th bit is 0\n", i);			
		}
	}
	BitmapDispose(&map);
	BitmapDispose(&mapN);		
	return 0;
}