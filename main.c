#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT PORTC //LCD data port
#define LCD_DDR DDRC //LCD data direction register
#define LCD_PIN PINC //LCD  input pins
#define LCD_RS 0 //LCD register select pin number
#define LCD_RW 1 //LCD read write pin number
#define LCD_EN 2 //LCD enable pin number

#define KEY_PORT PORTB //keypad port
#define KEY_DDR DDRB  //keypad data direction register
#define KEY_PIN PINB //keypad input pins

void lcd_command(unsigned char cmnd) //configure the LCD to send command
{
	LCD_PORT = (LCD_PORT & 0x0f) | (cmnd & 0xf0); //taking the higher four bits of the command
	LCD_PORT &= ~(1<<LCD_RS); //command mode
	LCD_PORT &= ~(1<<LCD_RW); //write operation
	LCD_PORT |= (1<<LCD_EN); //toggle enable first step
	_delay_us(1); //increase enable length
	LCD_PORT &= ~(1<<LCD_EN); //toggle enable second step
	_delay_us(100);
	
	LCD_PORT = (LCD_PORT & 0x0f) | (cmnd<<4  & 0xf0); //taking the lower four bits of the command
	LCD_PORT |= (1<<LCD_EN); //toggle enable first step
	_delay_us(1); //increase enable length
	LCD_PORT &= ~(1<<LCD_EN); //toggle enable second step the command will be transmitted now
	_delay_ms(3);
}

void lcd_data(unsigned char data)
{
	LCD_PORT = (LCD_PORT & 0x0f) | (data & 0xf0); //taking the higher four bits of the data
	LCD_PORT |= (1<<LCD_RS); //command mode
	LCD_PORT &= ~(1<<LCD_RW); //write operation
	LCD_PORT |= (1<<LCD_EN); //toggle enable first step
	_delay_us(1); //increase enable length
	LCD_PORT &= ~(1<<LCD_EN); //toggle enable second step
	_delay_us(100);
	
	LCD_PORT = (LCD_PORT & 0x0f) | (data<<4 & 0xf0); //taking the lower four bits of the data
	LCD_PORT |= (1<<LCD_EN); //toggle enable first step
	_delay_us(1); //increase enable length
	LCD_PORT &= ~(1<<LCD_EN); //toggle enable second step the data will be transmitted now
	_delay_ms(3);
}

void lcd_init(void)
{
	LCD_DDR = 0xff; //make the PORT as output
	_delay_ms(20);
	
	lcd_command(0x33); //4 bit mode
	_delay_us(100);
	
	lcd_command(0x32); //according to 4 bit mode
	_delay_us(100);
	
	lcd_command(0x28); // 2 lines 5x7 matrix
	_delay_us(100);
	
	lcd_command(0x0e); //cursor blinking
	_delay_us(100);
	
	lcd_command(0x06); //increment cursor
	_delay_us(2000);
	
	lcd_command(0x01); //clear screen
	_delay_us(100);
}

void lcd_goto_xy(unsigned char x, unsigned char y) //initializing the beginning of the line to be written on
{
	unsigned char first_char_address[] = {0x80, 0xc0, 0x94, 0xd4}; //array for the first indices of the lines
	lcd_command(first_char_address[y-1]+x-1); //prepare the suitable command
	_delay_us(100);
}

void lcd_print(char* str) //send the string to the LCD
{
	unsigned char i = 0; //index for looping
	while(str[i]!=0) //looping across the string
	{
		lcd_data(str[i]); //send individual character
		i++; //point to next character
	}
}

