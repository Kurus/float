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
        unsigned long long exp : 11;
        unsigned long long sgn : 1;
    } r;
    double f;
};

// float b2f(unsigned char a){
// 	union float32 aa;
// 	printf("%d ->",a );
// 	aa.r.sgn=(a&0x80)>>7;
// 	aa.r.man=(a&0x3)<<21;
// 	if(((a&0x7c)>>2) == 0){
// 		if((a&0x3)==0){
// 			aa.r.exp=0;
// 		}
// 		else{
// 			aa.r.exp=112;
// 		}
// 	}
// 	else{
// 		aa.r.exp=(((a&0x7c)>>2)+112);
// 	}
// 	printf("%e\n", aa.f);
// 	return aa.f;
// }

// unsigned char f2b(float a){
// 	union float32 aa;
// 	aa.f=a;
// 	printf("%f\n", aa.f);
// 	unsigned char x = (aa.r.exp-112);
// 	if(x<0){x= 0; aa.r.sgn=0; aa.r.man=0;}
// 	if(x>31)x=31;
// 	return aa.r.sgn<<7 | x<<2 | aa.r.man>>21;
// }

unsigned char d2b(double a){
	// printf("= %f\n", a);
    long long x = *(long long *)(&a);
    long long e = ((x&0x7FF0000000000000)>>52) - 1008;
    long long man = x&0x000C000000000000;
    long long sgn = x&0x8000000000000000;
    if (e<0){
        e = 0;
        man = 0;
        sgn = 0;
    }
    if (e>31){
        e = 31;
    }

    return sgn>>56 | e<<2 | man>> 50;
}

double b2d(long long x){
    long long ee = ((x&0x7c)>>2)+1008;
    if((x&0x7c) == 0){
        if ((x&0x3)==0){
            ee=0;
        }
        else{
            ee=1008;
        }
    }
    long long a= (((x&0x80)<<56) | (ee)<<52 | (x&0x3)<<50);
    // printf("> %f ", *(double *)(&a));
    return  *(double *)(&a);
}

int d2b2(double a){
	// printf("== %f\n", a);
    long long x = *(long long *)(&a);
    long long e = ((x&0x7FF0000000000000)>>52) - 1008;
    long long man = x&0x000FC00000000000;
    long long sgn = x&0x8000000000000000;
    if (e<0){
        e = 0;
        man = 0;
        sgn = 0;
    }
    if (e>31){
        e = 31;
    }

    return sgn>>52 | e<<6 | man>> 46;
}

double b2d2(long long x){
    long long ee = ((x&0x7c0)>>6)+1008;
    if((x&0x7c0) == 0){
        if ((x&0x3f)==0){
            ee=0;
        }
        else{
            ee=1008;
        }
    }
    long long a= (((x&0x800)<<52) | (ee)<<52 | (x&0x3f)<<46);
    // printf(">> %f", *(double *)(&a));
    return  *(double *)(&a);
}

// int d22b(double a){
// 	union float64 aa;
// 	aa.f=a;
// 	printf("%f\n", aa.f);
// 	int x = (aa.r.exp-1008);
// 	printf("%lx\n", (long)x);
// 	if(x<0){x= 0; aa.r.sgn=0; aa.r.man=0;}
// 	if(x>31)x=31;
// 	return aa.r.sgn<<11 | x<<4 | aa.r.man>>46;
// }

// double b22d(int a){
// 	union float64 aa;
// 	aa.r.sgn=(a&0x800)>>11;
// 	aa.r.man=(a&0x3f)<<46;
// 	if(((a&0x7c0)) == 0){
// 		if((a&0x3f)==0){
// 			aa.r.exp=0;
// 		}
// 		else{
// 			aa.r.exp=1008;
// 		}
// 	}
// 	else{
// 		aa.r.exp=(((a&0x7c0)>>6)+1008);
// 	}
// 	printf("%e\n", aa.f);
// 	return aa.f;
// }



//8bit input
void add(unsigned char a, unsigned char b, unsigned char* c,unsigned char* a_out, unsigned char* b_out, unsigned char en){
	static unsigned char arr[10]={0};
	static unsigned char aa[10]={0};
	static unsigned char bb[10]={0};
	static int cnt=0;
	static int rd=0;
	aa[cnt]=a;
	bb[cnt]=b;
	arr[cnt] = d2b( b2d(a) + b2d(b) );
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
//12 bit input
void add2(unsigned char a,unsigned char a0, unsigned char b,unsigned char b0, unsigned char* c, unsigned char* c0, unsigned char* a_out, unsigned char* a_out0, unsigned char* b_out, unsigned char* b_out0, unsigned char en){
	// a=a&0xff;a0= a0&0xff;
	// b=b&0xff;b0= b0&0xff;
	// printf("add2 %x %x %x %x \n",a, a0,b, b0 );
	static int arr[10]={0};
	static int aa[10]={0};
	static int bb[10]={0};
	static int cnt=0;
	static int rd=0;
	int a2= (a<<8)|a0;
	int b2= ((int)(b<<8)|b0);
	aa[cnt]= a2;
	bb[cnt]= b2;
	// printf("a2,b2 %x %x\n", a2,b2);
	arr[cnt] = d2b2(b2d2(a2)+b2d2(b2));
	cnt++;
	cnt=cnt%10;
	if(en){
		*c=(arr[rd]&0xff00)>>8;
		*c0=arr[rd]&0xff;
		*a_out=(aa[rd]&0xff00)>>8;
		*a_out0=(aa[rd]&0xff);
		*b_out=(bb[rd]&0xff00)>>8;
		*b_out0=(bb[rd]&0xff);
		rd++;
		rd=rd%10;
	}
}

int main()
{
	unsigned char ans,ans2,aa,bb,t;
	// int x = 0x4a;
	// printf("%f\n",(b2d(x)));
	// printf("%f\n",(b2d2(x<<4)));

	for (int i = 0; i < 10; ++i)
	{
		aa = rand()%256;
		bb = rand()%256;
		// printf("%x %x %x\n", aa, aa>>4, aa<<4);
		add(aa,bb,&ans,&t,&t,1);
		// printf("%d %d %d \n", ans,aa,bb);
		add2(aa>>4,aa<<4,bb>>4,(bb<<4)&0xff,&ans2,&ans,&t,&t,&t,&t,1);
		// printf("%d %d %d \n", (ans2<<4)|(ans>>4),aa,bb);
	}
	
	return 0;
}