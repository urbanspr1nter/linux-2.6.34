#include <linux/avgdev.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>

#define SAFE_MATH 0

#if SAFE_MATH == 1
DEFINE_SPINLOCK(math_stuff_lock);
#endif 

static volatile int x;

static int math_stuff(void* name) {
	int i; 

#if SAFE_MATH == 1
	spin_lock(&math_stuff_lock);
#endif

	for (i = 0; i < 100000; i++) {
		x++;
	}

	for (i = 0; i < 100000; i++) {
		x--;
	}

#if SAFE_MATH == 1
	spin_unlock(&math_stuff_lock);
#endif

	printk(KERN_INFO "AVGDEV - %s - x should be 0: %d", (char*)name, x);
}

static void avgdev_work_handler(void* d) {
	printk(KERN_INFO "AVGDEV - About to do some work...");

	x = 0;

	struct task_struct* a = kthread_create(math_stuff, "thread 1", "avgdev_1");
	struct task_struct* b = kthread_create(math_stuff, "thread 2", "avgdev_2");

	wake_up_process(a);
	wake_up_process(b);
}

DECLARE_WORK(avgdev_work, avgdev_work_handler);

