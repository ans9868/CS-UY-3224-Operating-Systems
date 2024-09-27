#include <linux/init.h>
#include <linux/module.h>
#include <linux/timekeeping.h>


ktime_t startTime;

MODULE_LICENSE("Dual BSD/GPL");
static int recordTime(void)
{
    startTime = ktime_get_boottime();
    return 0;
}

static void diffTime(void) //module exit, my kernel module woult not close unless i had an explicit
{
    
    ktime_t endTime = ktime_get_boottime();
    ktime_t elapsedTime = ktime_to_ns(ktime_sub(endTime, startTime));
    printk(KERN_ALERT "%lld\n", elapsedTime); 
}

module_init(recordTime);
module_exit(diffTime);
