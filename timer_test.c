#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/utsname.h>
#include <asm/pgtable.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

struct timer_list timer1;

int i;

int page_by_address(struct task_struct *task, const unsigned long addr)
{
    pgd_t *pgd;
    pte_t *ptep, pte;
    pud_t *pud;
    pmd_t *pmd;
    unsigned int accessed = 0;
    struct mm_struct *mm = task->mm;
    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        goto out;
    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud))
        goto out;
    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;
    ptep = pte_offset_map(pmd, addr);
    if (!ptep)
        goto out;
    pte = *ptep;
    if (pte_young(pte))
	accessed = 1;
    pte = pte_mkold(pte);
    *ptep = pte;
out:
    return accessed;
}

static int mm_exp_load(void)
{
	struct mm_struct *mm;
	struct task_struct *task;
	struct task_struct *task_thread;
	for_each_process(task) 
	{		
		if (strcmp(task->comm,"qemu-system-x86") == 0) 
		{
			unsigned int page_accessed = 0;
			
			task_thread = task;
			do {
				mm = task_thread->mm;
				const struct vm_area_struct *vma = mm->mmap;
 		       			while (vma != NULL) 
					{
    		    	 	  	 	unsigned long address;
        	 	   			for (address = vma->vm_start; address < vma->vm_end; address += PAGE_SIZE) 
						 {
        	  	   			   page_accessed += page_by_address(task_thread, address);
        	  	 			 }
        	  	  		vma = vma->vm_next;
       					}	
		       }while_each_thread(task, task_thread);
		       printk("\n%s[%d] = %d\n", task->comm, task->pid,page_accessed);
		}
	 }
	printk("-------------------------");
        return 0;
}

void timer1_routine(unsigned long data)
{
mm_exp_load();
//printk(KERN_ALERT"Inside Timer Routine count-> %d data passed %ld\n",i++,data);
mod_timer(&timer1, jiffies + HZ*1); /* restarting timer */
}


static int timer_module_init(void)
{


init_timer(&timer1);

timer1.function = timer1_routine;
timer1.data = 1;
timer1.expires = jiffies + HZ; /* 1 second */
add_timer(&timer1); /* Starting the timer */

printk(KERN_ALERT"Timer Module loaded\n");
return 0;
}

static void timer_module_exit(void)
{
del_timer_sync(&timer1); /* Deleting the timer */

printk(KERN_ALERT "Timer module unloaded \n");
}

module_init(timer_module_init);
module_exit(timer_module_exit);
