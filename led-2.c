#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/console_struct.h>
#define DEVICE_NAME "led"
#define CLASS_NAME "leddriver"

static int major;
static struct class* ledcharClass=NULL;
static struct device*ledcharDevice=NULL;
static ssize_t dev_read(struct file *,char *,size_t,loff_t *);
static ssize_t dev_write(struct file *,const char *,size_t,loff_t *);
extern int fg_console;

static struct file_operations fops={
	.read=dev_read,
	.write=dev_write,
	};
static int __init start(void){
	printk(KERN_ALERT "LED loaded\n");
	major=register_chrdev(0,DEVICE_NAME,&fops);
	if(major<0){
		printk(KERN_ALERT "LED char-dev module loaded!\n");
		return major;
		}
	ledcharClass=class_create(THIS_MODULE,CLASS_NAME);
	ledcharDevice=device_create(ledcharClass,NULL,MKDEV(major,0),NULL,DEVICE_NAME);
	printk("New char-dev 'LED' created with major %d and minor %d\n",major,0);
	return 0;
	}
static void __exit end(void){
	device_destroy(ledcharClass,MKDEV(major,0));
	class_unregister(ledcharClass);
	class_destroy(ledcharClass);
	unregister_chrdev(major,DEVICE_NAME);
	printk(KERN_ALERT "LED unloaded\n");
	}
static ssize_t dev_read(struct file *filp,char *buff,size_t length,loff_t *offset){
	const char *s_ptr;
	int error_code=0;
	ssize_t len=0;
	s_ptr=" - \n(y)\n = \n";
	len=min( (unsigned long)(strlen(s_ptr)-*offset),(unsigned long)(length));
	if(len<=0){
		return 0;
		}
		
	error_code=copy_to_user(buff, s_ptr+*offset,len);
	printk("The file was read! len:%d\n",len);
	printk("Error code: %d\n",error_code);
	
	*offset+=len;
	return len;
	}
static ssize_t dev_write(struct file *filp,const char *buff,size_t length,loff_t *offset){
	ssize_t ret;
	char nbuff[255];
	struct tty_driver *my_driver;
	my_driver=vc_cons[fg_console].d->port.tty->driver;
	ret=copy_from_user(nbuff,buff, sizeof(nbuff));
	nbuff[length-1]='\0';
	if(strcmp(nbuff,"on")==0){
		((my_driver->ops)->ioctl)(vc_cons[fg_console].d->port.tty,KDSETLED,4);
	}
	else if(strcmp(nbuff,"off")==0){
		((my_driver->ops)->ioctl)(vc_cons[fg_console].d->port.tty,KDSETLED,0);
	}
	else if(strcmp(nbuff,"reset")==0){
		((my_driver->ops)->ioctl)(vc_cons[fg_console].d->port.tty,KDSETLED,256); //state w/ higher bits will reset
	}
	return sizeof(nbuff);
	
	}
module_init(start);
module_exit(end);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edith F");
MODULE_DESCRIPTION("This is a char-dev of an LED\n");
