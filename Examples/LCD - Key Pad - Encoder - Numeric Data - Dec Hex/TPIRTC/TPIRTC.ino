// ejemplo de recoleccion de datos numericos desde el keypad
// recibe decimales de 1 a 4 digitos
// recibe hexadecimales de 1 y 2 digitos
// regresa los datos por el serial
// la posicion es seleccionable
// seleccionable el minimo, el maximo y el default en el caso de no selecciona ninguno



// librerias
#include <LiquidCrystal.h>		// libreria para LCD - ttps://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home	
#include <Wire.h>
#include <Encoder.h>			// libreria encoder - http://www.pjrc.com/teensy/td_libs_Encoder.html	
	// Teclado
byte Keypad_A			= 27;
byte Keypad_B			= 25;
byte Keypad_C			= 23;
byte Keypad_D			= 29;
byte Keypad_1			= 31;
byte Keypad_2			= 33;
byte Keypad_3			= 35;
byte Keypad_4			= 37;
	// LCD
byte LCD_RS 			= 46;
byte LCD_E  			= 44;
byte LCD_D4 			= 36;
byte LCD_D5 			= 34;
byte LCD_D6 			= 32;
byte LCD_D7				= 30;
byte LCD_Contrast 		= 3;
byte LCD_Backlight		= 2;
LiquidCrystal lcd		(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);  //LCD setup
byte LCD_Back_Light		= 0;
byte LCD_Cont			= 0;
byte LCD_Time_Out		= 0;
	// Encoder
byte 	Encoder_DT		= 43; 	
byte 	Encoder_CL		= 41;
byte 	Encoder_SW 		= 39;
Encoder myEnc(Encoder_CL, Encoder_DT);	// conexion de encoder

void setup()
{
	// LCD
	pinMode		(LCD_RS, 		OUTPUT);
	pinMode		(LCD_E,  		OUTPUT);
	pinMode		(LCD_D7, 		OUTPUT);
	pinMode		(LCD_D6, 		OUTPUT);
	pinMode		(LCD_D5, 		OUTPUT);
	pinMode		(LCD_D4, 		OUTPUT);
	pinMode		(LCD_Contrast, 	OUTPUT);
	pinMode		(LCD_Backlight, OUTPUT);
	lcd.begin	(20, 4);						//tamaño de LCD
	// logger
	Serial.begin(9600);
	// Teclado
	pinMode		(Keypad_A, OUTPUT);
	pinMode		(Keypad_B, OUTPUT);
	pinMode		(Keypad_C, OUTPUT);
	pinMode		(Keypad_D, OUTPUT);
	pinMode		(Keypad_1, INPUT_PULLUP);
	pinMode		(Keypad_2, INPUT_PULLUP);
	pinMode		(Keypad_3, INPUT_PULLUP);
	pinMode		(Keypad_4, INPUT_PULLUP);
	// encoder
	pinMode		(Encoder_DT, INPUT);
	pinMode		(Encoder_CL, INPUT);
	pinMode		(Encoder_SW, INPUT_PULLUP);
}

void loop()
{	
	Init_LCD();
	
	leer:
	
	lcd.clear();
	Serial.println(Numerico_Write(2, 2540, 0, 0, 1, 130));	// (minimo, maximo, coordenada x, coordenada y, decimal 1 - hexadecimal 2, numero anterior)
	
	delay(1000);
	
	goto leer;
}


void Init_LCD()
{
	// inicializador de lcd 
	analogWrite(LCD_Contrast,  120);
	analogWrite(LCD_Backlight, 255);
}

int  Numerico_Write(int min, int max, byte LCD_x, byte LCD_y, byte Dec_Hex, int num_ant)
{
	// regresa el numero tecleado
	// numero minimo a escribir - numero maximo a escribir - x y de pantalla donde se escribe
	// el calculo del tamaño del numero lo hace en automatico
	// num_ant, el numero que estaba impreso
	// Dec_Hex decimal 1, hexadecimal 2
	
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
		if (digitalRead(Encoder_SW) == 0)
		{
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