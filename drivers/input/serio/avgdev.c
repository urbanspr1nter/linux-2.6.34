
#include <linux/avgdev.h>
#include <linux/timer.h>
#include <asm/param.h>
#include <asm/current.h>
#include <linux/spinlock.h>

static struct timer_list avgdev_timer;
static int timer_initialized = 0;

DEFINE_SPINLOCK(avgdev_lock);

static void avgdev_acknowledge(unsigned long data) {
	printk(KERN_INFO "AVGDEV - Running the timer! JIFFIES:%d", data);
}

static void avgdev_schedule() {
	if (timer_pending(&avgdev_timer)) {
		printk(KERN_INFO "AVGDEV - Timer already pending.");
		return;
	}
	
	if (!timer_initialized) {
		init_timer(&avgdev_timer);
		timer_initialized = 1;
	}

	unsigned long current_jiffies = jiffies;
	avgdev_timer.expires = current_jiffies + (HZ * 3);
	avgdev_timer.data = current_jiffies;
	avgdev_timer.function = avgdev_acknowledge;	

	printk(KERN_INFO "AVGDEV - Setting up the timer at JIFFIES:%d ...", current_jiffies);

	add_timer(&avgdev_timer);
}

static void avgdev_work_handler(void* data) {
	printk(KERN_INFO "AVGDEV - Scheduling work..");

	spin_lock(&avgdev_lock);
	avgdev_schedule();
	spin_unlock(&avgdev_lock);
}

DECLARE_WORK(avgdev_work, avgdev_work_handler);
