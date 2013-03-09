/* simple CAN application example 
 * 
 * dump the data structure of canmsg_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <can4linux.h>



int main(int argc,char **argv)
{
unsigned long int base;
canmsg_t msg;

    printf(" sizeof(canmsg_t) is %d bytes\n", (int)sizeof(canmsg_t));

    base = (unsigned long int)&msg;

    printf("\t\t\t\t\tstart\tsize\n");
    printf("\tint flags\t\t\t%ld\t%d\n",
    	(unsigned long int)&msg.flags - base, (int)sizeof(msg.flags));

    printf("\tint cob\t\t\t\t%ld\t%d\n",
    	(unsigned long int)&msg.cob - base, (int)sizeof(msg.cob));

    printf("\tunsigned long id\t\t%ld\t%d\n",
    	(unsigned long int)&msg.id - base, (int)sizeof(msg.id));

    printf("\tstruct timeval timestamp\t%ld\t%d\n",
    	(unsigned long int)&msg.timestamp - base, (int)sizeof(msg.timestamp));


    printf("\tshort int length\t\t%ld\t%d\n",
    	(unsigned long int)&msg.length - base, (int)sizeof(msg.length));


    printf("\tunsigned char data[8]\t\t%ld\t%d\n",
    	(unsigned long int)&msg.data[0] - base, 8*(int)sizeof(msg.data[0]));

    return 0;
}



/* all data types can be easily represented with the 
   int16_t, uint32_t etc
   type definitions.
   The exception is "struct timeval"
   which is 8 byte == 2 * 4 byte on 32 bit x86
   but     16 byte == 2 * 8 byte on 64 bit x86 



  linux/time.h
  struct timeval {
        time_t          tv_sec;         // seconds
        suseconds_t     tv_usec;        // microseconds
   };


  both, time_t and suseconds_t are long values, which are 4 bytes on 32bit
  and 8 byte on 564 bit




*/
