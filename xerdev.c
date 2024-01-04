#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
//	声明区域
#define DEVICE_NAME "xerdev"
#define CLASS_NAME "xerclass"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xerolyskinner");
MODULE_DESCRIPTION("A simple Linux driver");
MODULE_VERSION("0.1");

static char message[256] = {0};

static int majorNumber;
static struct class* charClass = NULL;
static struct device* charDevice = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
//	声明区域
static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
//	声明区域
static int dev_open(struct inode* inodep, struct file* filep) {
    printk(KERN_NOTICE"CharDriver: Device has been opened\n");
    return 0;
}
//--------------------------------------------------------------------------------------------------
static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset) {
	int error_count = 0;
	error_count = copy_to_user(buffer,message,len);
	if (error_count == 0) return 0;
	else {
		printk(KERN_INFO "CharDriver: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;
	}
}
//--------------------------------------------------------------------------------------------------
static ssize_t dev_write(struct file* filep, const char* buffer, size_t len, loff_t* offset) {
	copy_from_user(message,buffer,len);
	printk(KERN_INFO "CharDriver: Received %zu characters from the user\n", len);
	return len;
}
//--------------------------------------------------------------------------------------------------
static int dev_release(struct inode* inodep, struct file* filep) {
    printk(KERN_NOTICE"CharDriver: Device successfully closed\n");
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
//	声明区域
static int __init char_init(void) {
    printk(KERN_INFO "CharDriver: Initializing the CharDriver\n");

    // Register the character device
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "CharDriver failed to register a major number\n");
        return majorNumber;}

    // Register the device class
    charClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(charClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(charClass);}

    // Register the device driver
    charDevice = device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(charDevice)) {
        class_destroy(charClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(charDevice);}

    printk(KERN_INFO "CharDriver: CharDriver initialized with major number %d\n",majorNumber);
    return 0;
}
//--------------------------------------------------------------------------------------------------
static void __exit char_exit(void) {
    device_destroy(charClass, MKDEV(majorNumber, 0));
    
    class_unregister(charClass);
    class_destroy(charClass);
    
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "CharDriver: CharDriver removed\n");
}
//--------------------------------------------------------------------------------------------------
module_init(char_init);
module_exit(char_exit);
////////////////////////////////////////////////////////////////////////////////////////////////////

