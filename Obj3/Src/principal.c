#include "gassp72.h"

extern void son_callback(void);
int etat=0;

typedef struct {
	int position;		// index courant dans le tableau d'echantillons
	int taille;		// nombre d'echantillons de l'enregistrement
	short int * son;	// adresse de base du tableau d'echantillons en ROM
	int resolution;		// pleine echelle du modulateur PWM
	int Tech_en_Tck;	// periode d'ech. audio en periodes d'horloge CPU
} type_etat;

/*
E_POS	equ	0
E_TAI	equ	4
E_SON	equ	8
E_RES	equ	12
E_PER	equ	16
	end
*/


type_etat Type_Etat;

int main(void)
{	
	int Periode_en_Tck=720000;
	
	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	// initialisation du timer 4
	// Periode_en_Tck doit fournir la durée entre interruptions,
	// exprimée en périodes Tck de l'horloge principale du STM32 (72 MHz)
	Timer_1234_Init_ff( TIM4, Periode_en_Tck );
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 2 est la priorité, timer_callback est l'adresse de cette fonction, a créér en asm,
	// cette fonction doit être conforme à l'AAPCS
	Active_IT_Debordement_Timer( TIM4, 2, son_callback );
	// lancement du timer
	Run_Timer( TIM4 );
	
	while	(1)
		{
		}
}
