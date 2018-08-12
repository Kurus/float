#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h> 


///////////////////// two byte 16 bit
unsigned int d2b2(double a){
	printf("== %f\n", a);
    long long x = *(long long *)(&a);
    printf("%llx\n",x );
    x = x+0x0000020000000000; // round off
    long long e = ((x&0x7FF0000000000000)>>52) - 1008;
    long long man = x&0x000FFC0000000000;
    long long sgn = x&0x8000000000000000;
    if (e<0){
        e = 0;
        man = 0;
        sgn = 0;
    }
    if (e>31){
        e = 31;
        man = 0x000FFC0000000000;
    }
    printf("%llx\n",x );
    return (sgn>>48 | e<<10 | man>> 42)& 0xffff;
}

double b2d2(long long x){
    long long ee = ((x&0x7c00)>>10)+1008;
    if((x&0x7c00) == 0){
        if ((x&0x3ff)==0){
            ee=0;
        }
        else{
            ee=1008;
        }
    }
    long long a= (((x&0x8000)<<48) | (ee)<<52 | (x&0x3ff)<<42);
    printf(">> %f\n", *(double *)(&a));
    printf("%llx\n", a);
    return  *(double *)(&a);
}



//12 bit input
void add(unsigned char a,unsigned char a0, unsigned char b,unsigned char b0, unsigned char* c, unsigned char* c0, unsigned char* a_out, unsigned char* a_out0, unsigned char* b_out, unsigned char* b_out0, unsigned char en){
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

//12 bit input
void mul(unsigned char a,unsigned char a0, unsigned char b,unsigned char b0, unsigned char* c, unsigned char* c0, unsigned char* a_out, unsigned char* a_out0, unsigned char* b_out, unsigned char* b_out0, unsigned char en){
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
	arr[cnt] = d2b2(b2d2(a2)*b2d2(b2));
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
	unsigned int ans,ans0,a,a0,b,b0,t;
	// int x = 0x4a;
	// printf("%f\n",(b2d(x)));
	// printf("%f\n",(b2d2(x<<4)));
    // unsigned int a = d2b2(b2d2(0x609)*b2d2(0x663));
    // printf("%x\n", a);
    // printf("%f\n",b2d2(0x580));
    // printf("%llx\n",d2b2(-1.10592E5));

	for (int i = 0; i < 10; ++i)
	{
		a  = rand()&0xff;
		a0 = rand()&0xff;
		b  = rand()&0xff;
		b0 = rand()&0xff;
		// printf("%x %x %x\n", aa, aa>>4, aa<<4);
		add(a,a0,b,b0,&ans,&ans0,&t,&t,&t,&t,1);
		printf("%d %d %d \n", (ans2<<4)|(ans>>4),aa,bb);
	}
	
	return 0;
}