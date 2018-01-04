#include <linux/device.h>

extern struct bus_type ldd_bus_type;

struct ldd_device {
	const char	* name;

	struct device	dev;
};


struct ldd_driver {
	int (*probe)(struct ldd_device *);
	int (*remove)(struct ldd_device *);
	void (*shutdown)(struct ldd_device *);
	int (*suspend)(struct ldd_device *, pm_message_t state);
	int (*suspend_late)(struct ldd_device *, pm_message_t state);
	int (*resume_early)(struct ldd_device *);
	int (*resume)(struct ldd_device *);
	struct device_driver driver;
};

#define to_ldd_driver(x) container_of(x, struct ldd_driver, driver)
#define to_ldd_device(x) container_of(x, struct ldd_device, dev)

extern int register_ldd_device(struct ldd_device *);  
extern void unregister_ldd_device(struct ldd_device *);  
extern int register_ldd_driver(struct ldd_driver *);  
extern void unregister_ldd_driver(struct ldd_driver *);  

