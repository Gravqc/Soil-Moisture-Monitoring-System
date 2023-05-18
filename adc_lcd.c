#include<LPC17xx.h>
#include<stdio.h>
//#include<AN_LCD.h>
#define Ref_Vtg 5.000
#define Full_Scale 0xFFF //12 bit ADC



extern unsigned long int temp1 , temp2;



unsigned long int temp1=0, temp2=0 ;
#define RS_CTRL  0x00000100  //P0.8
#define EN_CTRL  0x00000200  //P0.9
#define DT_CTRL  0x00000FF0  //P0.4 to P0.7 data lines



void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);                
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts(char *);



int main(void){
unsigned int adc_temp;
unsigned int i;
float in_vtg;
char vtg[14],dval[14];
char Msg3[] = {"           Moisture:"};
//char Msg4[] = {"Moisture value:"};
SystemInit();
SystemCoreClockUpdate();
//LPC_SC->PCONP |= (1<<15);//Power for GPIO block
//LPC_SC->PCONP |= (1<<12);//peripheral devices for ADC turned on
lcd_init();
LPC_PINCON->PINSEL1 |= 1<<14; //P0.23 as AD0.0(23-16)*2
LPC_SC->PCONP |= (1<<12); //enable the peripheral ADC
temp1 = 0x80;
lcd_com();
delay_lcd(800);
lcd_puts(&Msg3[0]);
temp1 = 0xC0;
lcd_com();
delay_lcd(800);
//lcd_puts(&Msg4[0]);
while(1)
{
LPC_ADC->ADCR = (1<<0)|(1<<21)|(1<<24);//0x01200001; //ADC0.0, start conversion and operational
for(i=0;i<2000;i++); //delay for conversion
while(((adc_temp = LPC_ADC->ADGDR) & 0x80000000) ==0);
//wait till 'done' bit is 1, indicates conversion complete
adc_temp = LPC_ADC->ADGDR;
adc_temp >>= 4;
adc_temp &= 0x00000FFF; //12 bit ADC
in_vtg = (((float)adc_temp * (float)Ref_Vtg))/((float)Full_Scale); //calculating input analog
//voltage
sprintf(vtg,"%0.2fV",in_vtg);
//convert the readings into string to display on LCD
sprintf(dval,"%d",adc_temp);
for(i=0;i<2000;i++);
temp1 = 0x8A;
lcd_com();
delay_lcd(800);
lcd_puts(&vtg[0]);
temp1 = 0xCB;
lcd_com();
delay_lcd(800);
lcd_puts(&dval[0]);
for(i=0;i<200000;i++);
//for(i=0;i<7;i++)
//vtg[i] = dval[i] = 0x00;
//adc_temp = 0;
//in_vtg = 0;
}
}



void lcd_init()
{
    /* Ports initialized as GPIO */
   LPC_PINCON->PINSEL0 &= 0x00000000; //P0.0 to P0.15



    /* Setting the directions as output */
    LPC_GPIO0->FIODIR |= DT_CTRL;
    LPC_GPIO0->FIODIR |= RS_CTRL; 
    LPC_GPIO0->FIODIR |= EN_CTRL;    

    clear_ports();
    delay_lcd(3200);



    temp2 = (0x30);
    wr_cn();     
    delay_lcd(30000);

    temp2 = (0x30);
    wr_cn();
    delay_lcd(30000);    

    temp2 = (0x30);
    wr_cn();
    delay_lcd(30000);



    temp2 = (0x20);
    wr_cn();
    delay_lcd(30000);



    temp1 = 0x28; 
    lcd_com();
    delay_lcd(30000);

    temp1 = 0x0c;    
    lcd_com();
    delay_lcd(800);

    temp1 = 0x06;
    lcd_com();
    delay_lcd(800);

    temp1 = 0x01;
    lcd_com();
    delay_lcd(10000);

    temp1 = 0x80;
    lcd_com();
    delay_lcd(800);
    return;
}



void lcd_com(void)
{
    temp2 = temp1 & 0xf0;//move data (7-8+1) times : 26 - HN 
                           //place, 4 - Bits
    temp2 = temp2 ; //data lines from 4 to 7
    wr_cn();
    temp2 = temp1 & 0x0f; //7-4+1
    temp2 = temp2 << 4; 
    wr_cn();
    delay_lcd(1000);
    return;
}

 

// command nibble o/p routine
void wr_cn(void)                        //write command reg
{      
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2;    // Assign the value to the data 
                                 //lines    
    LPC_GPIO0->FIOCLR = RS_CTRL;        // clear bit RS
    LPC_GPIO0->FIOSET = EN_CTRL;       // EN=1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR  = EN_CTRL;    // EN =0
    return;

}

 

// data o/p routine which also outputs high nibble first
// and lower nibble next
void lcd_data(void)
{             
    temp2 = temp1 & 0xf0;
    temp2 = temp2 ;
    wr_dn();
    temp2= temp1 & 0x0f;    
    temp2= temp2 << 4;
    wr_dn();
    delay_lcd(1000);    
    return;
}

 

// data nibble o/p routine
void wr_dn(void)
{        
    clear_ports();

 

    LPC_GPIO0->FIOPIN = temp2;        // Assign the value to the 
                                      //data lines    
    LPC_GPIO0->FIOSET = RS_CTRL;    // set bit  RS
    LPC_GPIO0->FIOSET = EN_CTRL;       // EN=1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;    // EN =0
    return;
}

 

void delay_lcd(unsigned int r1)
{
      unsigned int r;
      for(r=0;r<r1;r++);
    return;
}

 

void clr_disp(void)
{
    temp1 = 0x01;
    lcd_com();
    delay_lcd(10000);
    return;
}
void clear_ports(void)
{
    /* Clearing the lines at power on */
    LPC_GPIO0->FIOCLR = DT_CTRL; //Clearing data lines
    LPC_GPIO0->FIOCLR = RS_CTRL;  //Clearing RS line
    LPC_GPIO0->FIOCLR = EN_CTRL; //Clearing Enable line

    return;
}

 

void lcd_puts(char *buf1){

   unsigned int i=0;

 

   while(buf1[i]!='\0'){

         temp1 = buf1[i];
     lcd_data();
         i++;

     if(i==27){
             temp1 = 0xc0;
             lcd_com();
         }   
   }
    return;
}