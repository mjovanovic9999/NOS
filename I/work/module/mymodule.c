#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init mymodule_init(void)
{
    pr_info("mymodule init\n");
    return 0;
}
static void __exit mymodule_exit(void)
{
    pr_info("mymodule exit\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("My Module");
MODULE_AUTHOR("Milos Jovanovic");

//module param 112 slajd
