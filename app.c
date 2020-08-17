#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "gahodrv_ioctl_basic.h"

int main()
{
    int fd;
    uint8_t buf[16];
    uint16_t i;
    int num = 0;
    for (i=0; i<sizeof(buf); i++)
        buf[i] = i;
    

    fd = open("/dev/gahodrv", O_RDWR);
    if (fd < 0)
    {
        printf ("cannot open file\n");
        return -1;
    }
    // write & read test
    printf ("write the buffer\n");
    write(fd, buf, sizeof(buf));
    
    printf ("read the buffer\n");
    // clear the buffer
    memset(buf, 0xff, sizeof(buf));
    read(fd, buf, sizeof(buf));
    for (i=0; i<sizeof(buf)/sizeof(uint8_t); i++)
        printf ("%02X ", (unsigned)buf[i]);
    printf ("\n");

    // ioctl test
    num = 99;
    ioctl(fd, GAHODRV_IOC_SET_PARAM1, &num);
    printf ("ioctl test: set ... %d\n", num);

    num = 11;
    ioctl(fd, GAHODRV_IOC_GET_PARAM1, &num);
    printf ("ioctl test: get ... %d\n", num);

    // close
    close(fd);

    return 0;
}


