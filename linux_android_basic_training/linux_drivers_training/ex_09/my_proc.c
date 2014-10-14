#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>  /* use the proc fs */
#include<asm/uaccess.h>  /* for copy_from_user */
#include<linux/init.h>
#include<linux/vmalloc.h>
#include<linux/string.h>

#define PROCFS_MAX_SIZE  1024
#define PROCFS_NAME  "buffer1k"

static struct proc_dir_entry *my_proc_file, *dir;

static char procfs_buffer[PROCFS_MAX_SIZE];

static unsigned long procfs_buffer_size = 0;

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data){
	int ret;

	printk("procfile_read(/proc/%s)called\n", PROCFS_NAME);

	if(offset > 0){
		ret = 0;
	} else{
		memcpy(buffer, procfs_buffer, procfs_buffer_size);
		ret = procfs_buffer_size;
	}

	return ret;
}

int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data){
	procfs_buffer_size = count;

	if(procfs_buffer_size > PROCFS_MAX_SIZE){
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

	if(copy_from_user(procfs_buffer, buffer, procfs_buffer_size)){
		return -EFAULT;
	}

	return procfs_buffer_size;
}

int init_my_proc(void)
{
	dir = proc_mkdir("mydir", NULL);
	my_proc_file = create_proc_entry(PROCFS_NAME, 0644, dir);

	if(my_proc_file == NULL){
		remove_proc_entry(PROCFS_NAME, dir);  /* remove files under mydir */
		remove_proc_entry("mydir", NULL);  /* remove mydir */
		printk("error: could not initialize /proc/%s\n",PROCFS_NAME);
		return -ENOMEM;
	}

	my_proc_file->read_proc = procfile_read;
	my_proc_file->write_proc = procfile_write;
//	my_proc_file->owner = THIS_MODULE;
	my_proc_file->mode = S_IFREG|S_IRUGO;
	my_proc_file->uid = 0;
	my_proc_file->gid = 0;
	my_proc_file->size = 37;

	printk("/proc/%s created\n", PROCFS_NAME);

	return 0; /* everything is ok */
}

void cleanup_my_proc(void)
{
	remove_proc_entry(PROCFS_NAME, dir);
	remove_proc_entry("mydir", NULL);

	printk("/proc/%s removed\n", PROCFS_NAME);
}

module_init(init_my_proc);
module_exit(cleanup_my_proc);
MODULE_LICENSE("GPL");
