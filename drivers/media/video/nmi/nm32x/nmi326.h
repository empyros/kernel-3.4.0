/*****************************************************************************
 Copyright(c) 2012 NMI Inc. All Rights Reserved

 File name : nmi326.h

 Description : NM326 host interface

 History :
 ----------------------------------------------------------------------
 2012/11/27 	ssw		initial
*******************************************************************************/

#ifndef __NMI_HW_H__
#define __NMI_HW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include "cust_gpio_usage.h"
#include "cust_eint.h"

//punk
#define SIM_PAD

#define NM_DEBUG_ON

#ifdef NM_DEBUG_ON
//#define NM_KMSG(x...) printk(KERN_DEBUG ">ISDBT< " x)
#define NM_KMSG(x...) printk(x)
#else
#define NM_KMSG(x...) /* null */
#endif

#define ISDBT_DEV_NAME		"isdbt"
#if defined(NMI_DTV_SUPPORT) //dtv
#define ISDBT_DEV_MAJOR	191	//191 //227 is used by camera-pipemgr dev
#define ISDBT_DEV_MINOR	0
#else
	xx
#define ISDBT_DEV_MAJOR	227
#define ISDBT_DEV_MINOR	0
#endif

#define DTV_IRQ_DEINIT			0
#define DTV_IRQ_INIT			1
#define DTV_IRQ_SET				2


#ifdef SIM_PAD
#define NMI_POWER_PIN		GPIO_MODE_MAX
#define NMI_RESET_PIN		GPIO131
#define NMI_IRQN_PIN		GPIO34	//GPIO_DVT_EINT_PIN
#else
	xx
#define NMI_POWER_PIN		GPIO137
#define NMI_RESET_PIN		GPIO51
#define NMI_IRQN_PIN		GPIO_DVT_EINT_PIN
#endif

/***************************************/

#ifndef SIM_PAD
#define CUST_EINT_DTV_NUM					CUST_EINT_CUST_DTV_EINT_NUM		// mediatek\custom\lge75_v4_jb\kernel\dct\dct\custo_eint.h
#define CUST_EINT_DTV_DEBOUTCE_CN		CUST_EINT_CUST_DTV_EINT_DEBOUNCE_CN
#define CUST_EINT_DTV_POLARITY			CUST_EINT_POLARITY_LOW
#define CUST_EINT_DTV_SENSITIVE			CUST_EINT_LEVEL_SENSITIVE
#define CUST_EINT_DTV_DEBOUNCE_EN		CUST_EINT_DEBOUNCE_DISABLE
#endif
/****************************************/

typedef struct {
	long		index;
	void		**hInit;
	void		*hI2C;
	spinlock_t	isr_lock;
	struct fasync_struct *async_queue;
	unsigned char* rwBuf;
} ISDBT_OPEN_INFO_T;

#define MAX_OPEN_NUM		8

#define IOCTL_MAGIC	't'
//#define IOCTL_MAGIC	'T'

#define IOCTL_MAXNR				9

#define IOCTL_ISDBT_POWER_ON		_IO( IOCTL_MAGIC, 0 )
#define IOCTL_ISDBT_POWER_OFF		_IO( IOCTL_MAGIC, 1 )
#define IOCTL_ISDBT_RST_DN			_IO( IOCTL_MAGIC, 2 )
#define IOCTL_ISDBT_RST_UP			_IO( IOCTL_MAGIC, 3 )

#define IOCTL_ISDBT_INTERRUPT_REGISTER		_IO(IOCTL_MAGIC, 4)
#define IOCTL_ISDBT_INTERRUPT_UNREGISTER	_IO(IOCTL_MAGIC, 5)
#define IOCTL_ISDBT_INTERRUPT_ENABLE		_IO(IOCTL_MAGIC, 6)
#define IOCTL_ISDBT_INTERRUPT_DISABLE		_IO(IOCTL_MAGIC, 7)
#define IOCTL_ISDBT_INTERRUPT_DONE		_IO(IOCTL_MAGIC, 8)

#ifdef __cplusplus
}
#endif

#endif // __NMI_HW_H__

