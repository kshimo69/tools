/* Copyright (C) 2011 Shimomura Kimihiko <kshimo69@gmail.com> */
#include <linux/module.h>
#include <linux/kernel.h>

char *p = NULL;

int init_module(void) {
	printk(KERN_INFO "null pointer access.\n");

	*p = 1;
	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "Goodby world.\n");
}
