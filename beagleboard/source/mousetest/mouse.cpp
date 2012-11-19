#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {

    int uid = getuid();

    FILE* fmouse;
    char b[3];

    string argvstr[argc];
    for (int i = 0; i < argc; i++) {
        argvstr[i] = string(argv[i]);
    }

    if(uid != 0)
    {
        printf("This Programm requires to be root. Please log in as root or use sudo\n\n");
        return -1;
    }

	if (argc < 2) {printf("invalid count of arguments ...\ntype mouse --help for more information...\n\n"); return -1;}
	else if (argvstr[1] == "--help") {
		printf("usage: mouse <option> inputdevice (eg. mouse --read-specific-mouse /dev/input/mouse0)\n\n");
		printf("options:\n");
		printf("\t--read-all-mice\treads the events of all adapted mice\n");
		printf("\t--read-specific-mouse\treads the events of an specific mouse like /dev/input/mouse0\n");
		return 0;
	}
	else if (argvstr[1] == "--read-all-mice") { fmouse = fopen("/dev/input/mice","r"); }
	else if (argvstr[1] == "--read-specific-mouse") {
		if (argc != 3) { printf("invalid use of mouse --read-specific-mouse\ntype mouse --help for more information...\n\n"); return -1; }
		else { fmouse = fopen(argv[2], "r"); }
	}
	else { printf ("invalid option ...\n"); return -1; }

	int xd=0,yd=00; //x/y movement delt
        int xo=0,yo=0; //x/y overflow (out of range -255 to +255)
	int lb=0,mb=0,rb=0,hs=0,vs=0; //left/middle/right mousebutton
	int run=0;

	long long absx=0;
	long long absy=0;

        while(!run) {
		fread(b,sizeof(char),3,fmouse);
                lb=(b[0]&1)>0;
                rb=(b[0]&2)>0;
                mb=(b[0]&4)>0;
                hs=(b[0]&16)>0;
                vs=(b[0]&32)>0;
                xo=(b[0]&64)>0;
                yo=(b[0]&128)>0;
                xd=b[1];
                yd=b[2];

		absx += xd;
		absy += yd;

		float sxd = (absx/392.6);
		float syd = (absy/392.6);

        printf("%f cm\t%f cm\n", sxd, syd);
		//printf("%lld\t%lld\n", absx, absy);


		//printf("hs=%d,vs=%d,lb=%d rm=%d mb=%d xo=%d yo=%d xd=%d yd=%d\n",hs,vs,lb,rb,mb,xo,yo,xd,yd);
	}

        fclose(fmouse);

	return 0;
}

