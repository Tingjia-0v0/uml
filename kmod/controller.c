#define LINUX

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

#include <linux/delay.h>

#include "../linux-6.14/include/linux/printk.h"
#include "../linux-6.14/include/linux/sched/task.h"

// prototypes
typedef void (* st_control_t) (void) ;
extern void set_module_control(st_control_t __module_st_control);

typedef void (*st_record_t)(struct  task_struct *);
void set_module_record(st_record_t __module_st_record);

# define MAX_TEST_TASKS 100

typedef struct test_task_entry {
    struct task_struct *p;
    int status; // runnable or unrunnable
} test_task_entry_t;
test_task_entry_t *testTasks = NULL;

unsigned long current_entry_id = 0;

void sched_tester_control(void) {
    int n_entries = current_entry_id < MAX_TEST_TASKS ?
               current_entry_id : MAX_TEST_TASKS;
    struct task_struct *p;
    int i;

    // pr_info("number of test processes: %d\n", n_entries);
    for (i = 0; i < n_entries; i++) {
        if (testTasks[i].status == 1) {
            p = testTasks[i].p;
            p->jobctl |= JOBCTL_TRAP_FREEZE;
            signal_wake_up(p, false);
            pr_info("sched_tester: freeze the test process %d\n", p->pid);
            testTasks[i].status = 0;
        }
    }
}

void sched_tester_record(struct task_struct *p) {
    unsigned long __current_entry_id;
    __current_entry_id = __sync_fetch_and_add(&current_entry_id, 1);
    testTasks[__current_entry_id].p = p;
    testTasks[__current_entry_id].status = 1;

    pr_info("sched_tester: add task %d to test tasks array\n", testTasks[__current_entry_id].p->pid);
}

static int __init my_init(void) {
    unsigned long testTasksSize = MAX_TEST_TASKS * sizeof(test_task_entry_t);
    pr_info("sched_tester: init controller module\n");
    if(!(testTasks = vmalloc(testTasksSize)))
    {
        printk("sched_tester: kmalloc fail (%lu bytes)\n",
               testTasksSize);
        return -ENOMEM;
    }

    set_module_record(sched_tester_record);
    pr_info("sched_tester: set st_record in the module\n");
    set_module_control(sched_tester_control);
    pr_info("sched_tester: set st_control in the module\n");

    return 0;
}

static void __exit my_cleanup(void) {
    set_module_record(NULL);
    set_module_control(NULL);
    ssleep(1);
    vfree(testTasks);
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");