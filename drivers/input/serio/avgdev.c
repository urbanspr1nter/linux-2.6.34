#include <linux/avgdev.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>

#define SAFE_MATH 1 

#if SAFE_MATH == 1
struct semaphore math_sema;
#endif 

static volatile int x;

static int math_stuff(void* name) {
	int i; 

#if SAFE_MATH == 1
	if(down_interruptible(&math_sema)) {
		printk(KERN_INFO "AVGDEV - %s - Couldn't acquire semaphore", (char*)name);
	}
#endif

	for (i = 0; i < 100000; i++) {
		x++;
	}

	for (i = 0; i < 100000; i++) {
		x--;
	}

#if SAFE_MATH == 1
	up(&math_sema);
#endif

	printk(KERN_INFO "AVGDEV - %s - x should be 0: %d", (char*)name, x);
}

static void avgdev_work_handler(void* d) {
	printk(KERN_INFO "AVGDEV - About to do some work...");

#if SAFE_MATH == 1
	sema_init(&math_sema, 1);
#endif

	x = 0;

	struct task_struct* a = kthread_create(math_stuff, "thread 1", "avgdev_1");
	struct task_struct* b = kthread_create(math_stuff, "thread 2", "avgdev_2");

	wake_up_process(a);
	wake_up_process(b);
}

DECLARE_WORK(avgdev_work, avgdev_work_handler);

