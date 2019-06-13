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



// int main()
// {
// 	char* str = "engine/halo/summary.sum";
// 	int i=0;
// 	while(*(str+i)!='\0')
// 	{
// 		printf("%d %c\n", i, *(str+i));
// 		if(i>100) break;
// 		i++;
// 	}
// 	printf("hash: %d, %ud\n", i,hash(str, 23));
// 	return 0;
// }
