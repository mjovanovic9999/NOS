#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int pid = -1;

module_param(pid, int, 0);

static void show_process_info(struct task_struct *task){
    pr_info("pid: %d, executable name: %s, tgid: %d, prio: %d, static_prio: %d, normal_prio: %d, rt_priority: %u, nice: %d, uid: %d, Total pages mapped: %lu, Accumulated virtual memory usage: %llu, utime: %llu, stime: %llu, gtime: %llu\n",
            task->pid,task->comm, task->tgid, task->prio,task->static_prio, task->normal_prio, task->rt_priority, task_nice(task), task->cred->uid.val, task->active_mm->total_vm, task->acct_vm_mem1, task->utime, task->stime, task->gtime);
}


static void dfs_child_process(struct task_struct *task)
{
    struct task_struct *child;

    list_for_each_entry(child, &task->children, sibling) {
        dfs_child_process(child);
    }
    show_process_info(task);

}

static int __init mymodule_init(void)
{
    struct task_struct *task;

    pr_info("mymodule init\n");

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "Process with PID %d not found\n", pid);
        return -1;
    }

    dfs_child_process(task);

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
