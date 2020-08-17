#ifndef __GAHODRV_IOCTL_BASIC_H__
#define __GAHODRV_IOCTL_BASIC_H__

#define GAHODRV_IOC_MAGIC         'f'

#define GAHODRV_IOC_SET_PARAM1    (_IOWR(GAHODRV_IOC_MAGIC, 0, int))
#define GAHODRV_IOC_GET_PARAM1    (_IOR(GAHODRV_IOC_MAGIC, 1, int))
#define GAHODRV_IOC_MAXNR         2


#endif // __GAHODRV_IOCTL_BASIC_H__