unsigned char keypad_read(void) //keypad reading character
{
	unsigned char keypad[4][4] = {{'0', '1', '2', '3'},
	{'4', '5', '6', '7'},
	{'8', '9', 'A', 'B'},
	{'C', 'D', 'E', 'F'}};
	unsigned char colloc, rowloc; //column location & row location
	KEY_DDR = 0xf0; //make the port half input & half output
	KEY_PORT = 0xff; //make the port output
	unsigned char temp, return_char; //temp represent temporary variable and return_char represent the return of the function
	
	while (1) //checking if any key pressed & the DEBOUNCING effect + encode the character
	{
		do
		{
			KEY_PORT &= 0x0f;
			colloc = (KEY_PIN & 0x0f);
		} while (colloc != 0x0f);
		
		do
		{
			do
			{
				_delay_ms(20);
				colloc = (KEY_PIN & 0x0f);
			} while (colloc == 0x0f);
			
			_delay_ms(20);
			colloc = (KEY_PIN & 0x0f);
		} while (colloc == 0x0f);
		
		while(1)
		{
			KEY_PORT = 0xef;
			colloc = (KEY_PIN & 0x0f);
			if(colloc != 0x0f)
			{
				rowloc = 0;
				break;
			}
			
			KEY_PORT = 0xdf;
			colloc = (KEY_PIN & 0x0f);
			if(colloc != 0x0f)
			{
				rowloc = 1;
				break;
			}
			
			KEY_PORT = 0xbf;
			colloc = (KEY_PIN & 0x0f);
			if(colloc != 0x0f)
			{
				rowloc = 2;
				break;
			}
			
			KEY_PORT = 0x7f;
			colloc = (KEY_PIN & 0x0f);
			rowloc = 3;
			break;
		}
		
		if(colloc == 0x0e)
		temp = (keypad[rowloc][0]);
		else if(colloc == 0x0d)
		temp = (keypad[rowloc][1]);
		else if(colloc == 0x0b)
		temp = (keypad[rowloc][2]);
		else
		temp = (keypad[rowloc][3]);
		
		if(temp == 70) // encode 1 is pressed
		{
			return_char = 1;
			break;
		}
		
		if(temp == 69) // encode 2 is pressed
		{
			return_char = 2;
			break;
		}
		
		if(temp == 68) // encode 3 is pressed
		{
			return_char = 3;
			break;
		}
		
		if(temp == 67) // encode 4 is pressed
		{
			return_char = 4;
			break;
		}
		
		if(temp == 66) // encode 5 is pressed
		{
			return_char = 5;
			break;
		}
		
		if(temp == 65) // encode 6 is pressed
		{
			return_char = 6;
			break;
		}
		
		if(temp == 57) // encode 7 is pressed
		{
			return_char = 7;
			break;
		}
		
		if(temp == 56) // encode 8 is pressed
		{
			return_char = 8;
			break;
		}
		
		if(temp == 55) // encode 9 is pressed
		{
			return_char = 9;
			break;
		}
		
		if(temp == 54) // encode 0 is pressed
		{
			return_char = 0;
			break;
		}
		
		if(temp == 53) // encode 1 is pressed
		{
			return_char = 1;
			break;
		}
		
		if(temp == 52) // encode 2 is pressed
		{
			return_char = 2;
			break;
		}
		
		if(temp == 51) // encode 3 is pressed
		{
			return_char = 3;
			break;
		}
		
		if(temp == 50) // encode 4 is pressed
		{
			return_char = 4;
			break;
		}
		
		if(temp == 49) // encode 5 is pressed
		{
			return_char = 5;
			break;
		}
		
		if(temp == 48) // encode 6 is pressed
		{
			return_char = 6;
			break;
		}
	}
	return return_char;
}

void usart_initialization(void) //USART configurations
{
	UCSRB = (1<<TXEN); //enable transmitting
	UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL); //8 bits, no parity, one stop bit
	UBRRL = 0x33; //baud rate 9600
}

void usart_send_char(unsigned char character) //USART character sending function
{
	while((UCSRA&(1<<UDRE))==0); //keep polling if the USART data register is empty or not
	UDR = character; //put the character in the USART data register
}

void usart_send_string(char *string) //USART string sending function
{
	unsigned char pointer_to_chars = 0; //make pointer to string characters to loop across the
	while(string[pointer_to_chars]!=0) //check if the null terminator reached or not
	{
		usart_send_char(string[pointer_to_chars]); //send the character
		pointer_to_chars++; //increment the pointer to point to the next character
	}
}

void adc_initialization(void) //ADC configurations
{
	DDRA = 0x00; //make PORTA as input
	ADCSRA = 0x87; //enable ADC, pre_scalar = 128
	ADMUX = 0xe0; //reference voltage = 2.56, left adjustment enabled, channel zero
}

unsigned char first_digit(unsigned char temp) //change the first digit to string
{
	return (temp/10)+0x30;
}

unsigned char second_digit(unsigned char temp) //change the second digit to string
{
	return (temp%10)+0x30;
}

