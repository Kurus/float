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
	aa.r.man=(a&0x3)<<21;
	if(((a&0x7c)>>2) == 0){
		aa.r.exp=0;
	}
	else{
		aa.r.exp=(((a&0x7c)>>2)+112);
	}
	return aa.f;
}

char f2b(float a){
	union float32 aa;
	aa.f=a;
	char x = (aa.r.exp-112);
	if(x<0)return 0;
	if(x>31)x=31;
	return aa.r.sgn<<7 | x<<2 | aa.r.man>>21;
}

void mult(char a, char b, char* c,char* a_out, char* b_out, char en){
	static char arr[10]={0};
	static char aa[10]={0};
	static char bb[10]={0};
	static int cnt=0;
	static int rd=0;
	aa[cnt]=a;
	bb[cnt]=b;
	arr[cnt] = f2b(b2f(a)*b2f(b));
	cnt++;
	cnt=cnt%10;
	if(en){
		*c=arr[rd];
		*a_out=aa[rd];
		*b_out=bb[rd];
		rd++;
		rd=rd%10;
	}
}

int main()
{
	char ans,aa,bb;
		mult(36,129,&ans,&aa,&bb,1);
	printf("%d %x %x \n", ans,aa,bb);
	// for (int i = 0; i < 10; ++i)
	// {
	// 	mult(0,0,&ans,&aa,&bb,i%2);
	// 	printf("%d %d %d \n", ans,aa,bb);
	// }
	
	return 0;
}