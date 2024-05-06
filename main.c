
//C:\Users\HP\AppData\Local\Temp\VSM Studio\

//Include necessary header files
#include <pic.h>
#include <string.h>

//Define configuration bits for the microcontroller
__CONFIG( FOSC_HS & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_OFF & DEBUG_OFF);

#define _XTAL_FREQ 2000000

//Define constants for LCD display pins and keypad pins
#define rs RD2
#define en RD3

#define R1 RB0
#define R2 RB1
#define R3 RB2
#define R4 RB3
#define C1 RB4
#define C2 RB5
#define C3 RB6
#define C4 RB7
// Buzzer pin
#define BUZZER PORTAbits.RA4

// Define constants diodes pins
#define DIODE_BLOCKED PORTAbits.RA0
#define DIODE_DENIED PORTAbits.RA1
#define DIODE_GRANTED PORTAbits.RA2
#define DIODE_SECURITY PORTAbits.RA3

//Define functions for initializing the LCD display, sending commands to the LCD, displaying characters on the LCD, and providing delay for the LCD
void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();

//Define functions for controlling the buzzer and diodes
void buzzer(void);
void shutdown(void);
void diode_RED(void);
void diode_YELLOW(void);
void diode_GREEN(void);
void diode_BLUE(void);

//Define functions for reading the keypad and initializing the keypad
unsigned char key();
void keyinit();

//Define a function for implementing the 30-second countdown timer
void countdown();

