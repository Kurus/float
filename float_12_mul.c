#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h> 

union float32 {
    struct {
        unsigned int man : 23;
        unsigned int exp : 8;
        unsigned int sgn : 1;
    } r;
    float f;
};

float b2f(char a){
	union float32 aa;
	aa.r.sgn=(a&0x80)>>7;
	aa.r.exp=(((a&0x7c)>>2)+112);
	aa.r.man=(a&0x3)<<21;
	return aa.f;
}

char f2b(float a){
	union float32 aa;
	aa.f=a;
	return aa.r.sgn<<7 | (aa.r.exp-112)<<2 | aa.r.man>>21;
}

void mult(char a, char b, char* c, char en){
	static char arr[10]={0};
	static int cnt=0;
	static int rd=0;
	arr[cnt] = f2b(b2f(a)*b2f(b));
	cnt++;
	cnt=cnt%10;
	if(en){
		*c=arr[rd];
		rd++;
		rd=rd%10;
	}
}

int main()
{
	char ans;
	for (int i = 0; i < 10; ++i)
	{
		mult(61,60+i,&ans,i%2);
		printf("%d\n", ans);
	}
	
	return 0;
}