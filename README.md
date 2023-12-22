# Soil Moisture Sensor Using LPC1768

## 📖 Introduction
This project involves a practical application of the LPC1768 microcontroller to measure soil moisture. The system is intended for anyone interested in soil health, particularly in agriculture or gardening, providing real-time soil moisture data displayed via an LCD.

## 🛠️ Installation
**Requirements:**
- ARM LPC1768 Microcontroller
- Soil Moisture Sensor
- LCD Display
- Connecting wires and breadboard

**Setup:**
- Assemble the hardware according to the circuit diagram provided.
- Set up the ARM MBED development environment.
- Compile and upload the provided code to the LPC1768 microcontroller.

## 👨‍🏫 Get Started
To get started:
1. Power on the system.
2. Insert the soil moisture sensor into the soil.
3. Read the moisture levels from the LCD display.

## 📘 Documentation
The project includes the following main components:
- **LPC1768 Microcontroller**: The core controller running the main program.
- **Soil Moisture Sensor**: Measures the soil's moisture level.
- **LCD Display**: Shows the real-time moisture data.

## Code Explanation
### Main Function
The `main()` function initializes the system and enters an infinite loop to read and display the moisture levels.
```c
int main(void){
unsigned int adc_temp;
unsigned int i;
float in_vtg;
char vtg[14],dval[14];
char Msg3[] = {"           Moisture:"};
SystemInit();
SystemCoreClockUpdate();
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
while(1)
{
LPC_ADC->ADCR = (1<<0)|(1<<21)|(1<<24);//0x01200001; //ADC0.0, start conversion and operational
for(i=0;i<2000;i++); //delay for conversion
while(((adc_temp = LPC_ADC->ADGDR) & 0x80000000) ==0);
adc_temp = LPC_ADC->ADGDR;
adc_temp >>= 4;
adc_temp &= 0x00000FFF; //12 bit ADC
in_vtg = (((float)adc_temp * (float)Ref_Vtg))/((float)Full_Scale); //calculating input analog
sprintf(vtg,"%0.2fV",in_vtg);
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
}
}
```

### LCD Initialization
The `lcd_init()` function sets up the GPIO and initializes the LCD display for use.
```c
void lcd_init()
{
   LPC_PINCON->PINSEL0 &= 0x00000000; //P0.0 to P0.15
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
```

### ADC Reading
The code reads from the ADC, converts the reading to a voltage, and displays the result on the LCD. Within the main loop, the ADC is read, and the soil moisture level is calculated and converted into a voltage value to be displayed.
```c
LPC_ADC->ADCR = (1<<0)|(1<<21)|(1<<24); // Configuring ADC
for(i=0;i<2000;i++); //delay for conversion
adc_temp = LPC_ADC->ADGDR; // Reading ADC value
// Further processing to calculate voltage and display...
```

### Displaying on LCD
The `lcd_puts(char *buf1)` function displays the given string on the LCD.
```c
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
```
Refer to the c file for full code and visit the ARM MBED online resources for more on programming LPC1768.

## 🎥 Working Demo
To see the Soil Moisture Sensor in action, check out the working demo [here](https://drive.google.com/file/d/1QWmSQjbM47NHiyhXuVdhULx4H3t7a0_W/view?usp=drive_link).
