/* tuxctl-ioctl.c
 *
 * Driver (skeleton) for the mp2 tuxcontrollers for ECE391 at UIUC.
 *
 * Mark Murphy 2006
 * Andrew Ofisher 2007
 * Steve Lumetta 12-13 Sep 2009
 * Puskar Naha 2013
 */

#include <asm/current.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/miscdevice.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
#include <linux/spinlock.h>

#include "tuxctl-ld.h"
#include "tuxctl-ioctl.h"
#include "mtcp.h"

#define debug(str, ...) \
	printk(KERN_DEBUG "%s: " str, __FUNCTION__, ## __VA_ARGS__)




unsigned long leds = 0; //storage for current leds
unsigned int prev_button = 0xFF; //storage for button pressed
unsigned long leds_req = 0; //storage for value received from tux on poll
unsigned char request = 0; //0 if request is not currently pending
unsigned char req_b = 0; //0 for the first packet, 1 for the second
unsigned long leds_req_buffer = 0; // buffer for leds_req
spinlock_t lock_leds = SPIN_LOCK_UNLOCKED; //lock to access leds
spinlock_t lock_butn = SPIN_LOCK_UNLOCKED; //lock to access prev_button
spinlock_t lock_poll = SPIN_LOCK_UNLOCKED; //lock to access leds_req
spinlock_t lock_char = SPIN_LOCK_UNLOCKED; //lock to access request
spinlock_t lock_buff = SPIN_LOCK_UNLOCKED; //lock to access leds_req_buffer
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/* tuxctl_buttons
 * arguments:
 *      arg - pointer in user space
 * return value:
 *      null
 * effect:
 *      reads prev_button to user
 * called from:
 *      TUX_BUTTONS
 */

void
tuxctl_buttons(unsigned long arg)
{
    unsigned long * buttns;
    unsigned int btn_temp;
    unsigned int * btn_temp_ptr;
    buttns = (unsigned long *)arg;
    spin_lock_irq(lock_butn);
    //set the bits of the low byte
    btn_temp = prev_button;
    //corresponding to currently pressed buttons
    spin_unlock_irq(lock_butn);
    btn_temp_ptr = & btn_temp;
    if (copy_to_user(buttns, btn_temp_ptr, 4))
    {
        //printk("\n bad copy!!! bad copy!!!");
        return;
    }
    else
    {
        return;
    }
}

/* tuxctl_set_led
 * arguments:
 *      arg - value to be set
 *      msg - where to set it
 * return value:
 *      character pointer containing message
 * effect:
 *      creates msg to push to txctl
 * called from:
 *      TUX_SET_LED
 */

char*
tuxctl_set_led(unsigned long arg, char* msg)
{
	//create a character array to store the parsed values in
    char display[4];
    char led_on;
    char dec_on;
    int i;
    display[0] = (char)(arg & 0x0000000F);
    display[1] = (char)((arg & 0x000000F0) >> 4);
    display[2] = (char)((arg & 0x00000F00) >> 8);
    display[3] = (char)((arg & 0x0000F000) >> 12);
    
	//determine what is supposed to be turned on
	led_on = (char)((arg & 0x000F0000) >> 16);
    dec_on = (char)((arg & 0x0F000000) >> 24);
    
	//set the opcode
    msg[0] = MTCP_LED_SET;
	//tell it which LEDs to turn on
	msg[1] = 0x0F;

    for(i = 2; i < 6; i++) //for each LED
    {
        //if we want the LED to be on
        if (((led_on) & (0x1 << (i-2)))>>(i-2)) {
        switch(display[i-2])//display these values
        {//move this to a function to call
            case 0xD:
				msg[i] = (char)0x4F;
				break;
            case 0x0:
                msg[i] = (char)0xE7;
                break;
            case 0x1:
                msg[i] = (char)0x06;
                break;
            case 0x2:
                msg[i] = (char)0xCB;
                break;
            case 0x3:
                msg[i] = (char)0x8F;
                break;
            case 0x4:
                msg[i] = (char)0x2E;
                break;
            case 0x5:
                msg[i] = (char)0xAD;
                break;
            case 0x6:
                msg[i] = (char)0xED;
                break;
            case 0x7:
                msg[i] = (char)0x86;
                break;
            case 0xB:
				msg[i] = (char)0x6D;
				break;
            case 0x8:
                msg[i] = (char)0xEF;
                break;
            case 0x9:
                msg[i] = (char)0xAF;
                break;
            case 0xA:
                msg[i] = (char)0xEE;
                break;
            case 0xC:
                msg[i] = (char)0xE1;
                break;
            case 0xE:
                msg[i] = (char)0xE9;
                break;
            case 0xF:
                msg[i] = (char)0xE8;
                break;
            default:
                msg[i] = (char)0x00;
        }
        }
        else //otherwise set msg to 0
        {
            msg[i] = 0x00;
        }
        if((dec_on>>(i-2))&0x1)//if the dp is supposed to be turned on
        {
            msg[i] = (char)((int)msg[i] | 0x10);//turn it on
        }
    }
    return msg; //send the packet
}

/* tuxctl_leds_poll
 * arguments:
 *      packet - the value to put in leds_req
 *      led_number - which led we're working on
 * return value:
 *      character pointer containing message
 * effect:
 *      writes to leds_req
 * called from:
 *      MTCP_POLL in handle_packet
 */

void
tuxctl_leds_poll(unsigned long packet, unsigned long led_number)
{
    //printk("\n printing poll packet: %x", (unsigned int)packet);
    //printk("\n printing led_number : %x", (unsigned int)led_number);
    //case 1:
        //check if decimal set decimal and fall through
    //case 2:
        //set value of led
        //break

	switch(packet)
	{
		case 0xF7:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xE7:
			leds_req = leds_req | (0x0 << (4*led_number));
			break;
		case 0x16:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0x06:
			leds_req = leds_req | (0x1 << (4*led_number));
			break;
		case 0xDB:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xCB:
			leds_req = leds_req | (0x2 << (4*led_number));
			break;
		case 0x9F:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0x8F:
			leds_req = leds_req | (0x3 << (4*led_number));
			break;
		case 0x3E:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0x2E:
			leds_req = leds_req | (0x4 << (4*led_number));
			break;
		case 0xBD:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xAD:
			leds_req = leds_req | (0x5 << (4*led_number));
			break;
		case 0xFD:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xED:
			leds_req = leds_req | (0x6 << (4*led_number));
			break;
		case 0x96:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0x86:
			leds_req = leds_req | (0x7 << (4*led_number));
			break;
		case 0xFF:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xEF:
			leds_req = leds_req | (0x8 << (4*led_number));
			break;
		case 0xBF:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xAF:
			leds_req = leds_req | (0x9 << (4*led_number));
			break;
		case 0xFE:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xEE:
			leds_req = leds_req | (0xA << (4*led_number));
			break;
		case 0x7D:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0x6D:
			leds_req = leds_req | (0xB << (4*led_number));
			break;
		case 0xF1:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xE1:
			leds_req = leds_req | (0xC << (4*led_number));
			break;
		case 0x5F:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0x4F:
			leds_req = leds_req | (0xD << (4*led_number));
			break;
		case 0xF9:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xE9:
			leds_req = leds_req | (0xE << (4*led_number));
			break;
		case 0xF8:
			leds_req = leds_req | (0x01000000 << led_number);
		case 0xE8:
			leds_req = leds_req | (0xF << (4*led_number));
			break;
		default: //the led is not on
			if(led_number == 0)
				leds_req = leds_req & 0xFFFEFFFF;
			else if(led_number == 1)
				leds_req = leds_req & 0xFFFDFFFF;
			else if (led_number == 2)
				leds_req = leds_req & 0xFFFBFFFF;
			else
				leds_req = leds_req & 0xFFF7FFFF;
			break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


/************************ Protocol Implementation *************************/

/* tuxctl_handle_packet()
 * IMPORTANT : Read the header for tuxctl_ldisc_data_callback() in
 * tuxctl-ld.c. It calls this function, so all warnings there apply
 * here as well.
 */
void tuxctl_handle_packet (struct tty_struct* tty, unsigned char* packet)
{
    unsigned a, b, c;
    
    //printk("\n handling packet");
    
    a = packet[0]; /* Avoid printk() sign extending the 8-bit */
    b = packet[1]; /* values when printing them. */
    c = packet[2];

    /*printk("packet : %x %x %x\n", a, b, c); */
    
    switch (a) {
        //user polls for LEDs
        //first two LEDs
		case MTCP_LEDS_POLL0:
		case __LEDS_POLL01:
		case __LEDS_POLL02:
		case __LEDS_POLL012:
		{
            unsigned long temp_leds;
            //printk("\n poll packet 1");
            //load 'A1 / A0' into b and c
			b = b & (((a & 0x00000001)<<7)|0x7F);
			c = c & (((a & 0x00000002)<<6)|0x7F);
            
            //set values of leds_req
			spin_lock(&lock_poll);
			leds_req = leds_req | (0xF0F30000);
			tuxctl_leds_poll(b,0);
			tuxctl_leds_poll(c,1);
            temp_leds = leds_req;
			spin_unlock(&lock_poll);
            if(req_b == 1)
            {
                leds_req_buffer = temp_leds;
                spin_lock(&lock_char);
                req_b = 0;
                request = 0;
                spin_unlock(&lock_char);
            }
            else
            {
                req_b = 1;
            }
		}
        //user polls for LEDs
        //second two LEDs
		case MTCP_LEDS_POLL1:
		case __LEDS_POLL11:
		case __LEDS_POLL12:
		case __LEDS_POLL112:
		{
            unsigned long temp_leds;
            //printk("\n poll packet 2");
            //load 'A1 / A0' into b and c
			b = b & (((a & 0x00000001)<<7)|0x7F);
			c = c & (((a & 0x00000002)<<6)|0x7F);

            //set values of leds_req
			spin_lock(&lock_poll);
			leds_req = leds_req | (0xF0FC0000);
			tuxctl_leds_poll(b,2);
			tuxctl_leds_poll(c,3);
            //set the buffer
			temp_leds = leds_req;
			spin_unlock(&lock_poll);
            
            //the request is now finished
            if(req_b == 1)
            {
                leds_req_buffer = temp_leds;
			    spin_lock(&lock_char);
			    req_b = 0;
                request = 0;
			    spin_unlock(&lock_char);
            }
            else
            {
                req_b = 1;
            }
			break;
		}
        //reset button is pressed
        case MTCP_RESET:
        {
            char msg[6];
			char data[1];
            //allow user to set leds again
			data[0] = MTCP_LED_USR;
			tuxctl_ldisc_put(tty, data, 1);
            //re-allow button press interrupt
			data[0] = MTCP_BIOC_ON;
			tuxctl_ldisc_put(tty,data,1);
			//printk("\n locking!");
            spin_lock(&lock_leds);
            tuxctl_set_led(leds,msg);
            spin_unlock(&lock_leds);
			//printk("\n unlocking!");
			tuxctl_ldisc_put(tty,msg,6);
			//printk("\n breaking");
            break;
        }
        //acknowledge
        case MTCP_ACK:
        {
            //yeah...
            //printk("\n acknowleged, number one    ");
            break;
        }
        //button was pressed
        case MTCP_BIOC_EVENT:
        {
            unsigned long btn;
            char left;
            char down;
            //allow us to detect button events
            //printk("\n button incoming!   ");
            //switch left and down because biocevent is
            //messed up...
            left = (c & 0x02)<<1;
            down = (c & 0x04)>>1;
            c = (c & 0xF9) | left | down;
            btn = (0x0000000F & b) | ((0x0000000F & c)<<4);
            spin_lock(&lock_butn);
            //store the button pressed
            prev_button = btn;
            spin_unlock(&lock_butn);
            //printk("\n button completed! %lu   ", btn);
            break;
        }
        default:
            break;
    }
    
    
}

/******** IMPORTANT NOTE: READ THIS BEFORE IMPLEMENTING THE IOCTLS ************
 *                                                                            *
 * The ioctls should not spend any time waiting for responses to the commands *
 * they send to the controller. The data is sent over the serial line at      *
 * 9600 BAUD. At this rate, a byte takes approximately 1 millisecond to       *
 * transmit; this means that there will be about 9 milliseconds between       *
 * the time you request that the low-level serial driver send the             *
 * 6-byte SET_LEDS packet and the time the 3-byte ACK packet finishes         *
 * arriving. This is far too long a time for a system call to take. The       *
 * ioctls should return immediately with success if their parameters are      *
 * valid.                                                                     *
 *                                                                            *
 ******************************************************************************/

int 
tuxctl_ioctl (struct tty_struct* tty, struct file* file, 
	      unsigned cmd, unsigned long arg)
{
    switch (cmd) {
    //initialize
    //takes no arguments just sets up the tux
	//turns bioc on and allows user to set leds
	case TUX_INIT:
    {
        char data[1];
        char message[6];
        int i;
        //printk("\n initializing!");

        data[0] = MTCP_LED_USR;
        tuxctl_ldisc_put(tty, data, 1);

        data[0] = MTCP_BIOC_ON;
        tuxctl_ldisc_put(tty,data,1);

        for(i = 0; i < 6; i++)
        {
            message[i] = 0;
        }

        message[0] = MTCP_LED_SET;
        message[1] = 0xF;
        tuxctl_ldisc_put(tty, message, 6);
		
        return 0;
    }
    //return the button
    //argument is a pointer to a 32 bit integer
    //returns -EINVAL if pointer is invalid
    //otherwise it sets the bits of the lower byte according to documentation
	case TUX_BUTTONS:
    {
        //printk("\n buttons!");
    	//if we get a null ptr return error
        if (arg == 0) {
            return -EINVAL;
        }
        tuxctl_buttons(arg);
        return 0;
    }
    //set the leds
    //argument is a 32 bit integer specifying
	//what leds should be set as
	case TUX_SET_LED:
    {
        char msg[6];
		spin_lock_irq(lock_leds);
        leds = arg;
        spin_unlock_irq(lock_leds);
		//printk("\n leds!");
		tuxctl_set_led(arg,msg);
		tuxctl_ldisc_put(tty,msg,6);
        return 0;
    }
    //request the LED
    //initiates a request to the tux
    //if a request is already pending
    //we return an efault
    //otherwise we initiate a request and return 0
    case TUX_LED_REQUEST:
    {
		char msg[1];
        //printk("\n led request!");
        msg[0] = MTCP_POLL_LEDS;
		spin_lock_irq(&lock_char);
		if(request == 1)
		{
            // a request is pending
			spin_unlock_irq(&lock_char);
			return -EFAULT;
		}
        // set the request to be pending
		request = 1;
		spin_unlock_irq(&lock_char);
        //start the poll
		tuxctl_ldisc_put(tty,msg,1);
		return 0;
    }
    //acknowledge the request
    //ie check for request
    //if a request is pending we return 0
    //otherwise we return 1
    case TUX_LED_ACK:
    {
		char retval;
        //printk("\n ack!");
        spin_lock_irq(&lock_char);
		retval = request;
		spin_unlock_irq(&lock_char);
		if(retval == 1)
			return 0;
		return 1;
    }
    //read what was returned from the last request
    //argument is a pointer to a 32 bit integer
    //store values into 32 bit integer pointer
    //of last led request
    case TUX_READ_LED:
    {
        unsigned long * from;
		from = & leds_req_buffer;
		//printk("\n read led!");

    	//if our parameter is invalid
        if (arg == 0) {
            return -EFAULT;
        }
		copy_to_user((unsigned long *) arg,from,4);
        return 0;
    }
	default: //for everything else the in value is wrong
    {//return an error
        //printk("\n default?!");
	    return -EINVAL;
    }
    }
}