int main(void)
{
	//declaring the variables that we shall deal with
	unsigned char temperature, lower_limit, upper_limit, current_temperature, previous_temperature, temperature_rate, l1, l2, u1, u2, t1, t2;
	int first_attempt = 0; //this is for checking if it is the first time we start the system
	
	usart_initialization(); //call USART configuration function
	adc_initialization(); //call ADC configuration function
	
	//user input
	lcd_init(); //initialization of the LCD
	lcd_goto_xy(1, 1); //go to the first line
	lcd_print("The lower level"); //print the statement on the first line
	lcd_goto_xy(1, 2); //go to the second line
	lcd_print("is : "); //print the statement on the second line
	
	l1 = keypad_read();
	lcd_data(l1+0x30); //change to ASCII and send
	
	l2 = keypad_read();
	lcd_data(l2+0x30); //change to ASCII and send
	_delay_ms(1000);
	
	lcd_init(); //initialization of the LCD
	lcd_goto_xy(1, 1); //go to the first line
	lcd_print("The upper level"); //print the statement on the first line
	lcd_goto_xy(1, 2); //go to the second line
	lcd_print("is : "); //print the statement on the second line
	
	u1 = keypad_read();
	lcd_data(u1+0x30); //change to ASCII and send
	
	u2 = keypad_read();
	lcd_data(u2+0x30); //change to ASCII and send
	_delay_ms(1000);
	
	lcd_init(); //initialization of the LCD
	lcd_goto_xy(1, 1); //go to the first line
	lcd_print("The rate"); //print the statement on the first line
	lcd_goto_xy(1, 2); //go to the second line
	lcd_print("is : "); //print the statement on the second line
	
	t1 = keypad_read();
	lcd_data(t1+0x30); //change to ASCII and send
	
	t2 = keypad_read();
	lcd_data(t2+0x30); //change to ASCII and send
	_delay_ms(1000);
	
	lower_limit = ((l1*10)+l2);
	upper_limit = ((u1*10)+u2);
	temperature_rate = ((t1*10)+t2);
	
	
	/////// precaution print
	lcd_init();
	lcd_goto_xy(1, 1);
	lcd_print("T = ");
	lcd_goto_xy(5, 1);
	///////
	
	while (1)
	{
		ADCSRA |= (1<<ADSC); //start conversion
		while((ADCSRA&(1<<ADIF))==0); //keep polling if the ADC is not finished
		temperature = ADCH; //read the sensor temperature
		
		// initial print 
		lcd_init();
		lcd_goto_xy(1, 1);
		lcd_print("T = ");
		lcd_goto_xy(5, 1);
		unsigned char temper;
		temper = temperature;
		lcd_command(0xC);
		lcd_data(first_digit(temper));
		lcd_goto_xy(6, 1);
		lcd_data(second_digit(temper));
		lcd_goto_xy(5, 1);
		
		if(first_attempt == 0) //initially if it is the first attempt then the current and previous temperature are the same and equal the current temperature
		{
			previous_temperature = temperature;
			current_temperature = temperature;
			first_attempt++;
		}
		
		if((temperature > upper_limit) || (temperature < lower_limit)) //check if the temperature in the range
		{
			//print on LCD + sending using GSM module
			lcd_goto_xy(1, 2);
			lcd_print("range ex."); 
			usart_send_string("AT+CMGS=\"0990864955\"");
			usart_send_char(0x0d); //enter code
			usart_send_string("The temperature is out of range, ");
			usart_send_string("Temperature = ");
			usart_send_char(first_digit(temperature));			
			usart_send_char(second_digit(temperature));
			usart_send_char(0x1a); //ctrl - z
			usart_send_char(0x0d);
			_delay_ms(1000);
		}
		
		//make change to current and previous temperature
		previous_temperature = current_temperature;
		current_temperature = temperature;
		
		_delay_ms(1000);
		
		//check if the rate of change exceed the normal rate
		if(((previous_temperature-current_temperature) > temperature_rate) || ((current_temperature-previous_temperature) > temperature_rate))
		{
			//print on LCD + sending using GSM module
			lcd_goto_xy(10, 2);
			lcd_print("rate ex.");
			usart_send_string("AT+CMGS=\"0990864955\"");
			usart_send_char(0x0d);
			usart_send_string("The temperature rate exceed level");
			usart_send_char(0x1a);
			usart_send_char(0x0d);
			_delay_ms(5000);
		}
	}
}

