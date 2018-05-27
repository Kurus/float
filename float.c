#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h> // to generate random numbers
#define SIZE 100000
#define EX 5    //exponent of our custom float
#define MN 2    //mantissa of our custom float
#define MN_DF (23-MN) //used to convert float32 to ours)
#define B_DF (128-(1<<(EX-1)))
#define BI ((1<<(EX-1))-1) //bias


// used to seperate mantissa, exponent,sign bit from float32
union float32 {
    struct {
        unsigned int man : 23;
        unsigned int exp : 8;
        unsigned int sgn : 1;
    } r;
    float f;
};

// our float definisniooo
union float8{
    struct  {
    unsigned int man : 2;//least significant bit
    unsigned int exp : 5;
    unsigned int sgn : 1;
    }r;
    char f;   
};


//this function convert our float back to float32 to print it easily
float print(short sgn, short exp, short man){
    union float32 ans;
    ans.r.exp = exp + B_DF;
    ans.r.man = man<<MN_DF;
    ans.r.sgn = sgn;
    //convrted anser to flaot 32
    // printf("ans = %f \n",ans.f);
    return ans.f;

}
float add(char sgn_a, char exp_a, char man_a,
                char sgn_b, char exp_b, char man_b)
{

    if(exp_a<exp_b || (exp_a==exp_b && man_a<man_b)){
        exp_a = (exp_a + exp_b) - (exp_b = exp_a);
        sgn_a = (sgn_a + sgn_b) - (sgn_b = sgn_a);
        man_a = (man_a + man_b) - (man_b = man_a);
    }
    char d = exp_a - exp_b;
    char sgn,exp,man;
    int fr_a,fr_b,fr;
    // prepending 1bit to mantissa to create fractional part of the number
    fr_a = (1<<MN) | man_a;
    // depending on sigh converting mantissa to negative number
    //similarly for other number
    fr_b = (1<<MN) | man_b;

    // shifting fractional part of small number to aligh with big number and adding both fractional parts
    fr_b=fr_b<<3;///////////////////3bit
    fr_a=fr_a<<3;///////////////////3bit
    // printf("%x %x\n", fr_a, fr_b);
    if(sgn_a){
        if(sgn_b){
            fr = - fr_a - (fr_b >> d);
        }else{
            fr = - fr_a + (fr_b >> d);
        }
    }else{
        if(sgn_b){
            fr = fr_a - (fr_b >> d);
        }else{
            fr = fr_a + (fr_b >> d);
        }
    }
    fr=fr>0?fr:-fr;
    // printf("%x\n", fr);
    if(fr&0x40) fr = fr+8;///////////////////3bit
    if(fr&0x20) fr = fr+4;///////////////////3bit
    if(fr&0x10) fr = fr+2;///////////////////3bit
    if(fr&0x8) fr = fr+1;///////////////////3bit
    // printf("%x\n", fr);
    // printf("%d %d %d\n", fr_a, fr_b, fr);
    exp = exp_a;
    exp-=3; ///////////////////3bit
    // sign of the fractional part decide sign of final number
    sgn=sgn_a;
    // if fractional part is negative it will be in twos complement so cnvert it to normal
    // printf("%x %x %x \n",fr_a,fr_b,fr);
    // printf("%x %x %x\n", exp_a, exp_b,exp);
    // if over flow occur do a right shift and adjust exponent accordigly
    if(fr==0){
        exp = 0;
        sgn = 0;
        man = 0;
        return print(sgn,exp,man);
    }else if((fr&(16<<MN))){
        fr = fr>>4;
        exp+=4;
    }else if((fr&(8<<MN))){
        fr = fr>>3;
        exp+=3;
    }else if((fr&(4<<MN))){
        fr = fr>>2;
        exp+=2;
    }else if((fr&(2<<MN))){
        fr = fr>>1;
        exp+=1;
    }else{
        // nomalizing: counting leading zeros and shifting it , adjust exponent
    char zero = 0;
        while(!(fr&(1<<MN))){
            zero ++;
            fr = fr<<1;
        }
        exp = exp - zero;
    }
    man = fr & ((1<<MN)-1);

    // printf("sgn, exp, man -> 0x%x, 0x%x , 0x%x \n",sgn, exp, man);
    return print(sgn,exp,man); // print the final answer
}

