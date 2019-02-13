#include<reg51.h>
#define display_port P1 //Data pins connected to port 1 on microcontroller

int dis=0;
char v=0;
char n=0;

sbit rs = P2^0;  //RS pin connected to pin 2 of port 3
sbit rw = P2^1;  // RW pin connected to pin 3 of port 3
sbit e =  P2^2;  //E pin connected to pin 4 of port 3
sbit led=P2^3;
sbit led1=P2^4;
sbit trin=P0^0;
sbit trout=P0^1;

void display(int);
void extr0(void);
void msdelay(unsigned int);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_init();
void timer0_ISR(void);

void display(int num)
{
	int t,u,f;
	t=num%10;
	num=num/10;
	u=num%10;
	num=num/10;
	f=num%10;
	lcd_data('0'+f);
	msdelay(25);
	lcd_data('0'+u);
	msdelay(25);
	lcd_data('0'+t);
	msdelay(25);
}

void extr0(void) interrupt 0
{
	led=~led;
	dis=dis+2;
	n++;
}

void timer0_ISR(void) interrupt 1
{
	static int c=0;
	c++;
	while(c>1000)
	{
		led1=~led1;
		v=(n*2*18)/5;
		c=0;
		n=0;
	}
	TH0=0xFC;
	TL0=0x18;
}

void msdelay(unsigned int time)  // Function for creating delay in milliseconds.
{
    unsigned i,j ;
    for(i=0;i<time;i++)    
    for(j=0;j<1275;j++);
}
void lcd_cmd(unsigned char command)  //Function to send command instruction to LCD
{
    display_port = command;
    rs= 0;
    rw=0;
    e=1;
    msdelay(1);
    e=0;
}

void lcd_data(unsigned char disp_data)  //Function to send display data to LCD
{
    display_port = disp_data;
    rs= 1;
    rw=0;
    e=1;
    msdelay(1);
    e=0;
}

void lcd_init()    //Function to prepare the LCD  and get it ready
{
    lcd_cmd(0x38);  // for using 2 lines and 5X7 matrix of LCD
    msdelay(10);
    lcd_cmd(0x0E);  // turn display ON, cursor blinking
    msdelay(10);
    lcd_cmd(0x01);  //clear screen
    msdelay(10);
    lcd_cmd(0x80);  // bring cursor to position 1 of line 1
    msdelay(10);
}
void main(void)
{
     do
     {	unsigned char a[13]="Speed(km/hr):";   
    	unsigned char b[13]="Distance(m): ";
    	char l=0;
    	led=0;
    	led1=0;
    	TMOD=0X01;
    	IT0=1;
    	EX0=1;
    	ET0=1;
    	
    	TH0=0XFC;
    	TL0=0X18;
    	TR0=1;
    	EA=1;
    	trin=1;
    	lcd_init();
    	
    	
    	while(l<13) 
	    {
	        lcd_data(a[l]);
	        l++;
	        msdelay(10);
	    }
	    
	    display(v);
	    
	    lcd_cmd(0xC0);  // bring cursor to position 1 of line 2
	    msdelay(10);
	    
	    for(l=0;l<13;l++)
	    {
	        lcd_data(b[l]);
	        msdelay(10);
	    }
	    display(dis);
	    if(trin==1)
	    {
	    	trout=1;
	    }
	    else
	    {
	    	trout=0;
	    }
	   } 
      while(1);
}
	 
