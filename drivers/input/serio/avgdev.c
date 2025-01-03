#include <linux/avgdev.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>

#define SAFE_MATH 0 

#if SAFE_MATH == 1
DEFINE_SPINLOCK(my_lock);
#endif

static volatile int x;

static int math_stuff(void* name) {
	int i;
	
#if SAFE_MATH == 1
	spin_lock(&my_lock);
#endif
	for (i = 0; i < 100000; i++) {
		x++;
	}

	for (i = 0; i < 100000; i++) {
		x--;
	}

#if SAFE_MATH == 1
	spin_unlock(&my_lock);
#endif

	printk(KERN_INFO "AVGDEV - %s, x should be 0: %d",(char*)name, x);

	return x;
}

static void avgdev_work_handler(void* d) {
	printk(KERN_INFO "AVGDEV - it works.");
	
	x = 0;

	struct task_struct* a = kthread_create(math_stuff, "thread 1", "avgdev");	
	struct task_struct* b = kthread_create(math_stuff, "thread 2", "avgdev2");
	
	wake_up_process(a);
	wake_up_process(b);
}

DECLARE_WORK(avgdev_work, avgdev_work_handler);
