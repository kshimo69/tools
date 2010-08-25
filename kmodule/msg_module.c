/* Copyright (C) 2010 Shimomura Kimihiko <kshimo69@gmail.com> */
#include <linux/module.h>
#include <linux/kernel.h>

static int __init msg_module_init(void)
{
  printk(KERN_DEBUG "debug\n");
  printk(KERN_INFO "info\n");
  printk(KERN_NOTICE "notice\n");
  printk(KERN_WARNING "warning\n");
  printk(KERN_ERR "err\n");
  printk(KERN_CRIT "crit\n");
  printk(KERN_ALERT "alert\n");
  printk(KERN_EMERG "emerg\n");
  return 0;
}

static void __exit msg_module_exit(void)
{
  printk(KERN_INFO "exit module\n");
}

module_init(msg_module_init);
module_exit(msg_module_exit);

MODULE_LICENSE("GPL");
