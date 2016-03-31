// LCD  encoder

	// Librerias
#include <LiquidCrystal.h>			// libreria para LCD - ttps://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home	
#include <Wire.h>
#include <string.h>
#include <Encoder.h>				// libreria encoder - http://www.pjrc.com/teensy/td_libs_Encoder.html		

	// Encoder
byte  	Enc_Center			= 8;	// pin
byte 	Enc_CLK				= 7;	// pin
byte 	Enc_Data 			= 6;	// pin
Encoder myEnc				(Enc_CLK, Enc_Data);	// conexion de encoder

	// Cursor
int  Cursor_Index_Pos 		= 1;	// posicion en tiempo real de lcd
byte Cursor_Index[20][4] 	= 
						{	{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, },
  							{0, 0, 0, 0, }
						};

	// LCD
byte LCD_RS 				= 43;	// pin
byte LCD_E  				= 45;	// pin
byte LCD_D4 				= 47;	// pin
byte LCD_D5 				= 49;	// pin
byte LCD_D6 				= 51;	// pin
byte LCD_D7					= 53;	// pin
byte Back_Light_PWM			= 13;	// salida para PWM de Back Light de LCD
byte Contrast_PWM			= 12;	// salida para pwm de contraste de LCD
LiquidCrystal lcd			(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);  //LCD setup

void setup()
{
		// Encoder
	pinMode(Enc_Center,				INPUT_PULLUP);
	// no declarar CLK ni Data

		// LCD
	pinMode(LCD_RS, 				OUTPUT);
	pinMode(LCD_E,  				OUTPUT);
	pinMode(LCD_D7, 				OUTPUT);
	pinMode(LCD_D6, 				OUTPUT);
	pinMode(LCD_D5, 				OUTPUT);
	pinMode(LCD_D4, 				OUTPUT);
	pinMode(Back_Light_PWM,			OUTPUT);
	pinMode(Contrast_PWM,			OUTPUT);
	lcd.begin(20, 4);							//tamaño de LCD
	analogWrite(Back_Light_PWM, 	255);
	analogWrite(Contrast_PWM, 		150);

	Serial.begin(9600);
}

void loop()
{
	menu();
}

void menu()
{
	// menu interactivo
		// texto en lcd
	lcd.clear();
	lcd.setCursor (1, 0);
	lcd.print(1);
	lcd.setCursor (1, 1);
	lcd.print(2);
	lcd.setCursor (1, 2);
	lcd.print(3);
	lcd.setCursor (1, 3);
	lcd.print(4);
	lcd.setCursor (7, 0);
	lcd.print(5);
	lcd.setCursor (7, 1);
	lcd.print(6);
	lcd.setCursor (7, 2);
	lcd.print(7);
	lcd.setCursor (7, 3);
	lcd.print(8);
	lcd.setCursor (13, 0);
	lcd.print(9);
	lcd.setCursor (13, 1);
	lcd.print(10);
	lcd.setCursor (13, 2);
	lcd.print(11);
	lcd.setCursor (13, 3);
	lcd.print(12);
	lcd.setCursor (18, 0);
	lcd.print(13);
	lcd.setCursor (18, 1);
	lcd.print(14);
	lcd.setCursor (18, 2);
	lcd.print(15);
	lcd.setCursor (18, 3);
	lcd.print(16);
		// borrar datos previos en el indice
	Cursor_Index_Clear();
		// establecer el indice
	Cursor_Index[0][0] 	= 1;		// y x
	Cursor_Index[0][1] 	= 2;
	Cursor_Index[0][2] 	= 3;
	Cursor_Index[0][3] 	= 4;
	Cursor_Index[6][0] 	= 5;
	Cursor_Index[6][1] 	= 6;
	Cursor_Index[6][2] 	= 7;
	Cursor_Index[6][3] 	= 8;
	Cursor_Index[12][0] = 9;
	Cursor_Index[12][1] = 10;
	Cursor_Index[12][2] = 11;
	Cursor_Index[12][3] = 12;
	Cursor_Index[17][0] = 13;
	Cursor_Index[17][1] = 14;
	Cursor_Index[17][2] = 15;
	Cursor_Index[17][3] = 16;
		// establecer posicion inicial del cursor
	Cursor_Index_Pos = 1;
		// iniciar navegacion y evaluar el index seleccionado
	Navegar();
	Serial.println(Cursor_Index_Pos);
	while(1);
	
}

void Navegar()
{
	byte Index_Final = 0;	// saber cual es el ultimo indice

		// buscar donde poner el cursor y ultimo indice
	for (byte Conteo_Y = 0; Conteo_Y <= 19; Conteo_Y ++)
	{
		for (byte Conteo_X = 0; Conteo_X <= 3; Conteo_X ++)
		{
				// posicionar cursor
			if (Cursor_Index[Conteo_Y][Conteo_X] == Cursor_Index_Pos)
			{
				lcd.setCursor (Conteo_Y, Conteo_X);
				lcd.print(">");
			}
				// saber ultimo indice
			if (Cursor_Index[Conteo_Y][Conteo_X] > Index_Final)
			{
				Index_Final = Cursor_Index[Conteo_Y][Conteo_X];
			}
		}
	} 

	long Enc_ant 				= myEnc.read() / 4;
	long Enc_read				= 0;
	int  Cursor_Index_Pos_ant 	= 0;
	byte giro 					= 0;

		// Lectura de encoder
	while(digitalRead(Enc_Center) == 1)
	{
		Enc_read = myEnc.read() / 4;
		Cursor_Index_Pos_ant = Cursor_Index_Pos;
		giro = 0;

			// izquierda
		if (Enc_read < Enc_ant)
		{
			Enc_ant = Enc_read;
				// restar Cursor_Index_Pos
			Cursor_Index_Pos = Cursor_Index_Pos - 1;
				// limites
			if(Cursor_Index_Pos < 1)
			{
				Cursor_Index_Pos = Index_Final;
			}
			giro = 1;
		}
			// derecha
		if (Enc_read > Enc_ant)		
		{
			Enc_ant = Enc_read;
				// sumar Cursor_Index_Pos
			Cursor_Index_Pos = Cursor_Index_Pos + 1;
					// limites
			if(Cursor_Index_Pos > Index_Final)
			{
				Cursor_Index_Pos = 1;
			}
			giro = 1;
		}

			// si se giro...
		if (giro == 1)
		{
			// posicionar cursor
			for (byte Conteo_Y = 0; Conteo_Y <= 19; Conteo_Y ++)
			{
				for (byte Conteo_X = 0; Conteo_X <= 3; Conteo_X ++)
				{
						// posicionar cursor
					if (Cursor_Index[Conteo_Y][Conteo_X] == Cursor_Index_Pos)
					{
						lcd.setCursor (Conteo_Y, Conteo_X);
						lcd.print(">");
					}
						// borrar cursor anterior
					if (Cursor_Index[Conteo_Y][Conteo_X] == Cursor_Index_Pos_ant)
					{
						lcd.setCursor (Conteo_Y, Conteo_X);
						lcd.print(" ");
					}
				}
			}
		}
	}
}

void Cursor_Index_Clear()
{
	// borrar el indice
	for (byte Conteo_Y = 0; Conteo_Y <= 19; Conteo_Y ++)
	{
		for (byte Conteo_X = 0; Conteo_X <= 3; Conteo_X ++)
		{
			Cursor_Index[Conteo_Y][Conteo_X] = 0;
		}
	}
}




