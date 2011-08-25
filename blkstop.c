/* Copyright (C) 2011 Shimomura Kimihiko <kshimo69@gmail.com> */
#include <linux/version.h>
#include <linux/modversions.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/genhd.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
extern int int_init( void );
extern void int_cleanup( void );
module_init(int_init);
module_exit(int_cleanup);
#else
#define int_init init_module
#define int_cleanup cleanup_module
#endif

/*
 * blkstop -- block device stop module
 * This program can stop a specified block device.
 *
 *  # echo "off 8 0" > /proc/blkstop
 * meens "stop any access to device(8,0)".
 *
 *  # echo "on 8 0" > /proc/blkstop
 * meens "resume any access to device(8,0)".
 */

#define MAX_MINOR 16 /* max partition number is 16 */


static unsigned long save_nr_sect[MAX_BLKDEV][MAX_MINOR];


static int proc_blkstop_read( char *kbuf, char **start, off_t offset,
                                        int count, int *eof, void *data )
{
	int  i, j;

	sprintf( kbuf, "Usage: echo <on|off> major minor > /proc/blkstop\n" );
	sprintf( kbuf, "%sShowing off-lined devices below:\n", kbuf );
	for(i = 0; i < MAX_BLKDEV; i++)
		for(j = 0; j < MAX_MINOR; j++) {
			if(save_nr_sect[i][j] == 0)
				continue;
			sprintf( kbuf, "%s\tdev( %d %d ): nr_sects = %ld\n",
				 kbuf, i, j, save_nr_sect[i][j] );
		}
	return( strlen( kbuf ) );
}



static int proc_blkstop_write( struct file *file, const char *kbuf,
				unsigned long count, void *data )
{
	int   major, minor;
	char  buf[count+1];
	char  onoff[count+1];
	struct gendisk *g;

	sprintf( buf, "%.*s", (int)count, kbuf );

	if(sscanf( buf, "%s %d %d", onoff, &major, &minor ) != 3) {
		printk( KERN_WARNING "blkstop: argument error. %s\n", buf );
		return( count ); /* dummy */
	}

	if((major >= MAX_BLKDEV || minor >= MAX_MINOR) ||
	   (g = get_gendisk( MKDEV( major, minor ) )) == NULL) {
		printk( KERN_ERR "blkstop: invalid major/minor(%d,%d)\n",
			major, minor );
		return( count ); /* dummy */
	}

	if(strcmp( onoff, "on" ) == 0) {
		/* for ON-LINE */
		printk( KERN_WARNING "blkstop: dev(%d,%d) becames on-line\n", 
			major, minor );
		if(save_nr_sect[major][minor] == 0) {
			printk( KERN_ERR "blkstop: perhaps the device(%d,%d) "
				"has already been on-lined.\n", major, minor );
			return( count ); /* dummy */
		}
		g->part[minor].nr_sects = save_nr_sect[major][minor];
		save_nr_sect[major][minor] = 0;
	}
	else if(strcmp( onoff, "off" ) == 0) {
		/* for OFF-LINE */
		printk( KERN_WARNING "blkstop: dev(%d,%d) becames off-line\n", 
			major, minor );
		if(save_nr_sect[major][minor] != 0) {
			printk( KERN_ERR "blkstop: perhaps the device(%d,%d) "
				"has already been off-lined.\n", major, minor );
			return( count ); /* dummy */
		}
		save_nr_sect[major][minor] = g->part[minor].nr_sects;
		g->part[minor].nr_sects = 0;
	}
	else {
		/* Invalid command */
		printk( KERN_ERR "blkstop: invalid command(%s)\n", onoff );
		return( count ); /* dummy */
	}

	return( count ); /* dummy */
}



/* register /proc/blkstop entry */
static int proc_int_create( void )
{
	struct proc_dir_entry *blkstop = 0;

	blkstop = create_proc_entry( "blkstop", S_IFREG|S_IWUGO, 0 );
	if(blkstop == NULL)
		return( 1 );
	blkstop->read_proc  = proc_blkstop_read;
	blkstop->write_proc = proc_blkstop_write;

	return( 0 );
}

/* destory /proc/blkstop entry */
static void proc_int_destroy( void )
{
	remove_proc_entry( "blkstop",  0 );
}

/* init_module: module initialization */
int int_init( void )
{
	int  i, j;

	printk( "loaded: --------- [ Kernel blkstop module ] --------\n" );
	printk( "    This is very dangerous module, so use only for test.\n" );
	printk( "    \"echo on 8 1 > /proc/blkstop\" meens make /dev/sda1 off-line.\n" );

	for(i = 0; i < MAX_BLKDEV; i++)
		for(j = 0; j < MAX_MINOR; j++)
			save_nr_sect[i][j] = 0;
	proc_int_create();

	return( 0 );
}

/* cleanup_module: module cleaning up */
void int_cleanup( void )
{
	proc_int_destroy();
	printk( "removed: --------- [ Kernel blkstop module ] --------\n" );
}
