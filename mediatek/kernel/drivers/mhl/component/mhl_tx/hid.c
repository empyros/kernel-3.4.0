#include <linux/input.h>
#include <linux/module.h>

static struct input_dev *mhl_hid_dev;
static __u32 keycode[] = {1}; //TODO: fill it
static uint8_t key_status[] = {0,0,0,0,0};
static uint8_t mouse_btn = 0;
static __u32 cmdmmap[] = {
  BTN_LEFT, BTN_MIDDLE, BTN_RIGHT
};
static __u32 cmdkmap[] = {
  KEY_LEFTCTRL,  KEY_LEFTSHIFT,  KEY_LEFTALT,  KEY_LEFTMETA, 
  KEY_RIGHTCTRL, KEY_RIGHTSHIFT, KEY_RIGHTALT, KEY_RIGHTMETA 
};

static void parseScratchPad() {
  uint8_t *data = &(mhlTxConfig.localScratchPad[0]);
  uint8_t tmp_btn = 0, x, y, z;
  int i, j;
  if(data[0]==2) { 
    //TODO: plug/unplug event
  } else if(data[0]==1) {
    if(data[1]==2) { // Mouse
      x = data[3]<<8 + data[4];
      y = data[5]<<8 + data[6];
      z = data[7];
      tmp_btn = mouse_btn ^ data[2];
      for(i=1;i<8;i=i<<1) {
        if(tmp_btn & i) input_report_key(mhl_hid_dev, cmdmmap[i], ~(data[2] & i));
      }
      // event generated only for non-zero value by default
      input_report_rel(mhl_hid_dev, REL_X, x);
      input_report_rel(mhl_hid_dev, REL_Y, y);
      input_report_rel(mhl_hid_dev, REL_WHEEL, z);
    } else if(data[1]==3) { // Keyboard
      tmp_btn = key_status[0] ^ data[2];
      // TODO: send meta key separately, or merge into one key?
      for(i=1;i<=256;i=i<<1) {
        if(tmp_btn & i) input_report_key(mhl_hid_dev, cmdkmap[i], ~(data[2] & i));
      }
      // naive approach. use bit vector instead?
      for(i=3;i<7;i++) {
        for(j=1;j<5;j++) {
          if(data[i]==key_status[j]) break;
        }
        if(j==5) input_report_key(mhl_hid_dev, 0/*TODO: Map usage id to kcode*/, 1);
      }
      for(j=1;j<5;j++) {
        if(key_status[j]) {
          input_report_key(mhl_hid_dev, 0/*TODO: uid to kcode*/, 0);
        }
      }
      for(i=3;i<7;i++) {
        key_status[i-2] = data[i];
      }
      input_sync(mhl_hid_dev);
    }
  } else ; //TODO: assert, wrong command
}

// might or might not need these. could write in MHL driver's init/exit function
static int __init mhl_hid_init(void) {
  int err, i;
  mhl_hid_dev = input_allocate_device();
  if (!mhl_hid_dev) {
    printk(KERN_ERR "mhl_hid_dev: Not enough memory\n");
    return -ENOMEM;
  }
  set_bit(EV_KEY, mhl_hid_dev->evbit); // for kpd and mouse
  //set_bit(EV_ABS, mhl_hid_dev->evbit); // for touchpad, not used for now
  set_bit(EV_REL, mhl_hid_dev->evbit); // for mouse
  for(i=sizeof(keycode)-1;i>=0;i--) {
    set_bit(keycode[i], mhl_hid_dev->keybit);
  }
  err = input_register_device(mhl_hid_dev);
  if(err) {
    printk(KERN_ERR "mhl_hid_dev: Failed to register device\n");
    input_free_device(mhl_hid_dev);
    return err;
  }
  return 0;
}

static void __exit mhl_hid_exit(void) {
  input_unregister_device(mhl_hid_dev);
}
