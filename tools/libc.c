/***gcc -o libpycall.so -shared -fPIC pycall.c*/  
#include <stdio.h>  
#include <stdlib.h>  


unsigned int hash(char* str,int num)
{
	unsigned int hash = 0;
	int seed = 5;
	for(int i=0;i<num;i++)
	{
		hash = (hash<<5)+ (*(str+i)) + hash;
		// printf("%d %c, hash:%ud, \n", i, str[i], hash );
	}
	return hash;
}



int main()
{
	char* str = "resource/obj/sui/export hello world.png";
	printf("hash: %d\n", hash(str, 39));
	return 0;
}