//Define a 4x4 matrix for the keypad and variables to store the row and column numbers for the pressed key
unsigned char keypad[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','E'}};
unsigned char rowloc,colloc;

void main()
{
   int tr= 3; // Number of attempts allowed to enter the correct password
   unsigned int i;

   TRISD=0; // Set all pins of port D as output
   lcd_init(); // Initialize LCD display
   keyinit(); // Initialize keypad

   cmd(0x01); // Clear display
   cmd(0x80); // Set cursor to the first line and first position

   show((unsigned char*)" Enter the Key "); // Display "Enter the Key" message on LCD
   unsigned char password1[] = "1234"; // Set password 1
   unsigned char password2[] = "4567"; // Set password 2
   unsigned char password3[] = "2371"; // Set password 3
   unsigned char input_password[5] = {0}; // Initialize the input password array with all zeros
   unsigned char p[5] = {0}; // Initialize the password array with all zeros
   unsigned char index = 0; // Initialize the index variable
   unsigned char blocked = 0; // Initialize the blocked variable

   while(tr>0) // Loop until the number of attempts exceeds the allowed limit
   { 
      if (!blocked) // If not blocked
	  {
         cmd(0xc7); // Set cursor to the second line and fourth position
         input_password[index] =key(); // Get the key pressed by the user
	     p[index] ='*'; // Replace the key with an asterisk
         show(p); // Display the password with asterisks
         index++; // Increment the index variable
	  }
     
      if (index == 4) // If the password is complete
	  {  
         if (strcmp(input_password, password1) == 0) // If password 1 is entered correctly
		      {
             cmd(0x01); // Clear display
             show((unsigned char*)"Access Granted"); // Display "Access Granted" message
             cmd(0xc0); // Set cursor to the second line and first position
             show((unsigned char*)"Welcome MR1"); // Display "Welcome MR1" message
             diode_GREEN(); // Turn on green diode
             __delay_ms(8000); // Wait for 5 seconds
             index = 0; // Reset index to 0
             memset(input_password, 0, sizeof(input_password)); // Clear the input password array
             memset(p, 0, sizeof(p)); // Clear the password array
             break; // Exit the loop
		      } 
		      
		      else if (strcmp(input_password, password2) == 0) // If password 2 is entered correctly
		      {
             cmd(0x01); // Clear display
             show((unsigned char*)"Access Granted"); // Display "Access Granted" message
             cmd(0xc0); // Set cursor to the second line and first position
             show((unsigned char*)"Welcome MR2"); // Display "Welcome MR2" message
             diode_GREEN(); // Turn on green diode
             __delay_ms(8000); // Wait for 5 seconds
             index = 0; // Reset index to 0
             memset(input_password, 0, sizeof(input_password)); // Clear the input password array
             memset(p, 0, sizeof(p)); // Clear the password array
             break; // Exit the loop
		      } 
		      
		      
		      else if (strcmp(input_password, password3) == 0) // If password 3 is entered correctly
		      {
                     cmd(0x01);// Clear display
		      show((unsigned char*)"Access Granted");
			  cmd(0xc0);// Set cursor to the second line and first position
			 show((unsigned char*)"Securitiy ");
		       diode_BLUE();// Turn on blue diode
			 __delay_ms(8000); // Wait for 5 seconds
			     index = 0; // Reset index to 0
                      memset(input_password, 0, sizeof(input_password)); // Clear the input password array
                      memset(p, 0, sizeof(p)); // Clear the password array
                      break; // Exit the loop
		      }
		    else //password is entered incorrectly
		       {
			  
                    cmd(0x01);// Clear display
			  
                    show((unsigned char*)"Access Denied");// Display "Access Denied" message
			  
			  __delay_ms(8000); // Wait for 5 seconds
			    diode_YELLOW();// Turn on yellow diode
                          tr--; 
			      index = 0; // Reset index to 0
                      memset(input_password, 0, sizeof(input_password)); // Clear the input password array
                      memset(p, 0, sizeof(p)); // Clear the password array
			  if (tr==0) // If the user attempts to enter an incorrect password for three consecutive times
			     {
				  cmd(0x01);// Clear display
			  
                                   show((unsigned char*)"Access BLOCKED");// Display "Access BLOCKED" message
				     diode_RED();
				__delay_ms(10000); // Wait for 1s
				     blocked=1;// system is blocked
				countdown();  // wait for 30s
	       
	        cmd(0x01); // Clear display
            cmd(0x80);
     
    show((unsigned char*)" Security Key ");
	while(blocked){
	       cmd(0xc7);
        input_password[index] =key();
            p[index] ='*';
         show(p);
        index++;
		     if (index == 4) 
	  {
	                   if (strcmp(input_password, password3) == 0)
		      {
                     cmd(0x01);
		      show((unsigned char*)"Access Granted");
			  cmd(0xc0);
			 show((unsigned char*)"Securitiy ");
		       diode_BLUE();
			 __delay_ms(8000); // Wait for 5s
			 blocked=0;
			 tr=3;
			break;
		      }
		    else 
		       {
			  cmd(0x01);
		      show((unsigned char*)"SHUTDOWN");
			  shutdown();//system is shutting down in 3s
			  cmd(0x08);
                          cmd(0x00);

			  
	    }
	 }
      }
	 
				
				}
				else {
				   cmd(0x01); // Clear display
                                   cmd(0x80);
     
                                  show((unsigned char*)" Enter the Key ");
				   continue;
				}
			
			  
	     
	     }
	  }
       }
   
	    }
   	 


void lcd_init()
{
    cmd(0x02);
    cmd(0x28);
    cmd(0x0e);
    cmd(0x06);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    rs=0; 
    PORTD&=0x0F;
    PORTD|=(a&0xf0);
    en=1;
    lcd_delay();
    en=0;
    lcd_delay();
    PORTD&=0x0f;
    PORTD|=(a<<4&0xf0);
    en=1;
    lcd_delay();
    en=0;
    lcd_delay();
}

void dat(unsigned char b)
{
    rs=1; 
    PORTD&=0x0F;
    PORTD|=(b&0xf0);
    en=1;
    lcd_delay();
    en=0;
    lcd_delay();
    PORTD&=0x0f;
    PORTD|=(b<<4&0xf0);
    en=1;
    lcd_delay();
    en=0;
    lcd_delay();
}

void show(unsigned char *s)
{
    while(*s) {
        dat(*s++);
    }
}

void lcd_delay()
{
    unsigned int lcd_delay;
    for(lcd_delay=0;lcd_delay<=1000;lcd_delay++);
}

void keyinit()
{
    TRISB=0XF0;
    OPTION_REG&=0X7F;           //ENABLE PULL UP
}

unsigned char key()
{
    PORTB=0X00;
    while(C1&&C2&&C3&&C4);
    while(!C1||!C2||!C3||!C4) {
        R1=0;
        R2=R3=R4=1;
        if(!C1||!C2||!C3||!C4) {
            rowloc=0;
            break;
        }
        R2=0;R1=1;
        if(!C1||!C2||!C3||!C4) {
            rowloc=1;
            break;
        }
        R3=0;R2=1;
        if(!C1||!C2||!C3||!C4) {
            rowloc=2;
            break;
        }
        R4=0; R3=1;
        if(!C1||!C2||!C3||!C4){
            rowloc=3;
            break;
        }
    }
    if(C1==0&&C2!=0&&C3!=0&&C4!=0)
            colloc=0;
    else if(C1!=0&&C2==0&&C3!=0&&C4!=0)
            colloc=1;
    else if(C1!=0&&C2!=0&&C3==0&&C4!=0)
            colloc=2;
    else if(C1!=0&&C2!=0&&C3!=0&&C4==0)
            colloc=3;
    while(C1==0||C2==0||C3==0||C4==0);
    return (keypad[rowloc][colloc]);
}
    




void buzzer(void)
{
      TRISAbits.TRISA4 = 0; // Set Buzzer Pin as Output
       PORTAbits.RA4 = 1; // Buzzer ON
       __delay_ms(8000); // Wait for 1s
       PORTAbits.RA4 = 0; // Buzzer OFF
   
}


void diode_RED(void)
{
   for(int i=3;i>0;i--)
   {

 TRISAbits.TRISA0 = 0; // Set LED Pin as Output
PORTAbits.RA0 = 1; // LED ON
__delay_ms(8000); // Wait for 1s

 PORTAbits.RA0 = 0; // LED OFF
      __delay_ms(8000); // Wait for 1s
   }
   
}
void diode_YELLOW(void)
{
   for(int i=3;i>0;i--)
   {

 TRISAbits.TRISA1 = 0; // Set LED Pin as Output
PORTAbits.RA1 = 1; // LED ON
__delay_ms(8000); // Wait for 1s

 PORTAbits.RA1 = 0; // LED OFF
      __delay_ms(8000); // Wait for 1s
   }
   
}
void diode_GREEN(void)
{

 TRISAbits.TRISA2 = 0; // Set LED Pin as Output
PORTAbits.RA2 = 1; // LED ON
__delay_ms(100000); // Wait for 5s

 PORTAbits.RA2 = 0; // LED OFF
   
}
void diode_BLUE(void)
{


 TRISAbits.TRISA3 = 0; // Set LED Pin as Output
PORTAbits.RA3 = 1; // LED ON
__delay_ms(100000); // Wait for 5s

 PORTAbits.RA3 = 0; // LED OFF
        TRISAbits.TRISA5 = 0; // Set LED Pin as Output
PORTAbits.RA5 = 1; // LED ON
   
}
void shutdown(void)
{
  
 TRISAbits.TRISA0 = 0; // Set LED Pin as Output
PORTAbits.RA0 = 1; // LED ON
__delay_ms(8000); // Wait for 50ms
 PORTAbits.RA0 = 0; // LED OFF
      __delay_ms(8000); // Wait for 1s
    
 TRISAbits.TRISA1 = 0; // Set LED Pin as Output
PORTAbits.RA1 = 1; // LED ON
__delay_ms(8000); // Wait for 1s
 PORTAbits.RA1 = 0; // LED OFF
      __delay_ms(8000); // Wait for 1s
    
 TRISAbits.TRISA2 = 0; // Set LED Pin as Output
PORTAbits.RA2 = 1; // LED ON
__delay_ms(8000); // Wait for 1s
 PORTAbits.RA2 = 0; // LED OFF
      __delay_ms(8000); // Wait for 1s
    
 TRISAbits.TRISA3 = 0; // Set LED Pin as Output
PORTAbits.RA3 = 1; // LED ON
__delay_ms(8000); // Wait for 1s
 PORTAbits.RA3 = 0; // LED OFF
      __delay_ms(20000); // Wait for 1s
      cmd(0x08);
       cmd(0x00);

 
   }
void countdown()
{
  
    for(int i=30; i>0; i--)
    {
         cmd(0x01);
                show((unsigned char*)"Try again in 30s");
                for (i = 30; i > 0; i--)
                {
                    cmd(0xc5);
                    dat((i / 10) + '0');
                    dat((i % 10) + '0');
                    __delay_ms(9200);
                }
    }
   
}
 
 