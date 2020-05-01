#include "gassp72.h"

extern void timer_callback(void);
int etat=0;
extern short TabSig[];
extern int CalculDFT(unsigned short *,int );

int M2[6];
unsigned short int dma_buf[64];
int cmpt[6]={0,0,0,0,0,0};
int M2TIR=3936256/4;
int SYSTICK_PER=360000;

void sys_callback(){
	// Démarrage DMA pour 64 points
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	for(int k=17;k<=24;k++){
		M2[k-17]=CalculDFT(dma_buf,k);
		if(M2[k-17]>M2TIR){
			cmpt[k-17]++;
			GPIO_Set(GPIOB, 1);
		}
		else{
			cmpt[k-17]=0;
			GPIO_Clear(GPIOB, 1);
		}
	}	
}
	
int main(void)
{	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	// PA2 (ADC voie 2) = entrée analog
	GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
	// PB1 = sortie pour profilage à l'oscillo
	GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	// PB14 = sortie pour LED
	GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);

	// activation ADC, sampling time 1us
	Init_TimingADC_ActiveADC_ff( ADC1, 72 );
	Single_Channel_ADC( ADC1, 2 );
	// Déclenchement ADC par timer2, periode (72MHz/320kHz)ticks
	Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
	// Config DMA pour utilisation du buffer dma_buf (a créér)
	Init_ADC1_DMA1( 0, dma_buf );

	// Config Timer, période exprimée en périodes horloge CPU (72 MHz)
	Systick_Period_ff( SYSTICK_PER );
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 3 est la priorité, sys_callback est l'adresse de cette fonction, a créér en C
	Systick_Prio_IT( 3, sys_callback );
	SysTick_On;
	SysTick_Enable_IT;
	
	while	(1)
		{
		
		}
}
