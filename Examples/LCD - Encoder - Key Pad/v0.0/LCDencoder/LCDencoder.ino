// LCD  encoder

// Arduino mega
// probado en Arduino Arduino DMX-512 Tester Controller	hardv 0.7

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

// tecleado					
byte Keypad_A				= 30; 	
byte Keypad_B				= 32; 	
byte Keypad_C				= 34; 	
byte Keypad_D				= 36; 	
byte Keypad_1				= 38; 	
byte Keypad_2				= 40; 
byte Keypad_3				= 42;
byte Keypad_4				= 44;

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


int valor 					= 100;	// valor que cambia ---------------------------------

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

	// Teclado
	pinMode		(Keypad_A, OUTPUT);
	pinMode		(Keypad_B, OUTPUT);
	pinMode		(Keypad_C, OUTPUT);
	pinMode		(Keypad_D, OUTPUT);
	pinMode		(Keypad_1, INPUT_PULLUP);
	pinMode		(Keypad_2, INPUT_PULLUP);
	pinMode		(Keypad_3, INPUT_PULLUP);
	pinMode		(Keypad_4, INPUT_PULLUP);
}

void loop()
{
	menu();
}

void menu()
{
	// ------------------------------------------------------------------------------------------------------------------
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

	navegacion:

		// iniciar navegacion y evaluar el index seleccionado
	Navegar();	// actualiza Cursor_Index_Pos

	int valor_nuevo = 0;

	switch (Cursor_Index_Pos)
	{
		case 1:
			valor_nuevo = Numerico_Write(0, 255, 1, 0, 1, valor);

				// menor o igual al limites
			if (valor_nuevo <= 255)			// poner limite max
			{
				analogWrite(Back_Light_PWM, valor_nuevo);
				valor = valor_nuevo;
			}

				// mayor al limite
			if (valor_nuevo > 255)			// poner limite max
			{
				while(1)
				{
					valor_nuevo = Numerico_Enc_Write(0, 255, 1, 0, 1, valor);
					
					if (valor_nuevo > 255)	// poner limite max
					{
						break; // enter
					}

					analogWrite(Back_Light_PWM, valor_nuevo);	// accion
					valor = valor_nuevo;
		
				}
					// acomodar numero 	
				Numerico_Print(1, 0, valor, 255, 1);	// poner max 	// Numerico_Print(byte LCD_x, byte LCD_y, int valor, int max, byte Dec_Hex)
			}

			break;

		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
		case 7:

			break;
		case 8:

			break;
		case 9:

			break;
		case 10:

			break;
		case 11:

			break;
		case 12:

			break;
		case 13:

			break;
		case 14:

			break;
		case 15:

			break;
		case 16:

			break;
	}

	goto navegacion;
// ---------------------------------------------------------------------------------------------------------------------------------
}

void Numerico_Print(byte LCD_x, byte LCD_y, int valor, int max, byte Dec_Hex)
{
	// valor: numero a acomodar
	// max: numero maximo, para el espaciado
	// Dec_Hex: Dec 1, Hex 2
	// LCD_x: cursor
	// LCD_y: cursor

	// calcular tamaño

	lcd.noBlink();							// no parpadear cursor
	lcd.setCursor(LCD_x, LCD_y);

		// Dec
	if (Dec_Hex == 1)				
	{
		if (max < 10)						// 1 cifra
		{
			lcd.print(valor);
			goto salir;
		}
		if (max > 9 && max < 100)			// 2 cifras
		{
			if (valor < 10)
			{
				lcd.print(0);
				lcd.print(valor);
			}
			else
			{
				lcd.print(valor);
			}
			goto salir;
		}
		if (max > 99 && max < 1000)			// 3 cifras
		{
			if (valor < 10)
			{
				lcd.print("00");
				lcd.print(valor);
			}
			if (valor > 9 && valor < 100)
			{
				lcd.print("0");
				lcd.print(valor);
			}
			if (valor > 99)
			{
				lcd.print(valor);
			}
			goto salir;
		}
		if (max > 999)						// 4 cifras
		{
			if (valor < 10)
			{
				lcd.print("000");
				lcd.print(valor);
			}
			if (valor > 9 && valor < 100)
			{
				lcd.print("00");
				lcd.print(valor);
			}
			if (valor > 99)
			{
				lcd.print("0");
				lcd.print(valor);
			}
			if (valor > 999)
			{
				lcd.print(valor);
			}
			goto salir;
		}
	}
		// hexadecimal
	if (Dec_Hex == 2)				
	{
		if (valor < 16)
		{
			lcd.print("0");
			lcd.print(valor, HEX);
			goto salir;
		}
		if (valor > 15)
		{
			lcd.print(valor, HEX);
		}
	}

	salir: {}
}

