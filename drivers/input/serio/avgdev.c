#include <linux/avgdev.h>

static void avgdev_work_handler(void* data) {
	int i;

	i = 0;
	while (i < 100000) {
		// do nothing
		i++;
	}

	printk(KERN_INFO "AVGDEV - After busy waiting...");
}

DECLARE_WORK(avgdev_work, avgdev_work_handler); 

