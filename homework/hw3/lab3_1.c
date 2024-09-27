#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>


MODULE_LICENSE("Dual BSD/GPL");
static int tikTime(void)
{
    long hz = HZ;
    //printk("Hz: %lu", hz);
    long tickTime =  1000 / hz;
    //printk(KERN_ALERT "Tick: %lu", tickTime);
    printk(KERN_ALERT "Hello, world\n%lu", tickTime);
    return 0;
}

static void m_exit(void) //module exit, my kernel module woult not close unless i had an explicit
{

}

module_init(tikTime);
module_exit(m_exit);
