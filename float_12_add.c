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

union float64 {
    struct {
        unsigned long long man : 52;
        unsigned int exp : 11;
        unsigned int sgn : 1;
    } r;
    double f;
};

float b2f(char a){
	union float32 aa;
	printf("%d ->",a );
	aa.r.sgn=(a&0x80)>>7;
	aa.r.man=(a&0x3)<<21;
	if(((a&0x7c)>>2) == 0){
		if((a&0x3)==0){
			aa.r.exp=0;
		}
		else{
			aa.r.exp=112;
		}
	}
	else{
		aa.r.exp=(((a&0x7c)>>2)+112);
	}
	printf("%e\n", aa.f);
	return aa.f;
}

char f2b(float a){
	union float32 aa;
	aa.f=a;
	printf("%f\n", aa.f);
	char x = (aa.r.exp-112);
	if(x<0){x= 0; aa.r.sgn=0; aa.r.man=0;}
	if(x>31)x=31;
	return aa.r.sgn<<7 | x<<2 | aa.r.man>>21;
}

char d2b(double a){
	union float64 aa;
	aa.f=a;
	printf("%f\n", aa.f);
	int x = (aa.r.exp-1008);
	printf("%lx\n", x);
	if(x<0){x= 0; aa.r.sgn=0; aa.r.man=0;}
	if(x>31)x=31;
	return aa.r.sgn<<7 | x<<2 | aa.r.man>>50;
}
double f2d(float a){return (double)a;}
	
void add(char a, char b, char* c,char* a_out, char* b_out, char en){
	static char arr[10]={0};
	static char aa[10]={0};
	static char bb[10]={0};
	static int cnt=0;
	static int rd=0;
	aa[cnt]=a;
	bb[cnt]=b;
	arr[cnt] = d2b(f2d(b2f(a))+f2d(b2f(b)));
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
	// printf("%d\n",f2b(b2f(13)) );
		add(0,0,&ans,&aa,&bb,1);
	printf("%d\n", ans);
	// for (int i = 0; i < 10; ++i)
	// {
	// 	add(0,0,&ans,&aa,&bb,i%2);
	// 	printf("%d %d %d \n", ans,aa,bb);
	// }
	
	return 0;
}