void Navegar()
{
	// actualiza Cursor_Index_Pos

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
	delay(300); // rebote de centro encoder
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

int  Numerico_Enc_Write(int min, int max, byte LCD_x, byte LCD_y, byte Dec_Hex, long num_ant)
{
	// regresa el numero generado por el encoder
	// numero minimo a escribir - numero maximo a escribir - x y de pantalla donde se escribe
	// num_ant, el numero que estaba impreso
	// Dec_Hex decimal 1, hexadecimal 2
	// enter: encoder centro
	// hexadecimal max 2 digitos
	// si regresa numero max + 1 se presiono enter en encoder

	long Enc_ant 		= myEnc.read() / 4;
	long Enc_read		= 0;
	int  numero_lenght 	= 0;
	int  Enc_Real_Time  = 0;

	Enc_Real_Time 		= num_ant;

	lcd.setCursor(LCD_x, LCD_y);
	lcd.blink();					// parpadear cursor
	lcd.setCursor(LCD_x, LCD_y);
	lcd.print(num_ant);
	lcd.setCursor(LCD_x, LCD_y);
	
		// esperar giro
	while(1)
	{
		Enc_read = myEnc.read() / 4;
			
		// incrementar
		if (Enc_read > Enc_ant)	
		{
			Enc_Real_Time = Enc_Real_Time + 1;

			// limites
			if (Enc_Real_Time > max)
			{
				Enc_Real_Time = min;
				goto calculo;
			}
			goto calculo;
		}

			// decrementar
		if (Enc_read < Enc_ant)	
		{
			Enc_Real_Time = Enc_Real_Time - 1;

				// limites
			if (Enc_Real_Time < min)
			{
				Enc_Real_Time = max;
				goto calculo;
			}
			goto calculo;
		}

		if (digitalRead(Enc_Center) == 0)
		{
			delay(300);		// rebote
			
			return max + 1;
		}
	}
	
	calculo:

	// calcular tamaños maximos del numero

		// decimal
	if (Dec_Hex == 1)				
	{
		if (max < 10)
		{
			numero_lenght = 1;
		}
		if (max > 9)
		{
			numero_lenght = 2;
		}
		if (max > 99)
		{
			numero_lenght = 3;
		}
		if (max > 999)
		{
			numero_lenght = 4;
		}
	}
		// hexadecimal
	if (Dec_Hex == 2)				
	{
		if (max < 16)
		{
			numero_lenght = 1;
		}
		if (max > 15)
		{
			numero_lenght = 2;
		}
	}
	
	// borrar numeros anteriores

	lcd.setCursor(LCD_x, LCD_y);

	if (numero_lenght == 2)
	{
		lcd.print("  ");
	}
	if (numero_lenght == 3)
	{
		lcd.print("   ");
	}
	if (numero_lenght == 4)
	{
		lcd.print("    ");
	}

	// dibujar numero
	
	lcd.setCursor(LCD_x, LCD_y);
		// dec
	if (Dec_Hex == 1)
	{
		lcd.print(Enc_Real_Time);
	}
		// hex
	if (Dec_Hex == 2)
	{
		lcd.print(Enc_Real_Time, HEX);
	}

	return Enc_Real_Time;
}

int  Numerico_Write(int min, int max, byte LCD_x, byte LCD_y, byte Dec_Hex, int num_ant)
{
	// regresa el numero tecleado
	// numero minimo a escribir - numero maximo a escribir - x y de pantalla donde se escribe
	// el calculo del tamaño del numero lo hace en automatico
	// num_ant, el numero que estaba impreso
	// Dec_Hex decimal 1, hexadecimal 2
	// enter: # y encoder centro
	// cancelar y poner numero default: *
	// hexadecimal max 2 digitos
	// entrada desde encoder: D
	// si regresa numero max + 1 se presiono D para encoder
	
	int 	numero_lenght 		= 0;
	int 	retardo_lecturas 	= 300;
	byte 	numero				= 0;
	byte 	numero_1			= 0;
	byte	numero_2			= 0;
	byte	numero_3			= 0;
	byte	numero_4			= 0;
	byte 	lectura 			= 0;
	byte 	numero_unidades		= 0;
	
	lcd.blink();	// parpadear cursor
	
	// calcular tamaño de numero de espacios que ve a utilizar el numero maximo
		// decimal
	if (Dec_Hex == 1)				
	{
		if (max < 10)
		{
			numero_lenght = 1;
		}
		if (max > 9)
		{
			numero_lenght = 2;
		}
		if (max > 99)
		{
			numero_lenght = 3;
		}
		if (max > 999)
		{
			numero_lenght = 4;
		}
	}
		// hexadecimal
	if (Dec_Hex == 2)				
	{
		if (max < 16)
		{
			numero_lenght = 1;
		}
		if (max > 15)
		{
			numero_lenght = 2;
		}
	}
	
		// LCD posicionar cursor	
	lcd.setCursor(LCD_x, LCD_y);
	if (numero_lenght == 1)
	{
		lcd.print("_");
		lcd.setCursor(LCD_x, LCD_y);
	}
	if (numero_lenght == 2)
	{
		lcd.print("__");
		lcd.setCursor(LCD_x + 1, LCD_y);
	}
	if (numero_lenght == 3)
	{
		lcd.print("___");
		lcd.setCursor(LCD_x + 2, LCD_y);
	}
	if (numero_lenght == 4)
	{
		lcd.print("____");
		lcd.setCursor(LCD_x + 3, LCD_y);
	}
	
dibujar_numero_1:

	lectura = Numerico_Key();
		
		// lectura desde encoder
	if (lectura == 13)	
	{
		return max + 1;
	}

		// reglas decimal
	if (Dec_Hex == 1)
	{
		if (lectura == 16 || lectura == 14 || lectura == 15)	// enter, sin numero
		{
			goto numero_ant;
		}
		if (lectura > 9 && lectura < 14)	// tecla no valida
		{
			goto dibujar_numero_1;
		}
	}
		// reglas hexadecimal
	if (Dec_Hex == 2)
	{
		if (lectura == 16)					// enter, sin numero
		{
			goto numero_ant;
		}
	}
	
	// dibujar numero
	lcd.setCursor(LCD_x, LCD_y);
		// decimal
	if (Dec_Hex == 1)
	{
		if (numero_lenght == 1)
		{
			lcd.print(lectura);
			numero_1 = lectura;
			goto salida;
		}
		if (numero_lenght == 2)
		{
			lcd.setCursor(LCD_x + 1, LCD_y);
			lcd.print(lectura);
			lcd.setCursor(LCD_x + 1, LCD_y);
		}
		if (numero_lenght == 3)
		{
			lcd.setCursor(LCD_x + 2, LCD_y);
			lcd.print(lectura);
			lcd.setCursor(LCD_x + 2, LCD_y);
		}
		if (numero_lenght == 4)
		{
			lcd.setCursor(LCD_x + 3, LCD_y);
			lcd.print(lectura);
			lcd.setCursor(LCD_x + 3, LCD_y);
		}
	}
		// hexadecimal
	if (Dec_Hex == 2)
	{
		if (numero_lenght == 1)
		{
			lcd.print(lectura, HEX);
			numero_1 = lectura;
			goto salida;
		}
		if (numero_lenght ==  2)
		{
			lcd.setCursor(LCD_x + 1, LCD_y);
			lcd.print(lectura, HEX);
			lcd.setCursor(LCD_x + 1, LCD_y);
		}
	}
	
		// continuar
	numero_1 = lectura;

dibujar_numero_2:

	delay(retardo_lecturas);
	lectura = Numerico_Key();
	
		// reglas decimal
	if (Dec_Hex == 1)
	{
		if (lectura == 16 || lectura == 14)	// enter, numero 1
		{
			lcd.setCursor(LCD_x, LCD_y);
			lcd.print("0");
			numero_2 = 0;
			numero_unidades = 1;
			goto salida;
		}
		if (lectura > 9 && lectura < 14)	// tecla no valida
		{
			goto dibujar_numero_2;
		}
		if (lectura == 15)					// cancelar
		{
			goto numero_ant;
		}
	}
		// reglas hexadecimal
	if (Dec_Hex == 2)
	{
		if (lectura == 16)					// enter encoder, solo numero 1
		{
			lcd.setCursor(LCD_x, LCD_y);
			lcd.print("0");
			numero_2 = 0;
			numero_unidades = 1;
			goto salida;
		}
	}
	
	// dibujar numero
		// decimal
	if (Dec_Hex == 1)
	{
		lcd.setCursor(LCD_x, LCD_y);
		if (numero_lenght == 2)
		{
			lcd.print(numero_1);
			lcd.print(lectura);
			numero_2 = lectura;
			goto salida;
		}
		if (numero_lenght == 3)
		{
			lcd.setCursor(LCD_x + 1, LCD_y);
			lcd.print(numero_1);
			lcd.print(lectura);
			lcd.setCursor(LCD_x + 2, LCD_y);
		}
		if (numero_lenght == 4)
		{
			lcd.setCursor(LCD_x + 2, LCD_y);
			lcd.print(numero_1);
			lcd.print(lectura);
			lcd.setCursor(LCD_x + 3, LCD_y);
		}
	}
		// hexadecimal
	if (Dec_Hex == 2)
	{
		lcd.setCursor(LCD_x, LCD_y);
		lcd.print(numero_1, HEX);
		lcd.print(lectura, HEX);
		numero_2 = lectura;
		goto salida;
	}		
	
	numero_2 = lectura;

dibujar_numero_3:

	delay(retardo_lecturas);
	
	lectura = Numerico_Key();
	
		// reglas
	if (lectura == 16 || lectura == 14)	// enter, numero 2
	{
		numero_3 = 0;
		numero_unidades = 2;
		goto salida;
	}
	if (lectura > 9 && lectura < 14)	// tecla no valida
	{
		goto dibujar_numero_3;
	}
	if (lectura == 15)					// cancelar
	{
		goto numero_ant;
	}	
		// dibujar numero
	lcd.setCursor(LCD_x, LCD_y);
	if (numero_lenght == 3)
	{
		lcd.print(numero_1);
		lcd.print(numero_2);
		lcd.print(lectura);
		numero_3 = lectura;
		goto salida;
	}
	if (numero_lenght == 4)
	{
		lcd.setCursor(LCD_x + 1, LCD_y);
		lcd.print(numero_1);
		lcd.print(numero_2);
		lcd.print(lectura);
		lcd.setCursor(LCD_x + 3, LCD_y);
	}
	numero_3 = lectura;	
	
dibujar_numero_4:

	delay(retardo_lecturas);
	
	lectura = Numerico_Key();
	
		// reglas
	if (lectura == 16 || lectura == 14)	// enter, numero 3
	{
		numero_4 = 0;
		numero_unidades = 3;
		goto salida;
	}
	if (lectura > 9 && lectura < 14)	// tecla no valida
	{
		goto dibujar_numero_4;
	}
	if (lectura == 15)					// cancelar
	{
		goto numero_ant;
	}	
		// dibujar numero
	lcd.setCursor(LCD_x, LCD_y);
	lcd.print(numero_1);
	lcd.print(numero_2);
	lcd.print(numero_3);
	lcd.print(lectura);
	
	numero_4 = lectura;
		
	goto salida;	
	
numero_ant:
	
	lcd.noBlink();
		// inicial
	lcd.setCursor(LCD_x, LCD_y);
		// 2 digitos
	if (numero_lenght == 2)
	{
			// decimal
		if (Dec_Hex == 1)
		{
			if (num_ant < 10)
			{
				lcd.print("0");
			}
		}
			// hexadecimal
		if (Dec_Hex == 2)
		{
			if (num_ant < 16)
			{
				lcd.print("0");
			}
		}
	}
		// 3 digitos
	if (numero_lenght == 3)
	{
		if (num_ant < 10)
		{
			lcd.print("00");
		}
		if (num_ant > 9 && num_ant < 100)
		{
			lcd.print("0");
		}
	}
		// 4 digitos
	if (numero_lenght == 4)
	{
		if (num_ant < 10)
		{
			lcd.print("000");
		}
		if (num_ant > 9 && num_ant < 100)
		{
			lcd.print("00");
		}
		if (num_ant > 99 && num_ant < 1000)
		{
			lcd.print("0");
		}		
	}
		// dibujar numero
	if (Dec_Hex == 1)	// decimal
	{
		lcd.print(num_ant);
	}
	if (Dec_Hex == 2)	// hexadecimal
	{
		lcd.print(num_ant, HEX);
	}
	return num_ant;
	
salida:
		// agrupar numeros para enviar de vuelta
	lcd.noBlink();
	int numero_total;
	int numero_temp;
	
		// 1 digito
	if (numero_lenght == 1)
	{
			// limites
		lcd.setCursor(LCD_x, LCD_y);
		if (numero_1 > max)
		{
			numero_1 = max;
			if (Dec_Hex == 1)	// decimal
			{
				lcd.print(max);
			}
			if (Dec_Hex == 2)	// hexadecimal
			{
				lcd.print(max, HEX);
			}
		}
		if (numero_1 < min)
		{
			numero_1 = min; 
			if (Dec_Hex == 1)	// decimal
			{
				lcd.print(min);
			}
			if (Dec_Hex == 2)	// hexadecimal
			{
				lcd.print(min, HEX);
			}
		}
		
		return numero_1;
	}
		// 2 digitos
	if (numero_lenght == 2)
	{
		// cncatenar numero
			// decimal
		if (Dec_Hex == 1)
		{
			if (numero_unidades == 0)
			{
				numero_total 	= numero_1 * 10;
				numero_total 	= numero_total + numero_2;
			}
			else
			{
				numero_total 	= numero_1;
			}
		
		}
			// hexadecimal
		if (Dec_Hex == 2)
		{
			if (numero_unidades == 0)
			{
				numero_total 	= numero_1 * 16;
				numero_total 	= numero_total + numero_2;
			}
			else
			{
				numero_total 	= numero_1;
			}
			
		}
			// limites
		if (numero_total > max)
		{
			numero_total = max;
		}
		if (numero_total < min)
		{
			numero_total = min;
		}
			// escribir numero
		lcd.setCursor(LCD_x, LCD_y);
		if (numero_total < 10)
		{
			lcd.print("0");
			if (Dec_Hex == 1)	// decimal
			{
				lcd.print(numero_total);
			}
			if (Dec_Hex == 2)	// hexadecimal
			{
				lcd.print(numero_total, HEX);
			}
		}
		if (numero_total > 9)
		{
			if (Dec_Hex == 1)	// decimal
			{
				lcd.print(numero_total);
			}
			if (Dec_Hex == 2)	// hexadecimal
			{
				if (numero_total > 15)
				{
					lcd.print(numero_total, HEX);
				}
				else
				{
					lcd.print("0");
					lcd.print(numero_total, HEX);
				}
				
			}
		}
		
		return numero_total;
	}
		// 3 digitos
	if (numero_lenght == 3)
	{
		// concatenar numero
			// 1 digitos
		if (numero_unidades == 1)
		{
			numero_total 	= numero_1;
		}
			// 2 digitos
		if (numero_unidades == 2)
		{
			numero_total 	= numero_1 * 10;
			numero_total	= numero_total + numero_2;
		}
			// 3 digitos
		if (numero_unidades == 0)
		{
			numero_total 	= numero_1 * 100;
			numero_temp		= numero_2 * 10; 
			numero_total	= numero_total + numero_temp + numero_3;
		}
		
		// limites
		if (numero_total > max)
		{
			numero_total = max;
		}
		if (numero_total < min)
		{
			numero_total = min;
		}
			// escribir numero
		lcd.setCursor(LCD_x, LCD_y);
		if (numero_total < 10)
		{
			lcd.print("00");
			lcd.print(numero_total);
		}
		if (numero_total > 9 && numero_total < 100)
		{
			lcd.print("0");
			lcd.print(numero_total);
		}
		if (numero_total > 99)
		{
			lcd.print(numero_total);
		}
		
		return numero_total;
	}
		// 4 digitos
	if (numero_lenght == 4)
	{
			// concatenar numeros
		if (numero_unidades == 1)
		{
			numero_total 	= numero_1;
		}
		if (numero_unidades == 2)
		{
			numero_total 	= numero_1 * 10;
			numero_total	= numero_total + numero_2;
		}
		if (numero_unidades == 3)
		{
			numero_total 	= numero_1 * 100;
			numero_temp		= numero_2 * 10;
			numero_total	= numero_total + numero_temp + numero_3;
		}
		if (numero_unidades == 0)
		{
			numero_total 	= numero_1 * 1000;
			numero_temp		= numero_2 * 100; 
			numero_total	= numero_total + numero_temp;
			numero_temp		= numero_3 * 10;
			numero_total	= numero_total + numero_temp + numero_4;
		}
			// limites
		if (numero_total > max)
		{
			numero_total = max;
		}
		if (numero_total < min)
		{
			numero_total = min;
		}
			// escribir numero
		lcd.setCursor(LCD_x, LCD_y);
		if (numero_total < 10)
		{
			lcd.print("000");
			lcd.print(numero_total);
		}
		if (numero_total > 9 && numero_total < 100)
		{
			lcd.print("00");
			lcd.print(numero_total);
		}
		if (numero_total > 99 && numero_total < 1000)
		{
			lcd.print("0");
			lcd.print(numero_total);
		}
		if (numero_total > 999)
		{
			lcd.print(numero_total);
		}
		
		return numero_total;
	}
}

int Numerico_Key()
{
	// devuelve el numero de la tecla precionada o el centro del encoder
	// A = 10, B = 11, C = 12, D = 13, # = 14, * = 15, encoder = 16
	
	byte numero = 17;			// 17 para valor sin tecla
	
	// puertos a ceros
	digitalWrite(Keypad_A, HIGH);
	digitalWrite(Keypad_B, HIGH);
	digitalWrite(Keypad_C, HIGH);
	digitalWrite(Keypad_D, HIGH);
	
	while (numero == 17)
	{
		// encoder
		if (digitalRead(Enc_Center) == 0)
		{
			delay(300);
			numero = 16;
			break;
		}
		
		// A 1
		digitalWrite(Keypad_A, LOW);
		if (digitalRead(Keypad_1) == 0)
		{
			numero = 1;
			break;
		}
		
		// A 2
		if (digitalRead(Keypad_2) == 0)
		{
			numero = 2;
			break;
		}
		
		// A 3
		if (digitalRead(Keypad_3) == 0)
		{
			numero = 3;
			break;
		}
		
		// A 4
		if (digitalRead(Keypad_4) == 0)
		{
			numero = 10;
			break;
		}
		digitalWrite(Keypad_A, HIGH);
		
		// B 1
		digitalWrite(Keypad_B, LOW);
		if (digitalRead(Keypad_1) == 0)
		{
			numero = 4;
			break;
		}
		
		// B 2
		if (digitalRead(Keypad_2) == 0)
		{
			numero = 5;
			break;
		}
		
		// B 3
		if (digitalRead(Keypad_3) == 0)
		{
			numero = 6;
			break;
		}
		
		// B 4
		if (digitalRead(Keypad_4) == 0)
		{
			numero = 11;
			break;
		}
		digitalWrite(Keypad_B, HIGH);
		
		// C 1
		digitalWrite(Keypad_C, LOW);
		if (digitalRead(Keypad_1) == 0)
		{
			numero = 7;
			break;
		}
		
		// C 2
		if (digitalRead(Keypad_2) == 0)
		{
			numero = 8;
			break;
		}
		
		// C 3
		if (digitalRead(Keypad_3) == 0)
		{
			numero = 9;
			break;
		}
		
		// C 4
		if (digitalRead(Keypad_4) == 0)
		{
			numero = 12;
			break;
		}
		digitalWrite(Keypad_C, HIGH);
		
		// D 1
		digitalWrite(Keypad_D, LOW);
		if (digitalRead(Keypad_1) == 0)
		{
			numero = 15;
			break;
		}
		
		// D 2
		if (digitalRead(Keypad_2) == 0)
		{
			numero = 0;
			break;
		}
		
		// D 3
		if (digitalRead(Keypad_3) == 0)
		{
			numero = 14;
			break;
		}
		
		// D 4
		if (digitalRead(Keypad_4) == 0)
		{
			numero = 13;
			break;
		}
		digitalWrite(Keypad_D, HIGH);
	}
	
		// puertos a ceros
	digitalWrite(Keypad_A, HIGH);
	digitalWrite(Keypad_B, HIGH);
	digitalWrite(Keypad_C, HIGH);
	digitalWrite(Keypad_D, HIGH);
	
	return numero;
}

void Numerico_Write (int valor, int col, int row)
{
  // posicionar el valor en los campos 000

  lcd.setCursor (col, row);
  lcd.print ("000");

  if (valor < 10)
  {
    lcd.setCursor (col + 2, row);
  }

  if (valor > 9 && valor < 100)
  {
    lcd.setCursor (col + 1, row);
  }

  if (valor > 99)
  {
    lcd.setCursor (col, row);
  }

  lcd.print (valor);
}
