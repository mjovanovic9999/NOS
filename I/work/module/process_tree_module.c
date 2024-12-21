#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Process tree module");
MODULE_AUTHOR("Milos Jovanovic");


static int pid = 1;

module_param(pid, int, 0);

static int print_task_data(struct task_struct *task){
    kuid_t uid = task->real_cred->uid;

    printk(KERN_INFO "Executable: %s, pid: %d, tgid: %d, pr %d, static pr: %d, normaln pr: %d, real time pr: %u, nice: %d ,user ID: %u, memory pages: %lu, policy: %u, flags: %u, utime: %llu, stime: %llu \n", task->comm, task->pid, task->tgid, task->prio, task->static_prio, task->normal_prio, task->rt_priority, task_nice(task), from_kuid(&init_user_ns, uid), task->active_mm->total_vm, task->policy, task->flags, task->utime, task->stime);
    return 0;
}


static void print_children_info(struct task_struct *task)
{
    struct task_struct *child;

    list_for_each_entry(child, &task->children, sibling) {
        print_task_data(child);
        print_children_info(child);
    }

}

static int __init process_tree_module_init(void)
{
    struct task_struct *task;

    printk(KERN_INFO "From module process_tree\n");

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "Process with PID %d not found\n", pid);
        return -1;
    }
    print_task_data(task);
    print_children_info(task);

    return 0;
}
static void __exit process_tree_module_exit(void)
{
    printk(KERN_INFO "process tree module exit\n");
}

module_init(process_tree_module_init);
module_exit(process_tree_module_exit);


//module param 112 slajd
