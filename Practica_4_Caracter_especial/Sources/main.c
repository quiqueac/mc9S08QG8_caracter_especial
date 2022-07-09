#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


void inicializarPantalla(void);
void instruccion(unsigned char x);
void dato(unsigned char x);
void enable(void);
void retardo(unsigned char t);
void MCUinit(void);
void mensaje(unsigned char *p);
unsigned char separarDato(unsigned char dato, unsigned char parte);
void crearCaracter(unsigned char *pattern, unsigned char location);
void mostrarCaracter(void);

char customChar[8] = {
  0x00,
  0x0A,
  0x15,
  0x11,
  0x0A,
  0x04,
  0x00,
  0x00
};

void main(void) {
	MCUinit();
	inicializarPantalla();
	crearCaracter(&customChar[0], 1);
	mostrarCaracter();
	fin:
	goto fin;
	/*
	for (;;) {
		__RESET_WATCHDOG()
		;
	}
	*/
}

void inicializarPantalla(void) {
	unsigned char i = 0;
	unsigned char tabla[7] = { 0x33, 0x32, 0x28, 0x0F, 0x06, 0x01};
	while (i < 6) {
		instruccion(tabla[i]);
		i++;
	}
}

void crearCaracter(unsigned char *pattern, unsigned char location){
	int i=0; 
	instruccion(0x40+(location*8));     //Send the Address of CGRAM
	for (i=0; i<8; i++)
		dato(pattern [ i ] );         //Pass the bytes of pattern on LCD 
}

void mostrarCaracter(void){
	instruccion(0x80);
	dato(1);
}

void mensaje(unsigned char *p) {
	while (*p != 0) {
		dato(*p);
		p++; // i++;
	}
}

unsigned char separarDato(unsigned char dato, unsigned char parte) {
	if (parte)
		return (dato & 0xF0);
	else if (!parte)
		return ((dato & 0x0F) << 4);
}

void instruccion(unsigned char x) {
	PTAD_PTAD0 = 0;
	PTBD = separarDato(x, 1);
	enable();
	PTBD = separarDato(x, 0);
	enable();
}

void dato(unsigned char x) {
	PTAD_PTAD0 = 1;
	PTBD = separarDato(x, 1);
	enable();
	PTBD = separarDato(x, 0);
	enable();
}

void enable(void) {
	PTAD_PTAD1 = 1;
	retardo(0x01);
	PTAD_PTAD1 = 0;
}

void retardo(unsigned char t) {
	SRTISC = t;
	while (SRTISC_RTIF == 0);
	SRTISC_RTIACK = 1;
	SRTISC = 0x00;
}

void MCUinit(void) {
	SOPT1 = 0x12;
	PTBDD = 0xFF;
	PTADD_PTADD0 = 1;
	PTADD_PTADD1 = 1;
	PTBD = 0x00;
	PTAD_PTAD0 = 0;
	PTAD_PTAD1 = 0;
}
