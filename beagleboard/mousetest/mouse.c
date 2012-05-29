#include <stdio.h>
#include <stdlib.h>
int main() {
        FILE* fmouse0;
	FILE* fmouse1;
        char b0[3];
	char b1[3];
        fmouse0 = fopen("/dev/input/mouse0","r");
        fmouse1 = fopen("/dev/input/mouse1","r");
	int x0d=0,y0d=0; //x/y movement delta for mouse0
	int x1d=0,y1d=0; //x/y movement delta for mouse1
        int x0o=0,y0o=0; //x/y overflow (out of range -255 to +255) for mouse0
	int x1o=0,y1o=0; //x/y overflow (out of range -255 to +255) for mouse1
        
	int lb0=0,mb0=0,rb0=0,hs0=0,vs0=0; //left/middle/right mousebutton for mouse0
	int lb1=0,mb1=0,rb1=0,hs1=0,vs1=0; //left/middle/right mousebutton for mouse1        
	int run0=0,run1=0;
        while(!run0 && !run1){
                fread(b0,sizeof(char),3,fmouse0);
		fread(b1,sizeof(char),3,fmouse1);
                lb0=(b0[0]&1)>0;
		lb1=(b1[0]&1)>0;
                rb0=(b0[0]&2)>0;
		rb1=(b1[0]&2)>0;
                mb0=(b0[0]&4)>0;
		mb1=(b1[0]&4)>0;
                hs0=(b0[0]&16)>0;
		hs1=(b1[0]&16)>0;
                vs0=(b0[0]&32)>0;
		vs1=(b1[0]&32)>0;
                x0o=(b0[0]&64)>0;
		x1o=(b1[0]&64)>0;
                y0o=(b0[0]&128)>0;
		y1o=(b1[0]&128)>0;
                x0d=b0[1];
		x1d=b1[1];
                y0d=b0[2];
		y1d=b1[2];
                
		printf("hs0=%d,vs0=%d,lb0=%d rm0=%d mb0=%d xo0=%d yo0=%d xd0=%d yd0=%d\t",hs0,vs0,lb0,rb0,mb0,x0o,y0o,x0d,y0d);
		printf("hs1=%d,vs1=%d,lb1=%d rm1=%d mb1=%d xo1=%d yo1=%d xd1=%d yd1=%d\n",hs1,vs1,lb1,rb1,mb1,x1o,y1o,x1d,y1d);        
	}
        fclose(fmouse0);
	fclose(fmouse1);
	
	return 0;
}

