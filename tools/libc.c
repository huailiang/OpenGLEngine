/**
此c语言接口是给python调用

make so:
  gcc -o libc.so -shared -fPIC libc.c

**/  

#include <stdio.h>  
#include <stdlib.h>  



/*
python 中int不会溢出，所以使用c实现了一遍
*/
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


/*
int main()
{
	char* str = "resource/obj/sui/export hello world.png";
	printf("hash: %ud\n", hash(str, 39));
	return 0;
}
*/