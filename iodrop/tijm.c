/*
 * scsi timeout injection module
 */
#include <linux/module.h>
#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>

#define MODNAME "tijm2"
#define EXECCNT 0

static struct scsi_host_template *sht;
static char config[32];
//static int inject_done;
static unsigned int cmd_cnt;

static struct target {
	short host;
	uint channel;
	uint id;
	uint lun;
} st;

static int (*org_qc)(struct scsi_cmnd *,
		     void (*done)(struct scsi_cmnd *));

static unsigned char rw_io[] = {
	TEST_UNIT_READY
//	READ_6, READ_10, READ_12, READ_16,
//	WRITE_6, WRITE_10, WRITE_12, WRITE_16
};

static inline int check_io(unsigned char c)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(rw_io); i++)
		if (c == rw_io[i])
			return 1;
	return 0;
}

static inline int check_dev(struct target *st, struct scsi_cmnd *cmd)
{
	return (st->host == cmd->device->host->host_no &&
		st->channel == cmd->device->channel &&
		st->id == cmd->device->id &&
		st->lun == cmd->device->lun);
}

static void iodrop_done(struct scsi_cmnd *cmd)
{
	//Do nothing.
	printk(KERN_ERR
			"[%s]: %s cmd=%p sn=%ld jiffies=%lx per_TO=%d *****\n"
			"cmd[]=[%x][%x][%x][%x][%x][%x][%x][%x][%x][%x]\n", 
			MODNAME, __func__, cmd, cmd->serial_number, jiffies, cmd->timeout_per_command,
			cmd->cmnd[0], cmd->cmnd[1], cmd->cmnd[2], cmd->cmnd[3], cmd->cmnd[4], cmd->cmnd[5],
			cmd->cmnd[6], cmd->cmnd[7], cmd->cmnd[8], cmd->cmnd[9]);

	return;
}

static int dbg_qc(struct scsi_cmnd *cmd,
		  void (*done)(struct scsi_cmnd *))
{
	int ret = 0;


	preempt_disable();

//	if(cmd_cnt < EXECCNT || inject_done){


	if (check_dev(&st, cmd) && check_io(cmd->cmnd[0])){
		if(++cmd_cnt < EXECCNT){
			goto call_org;
		}

		/* inject scsi timeout. Don't call "scsi_done". */
		printk(KERN_ERR
			"[%s]: inject set cmd=%p sn=%ld jiffies=%lx per_TO=%d\n"
			"cmd[]=[%x][%x][%x][%x][%x][%x][%x][%x][%x][%x]\n", 
			MODNAME, cmd, cmd->serial_number, jiffies, cmd->timeout_per_command,
			cmd->cmnd[0], cmd->cmnd[1], cmd->cmnd[2], cmd->cmnd[3], cmd->cmnd[4], cmd->cmnd[5],
			cmd->cmnd[6], cmd->cmnd[7], cmd->cmnd[8], cmd->cmnd[9]);
		done=iodrop_done;
	}


call_org:
	ret = org_qc(cmd, done);

	preempt_enable();
	return ret;
}

static int __init scsi_timeout_module_init(void)
{
	int ret;


//	inject_done=0;
	cmd_cnt=0;

	ret = sscanf(config, "%lx,%hd:%d:%d:%d",
		     (ulong *)&sht,
		     &st.host, &st.channel, &st.id, &st.lun);
	if (ret != 5) {
		printk(KERN_ERR "[%s]: invalid options\n", MODNAME);
		return -1;
	}

	org_qc = sht->queuecommand;
	sht->queuecommand = dbg_qc;

	printk(KERN_ERR
	       "[%s]: loaded %p %hd:%d:%d:%d\n",
		MODNAME, sht, st.host, st.channel, st.id, st.lun);

	return 0;
}

static void __exit scsi_timeout_module_exit(void)
{
	sht->queuecommand = org_qc;
	synchronize_sched();

}

module_init(scsi_timeout_module_init);
module_exit(scsi_timeout_module_exit);
module_param_string(param, config, 32, 0);

MODULE_LICENSE("GPL");


