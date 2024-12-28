#include <linux/avgdev.h>

static void avgdev_tasklet_handler(unsigned long data) {
	int i;
	// do stuff here
	printk(KERN_INFO "AVGDEV - I am here! Data: %ld\n", data);
	
	for (i = 0; i < 20; i++) {
		printk(KERN_INFO "AVGDEV - The value of i^3 is: %d\n", i * i * i);
	}
	
}

DECLARE_TASKLET(avgdev_tasklet, avgdev_tasklet_handler, 0);
