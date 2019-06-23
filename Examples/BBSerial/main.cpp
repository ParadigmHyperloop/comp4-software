/*
 * serial_communication
 * ------------------
 * $Revision: 1.1 $
 * $Date: 2015/01/04 13:14:18 $
 * Author: K Raghav
 */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
void reading(int fd)
{
    char red[999];
    int i,ret,flag;
    ret=i=flag=0;
    printf("\nfunction name is  %s >>\n",__func__);
    while(1)
    {
        ret=read(fd,red,99);
        printf("\nReading data.... >>>>>%s<< \n",red);
        sleep(1);
    }
}
int open_port(void)
{
    int fd,ret;
    char red[88];
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    // ret=read(fd,red,16);
    printf("\nfunction name is  %s >>\n",__func__);
    printf("reading from Serial port== %c >>\n",red);
    printf("returned fd is :%d\n",fd );
    if (fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyUSB0 - ");
    }
    return (fd);
}
int main()
{
    int k = open_port();
    reading(k);
    return 0;
}