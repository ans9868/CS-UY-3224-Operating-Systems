#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>


unsigned long startTime;

MODULE_LICENSE("Dual BSD/GPL");
static int recordJiff(void)
{
    startTime = jiffies;
    return 0;
}

static void diffJiff(void) //module exit, my kernel module woult not close unless i had an explicit
{
    unsigned long endTime = jiffies;
    unsigned long elapsedTime = endTime - startTime;
    printk(KERN_ALERT "%lu\n", elapsedTime);
}

module_init(recordJiff);
module_exit(diffJiff);