float mul(short sgn_a,short exp_a,short man_a,short sgn_b,short exp_b,short man_b)
{
    short d = exp_a - exp_b;
    short sgn,exp,man;
    int fr_a,fr_b,fr;
    // prepending 1bit to mantissa to create fractional part of the number
    fr_a = (1<<MN) | man_a;
    //similarly for other number
    fr_b = (1<<MN) | man_b;
    fr = (fr_a * fr_b)>>MN; //since we treat fractional part as integer, we should shift by mantissa, for exampl
            // 1.001 * 1.100 - we consider as 1001*1100 then need to shift by 3(mantissa)
    exp = exp_a + exp_b - BI;
    exp = exp>0?exp:0;
    if(exp==0){
        // printf("%x %x\n",exp_a, exp_b );
    }
    exp = exp<(1<<EX)?exp:((1<<EX)-1);
    // sign of the fractional part decide sign of final number
    sgn=sgn_a^sgn_b;
    // if over flow occur do a right shift and adjust exponent accordigly
    if((fr&(2<<MN))){
        fr = fr>>1;
        exp++;
    }else{
        // nomalizing: counting leading zeros and shifting it , adjust exponent
        short zero = 0;
        while(!(fr&(1<<MN))){
            zero ++;
            fr = fr<<1;
            if(fr==0){
                exp = 0;
                zero = 0;
                break;
            }
        }
        exp = exp - zero;
    }
    man = fr & ((1<<MN)-1);

    // printting final answer
    // printf("sgn, exp, man -> 0x%x, 0x%x , 0x%x \n",sgn, exp, man);
    return print(sgn,exp,man); // print the final answer
}




float ff(float in){// not competed
    uint32_t f = (uint32_t)in;
    uint32_t m = f&0x007fffffu;
    uint32_t e = f&0x7f800000u;
    uint32_t s = f&0x80000000u;

    printf("%x %x %x\n",s,e,m);
    return f;
}

#define EX_U 143
#define EX_L 112
#define SH 21
//float to 8bit float
float f2bf(float in){ //correct one
    union float32 a;
    a.f=in;
    if((a.r.man & 0x700000) == 0x700000){ a.r.exp++;}
    a.r.man = a.r.man + 0x100000;
    a.r.man=(a.r.man>>SH)<<SH;
    if(a.r.exp == 0) return a.f;
    if(a.r.exp>EX_U)a.r.exp=EX_U;
    if(a.r.exp<EX_L)a.r.exp=EX_L;
    return a.f;
}


float f2bf_trunc(float in){ // not correct
    union float32 a;
    a.f=in;
    a.r.man=(a.r.man>>SH)<<SH;
    if(a.r.exp == 0) return a.f;
    if(a.r.exp>EX_U)a.r.exp=EX_U;
    if(a.r.exp<EX_L)a.r.exp=EX_L;
    return a.f;
}

//byte to float sig|exp|man
float b2f(char in){ // byte to float
    union float8 a;
    union float32 b;
    a.f = in;
    b.r.man=a.r.man<<21;
    b.r.exp=a.r.exp == 0? 0: a.r.exp+112;
    b.r.sgn = a.r.sgn;
    // printf("%x %x %x\n",a.r.sgn, a.r.exp, a.r.man);
    // printf("%x %x %x\n",b.r.sgn, b.r.exp, b.r.man);
    return b.f;
}

void test(){
    printf("%f\n",f2bf(1792.000000) );
}

