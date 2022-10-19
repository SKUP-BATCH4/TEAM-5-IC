/*
 * File:   team5_IC.c
 * Author: user
 *
 * Created on October 19, 2022, 12:36 PM
 */


#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L
#pragma config STVR = OFF       // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will not cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-Voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)
// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)
#include <xc.h>
#define ENABLE 1
#define DISABLE  0
#define _XTAL_FREQ 20000000
unsigned char can_read_msg[11]; //DECLARING ARRAY FOR READ FUNCTION  

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF. 
 void can_read()//CAN READ FUNCTION 
{ 
    CANCON = 0x0C;     //requests normal mode and receiver buffer 0/
    can_read_msg[0] = RXB0D0;          //Store the received Data Buffer 5 value to array/
    can_read_msg[1] = RXB0D1;            //Store the received Data Buffer 6 value to array/
    can_read_msg[2] = RXB0D2;          //Store the received Data Buffer 7 value to array/
    can_read_msg[3] = RXB0D3;   //Store the received Data Buffer 0 value to array/
    can_read_msg[4] = RXB0D4;      //Store the received Data Buffer 1 value to array/
    can_read_msg[5] = RXB0D5;   //Store the received Data Buffer 2 value to array/
    can_read_msg[6] = RXB0D6; //Store the received Data Buffer 3 value to array/
    can_read_msg[7] = RXB0D7; //Store the received Data Buffer 4 value to array/
    can_read_msg[8] = RXB0DLC;  //Store the received DLC value to array/
    can_read_msg[9] = RXB0SIDL;     //Store the received Standard Identifier  value to array/
    can_read_msg[10] = RXB0SIDH;     //Store the received Standard Identifier  value to array/
    RXB0CONbits.RXB0FUL = 0;       //Receive buffer is open to receive a new message/
    CANCON=0x00;
//    if(can_read_msg[3]==0X04 && can_read_msg[9]==0X00 && can_read_msg[10]==0X11 ) //COMPARING THE RECIEVED THE MESSAGE WITH GIVEN DATA 
//           { 
//             LATA = ~LATA;           
//            //EEPROM_write(0x21,0x00);
//            }
 //PORTDbits.RD7= ENABLE; // led on 
}
 
 
 
 
 void __interrupt() ISR1()
{   
      
      
    if( PIR3bits.RXB0IF)// CONTION CHECKING CANSTATUS REGISTER WITH RECIVE BUFFER INTERRUPT
    {
       
      //   PIR3bits.RXB0IF = 0; //clearing interrupt flag 
         can_read();// CAN READ FUNCTION CALL WHICH IS DECLARED IN HEADER FILE 
     if(can_read_msg[3]==0X04 && can_read_msg[9]==0X00 && can_read_msg[10]==0X11) //COMPARING THE RECIEVED THE MESSAGE WITH GIVEN DATA 
           { 
             LATAbits.LA0 = ~LATAbits.LA0;           
            //EEPROM_write(0x21,0x00);
            }
     else if(can_read_msg[1]==0X03 && can_read_msg[9]==0X00 && can_read_msg[10]==0X11) //COMPARING THE RECIEVED THE MESSAGE WITH GIVEN DATA 
           { 
             LATAbits.LA1 = ~LATAbits.LA1;           
            //EEPROM_write(0x21,0x00);
            }      
      
            PIR3bits.RXB0IF=0;
    }
              
  }  
void sys_init() //SYSTEM INIT FUNCTION
{
    INTCONbits.GIE=ENABLE; //ENABLING GLOBAL INTERRUPT
    INTCONbits.PEIE=ENABLE; //ENABLING PERIPHERAL INTERRUPT
}

void can_init() //CAN INIT FUNCTION
{
   TRISBbits.TRISB2=DISABLE ; //SET PORT B PIN 2 AS TRANSMITTER BIT;
   TRISBbits.TRISB3=ENABLE;  //SET PORT B PIN 3 AS RECIEVER BIT;
   PIE3bits.RXB0IE =ENABLE;  // PERIPHERAL INTERRUPT REGISTER ENABLING RECIERVE BUFFER0
   //PIE3bits.RXB1IE=ENABLE;  // PERIPHERAL INTERRUPT REGISTER ENABLING RECIERVE BUFFER1
   IPR3bits.RXB0IP =ENABLE;  // PERIPHERAL INTERRUPT PRIORITY ENABLING THE RECIVE BUFFER0 PRIORITY BIT
   //IPR3bits.RXB1IP=ENABLE;  // PERIPHERAL INTERRUPT PRIORITY ENABLING THE RECIVE BUFFER1 PRIORITY BIT
   CANCON|=0x80; //CANCON IS IN CONFIGURATION MODE;
    while(CANSTAT!=0x80) ;//CHECKING THE CONDITION IF CANCON==CONFIGURATION MODE 
              BRGCON1=0xC1; // 1100 0001  
            BRGCON2=0xAE; // 1010 1110   
            BRGCON3=0x45; // 0100 0101 
            CIOCON=0x20;  //CAN I/O CONTROL REGISTER ,CANTX pin will drive VDD(5v) when recessive bit
            RXB0CONbits.RXM0=1;
            RXB0CONbits.RXM1=1;
            CANCON=0x00;  // CANCON IS IN NORMAL MODE
    
}  

void tx_buffer0(void) /*function definition*/
{
/*given Standard data frame identifier is 0x701 */
TXB0SIDH = 0x11; /* Standard Identifier higher bits D10 to D3 8 bit occupies*/
TXB0SIDL = 0x00; /* Standard Identifier lower bits D2 TO D0 3 bit occupies*/
TXB0DLC =0x08; /* Data Length Code 8 bytes of length */
//TXB0D0 = 0x22; /* DataByte 0 */
TXB0D1 = 0x22; /* DataByte 1 */
//TXB0D2 = 0x22; /* DataByte 2 */
TXB0D3 = 0x22; /* DataByte 3 */
//TXB0D4 = 0x22; /* DataByte 4 */
TXB0D5 = 0x22; /* DataByte 5 */
//TXB0D6 = 0x22; /* DataByte 6 */
//TXB0D7 = 0x22; /* DataByte 7 */
//forcing the message to send from buffer to i/o pins/
TXB0CON = 0x08; /*Requests sending a message. Clears the TXABT, TXLARB and TXERR bits. Priority Level 0 (lowest priority */
}
void main() 
{
    TRISAbits.RA0=0;
    LATAbits.LA0=0; 
    TRISAbits.RA1=0;
    LATAbits.LA1=0;
    sys_init();  
    can_init();  
    RBPU=0;
    TRISBbits.RB0=1;
    while(1){
        if(PORTBbits.RB0==0){
            tx_buffer0();
            __delay_ms(1000);
        }
        
    }    
}