int main(){
    union float32 a,b;  //first we create float32 number and then convert to our custom format
    int i;



int cntt =0;
    // loop for random testing
    for(int i=0;i<100;i++){
        srand(time(NULL)+rand()*i);


        char aa =rand();
        char bb=rand();
        a.f = b2f(aa); 
        b.f = b2f(bb);
        char sign_a  = (aa&0x80)>>7; 
        char expo_a  = (aa&0x7c)>>2; 
        char manti_a  = (aa&0x3); 
        char sign_b  = (bb&0x80)>>7; 
        char expo_b  = (bb&0x7c)>>2; 
        char manti_b  = (bb&0x3);

        float ans = add(sign_a ,expo_a ,manti_a,sign_b ,expo_b ,manti_b);
        float ans2 = f2bf(f2bf(a.f)+f2bf(b.f));
        float ori = (a.f + b.f) ;
        float err = ori - ans;
        float err2 = ori-ans2;
        float err3 = ans-ans2;
        if(ans!=ans2){
            // printf("%d %d\n",aa,bb );
            // printf("%f %f \n",print(sign_a ,expo_a ,manti_a),a.f);
            // printf("%d %d %d\n", sign_a ,expo_a ,manti_a);
            // printf("%d %d %d\n", sign_b ,expo_b ,manti_b);
            printf("%f , %f \n",a.f,b.f);        
            printf("ori %.9f \t add1 %.9f \tadd2 %.10f \t er %.4f  per %.2f %%\n",ori, ans ,ans2, err3, err3*100/ori );
            // return 0;
            cntt++;
        }

        // for mulitplication
        // ans = mul(a.r.sgn,a.r.exp - B_DF , a.r.man>>MN_DF,b.r.sgn,b.r.exp - B_DF, b.r.man>>MN_DF);
        // ori = a.f*b.f ;
        // err = ori - ans;
        // ans2 = f(f(a.f)*f(b.f));
        // err2 = ans-ans2;
        // // printf("mul %f\t err %.6f per %.4f %%\n", ans, err, err*100/ori);

        // if(err2!=0){
        //     printf("err %f ", err2);
        //     printf(" | %f , %f  = %f  = %f == %f \n",a.f,b.f, ans, ans2,ori);            
        // }


        // printf("\n");
    }


    printf("%d\n",cntt );
    return 0;
}











// float add(short sgn_a,short exp_a,short man_a,short sgn_b,short exp_b,short man_b){
//     if(exp_a<exp_b || (exp_a==exp_b && man_a<man_b)){
//         exp_a = (exp_a + exp_b) - (exp_b = exp_a);
//         sgn_a = (sgn_a + sgn_b) - (sgn_b = sgn_a);
//         man_a = (man_a + man_b) - (man_b = man_a);
//     }
//     short d = exp_a - exp_b;
//     short sgn,exp,man;
//     int fr_a,fr_b,fr;
//     // prepending 1bit to mantissa to create fractional part of the number
//     fr_a = (1<<MN) | man_a;
//     // depending on sigh converting mantissa to negative number
//     //similarly for other number
//     fr_b = (1<<MN) | man_b;

//     // shifting fractional part of small number to aligh with big number and adding both fractional parts
    
//     if(sgn_a){
//         if(sgn_b){
//             fr = - fr_a - (fr_b >> d);
//         }else{
//             fr = - fr_a + (fr_b >> d);
//         }
//     }else{
//         if(sgn_b){
//             fr = fr_a - (fr_b >> d);
//         }else{
//             fr = fr_a + (fr_b >> d);
//         }
//     }

//     exp = exp_a;
//     // sign of the fractional part decide sign of final number
//     sgn=sgn_a;
//     // if fractional part is negative it will be in twos complement so cnvert it to normal
//     fr=fr>0?fr:-fr;
//     // printf("%x %x %x \n",fr_a,fr_b,fr);
//     // printf("%x %x %x\n", exp_a, exp_b,exp);
//     // if over flow occur do a right shift and adjust exponent accordigly
//     if(fr==0){
//         exp = 0;
//         sgn = 0;
//     }else if((fr&(2<<MN))){
//         fr = fr>>1;
//         exp++;
//     }else{
//         // nomalizing: counting leading zeros and shifting it , adjust exponent
//     short zero = 0;
//         while(!(fr&(1<<MN))){
//             zero ++;
//             fr = fr<<1;
//         }
//         exp = exp - zero;
//     }
//     man = fr & ((1<<MN)-1);

//     // printting final answer
//     // printf("%x \n", fr);
//     // printf("sgn, exp, man -> 0x%x, 0x%x , 0x%x \n",sgn, exp, man);
//     return print(sgn,exp,man); // print the final answer
// }