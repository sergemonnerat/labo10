/*
------------------------------------------------------------
Copyright 2003-20xx Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier :	iUart.c
Auteur et Date :	Monnerat Serge 11.01.20xx

Description dans le fichier iUart.h
------------------------------------------------------------
*/

#include "iUart.h"
#include <MKL46Z4.h>
#include <core_cm0plus.h>

// Frï¿½quence du bus
#define kBusClockkHz 24000

// Vitesse de transmission
#define kUART0BaudRate 115200

// Priorité de l'interruption RX
#define kUART0Prio 1

// 50 bytes buffer
#define kRxBufSize ((UInt8)(50)) 

// RX buffer control struct
static struct
{
	Int8     	RxBuf[kRxBufSize];
	UInt16 	  InIndex;
	UInt16 	  OutIndex;
	UInt16    ByteCount;
	bool      BufferIsFull;
}sUartRxBufStruct;

//-----------------------------------------------------------------------
// UART 2 module configuration
//-----------------------------------------------------------------------
void iUart_Config(void)
{
	unsigned int aSbr;
	
	// Enable du clock de l'UART0
	// System Clock Gating Control Register 4 (SIM_SCGC4)
	SIM->SCGC4|=SIM_SCGC4_UART0_MASK;

	// Config de la source du clock de l'UART0
	// System Options Register 2 (SIM_SOPT2)
	SIM->SOPT2&=SIM_SOPT2_UART0SRC_MASK;
	SIM->SOPT2|=SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2|=SIM_SOPT2_PLLFLLSEL_MASK;
	
	// Configuration du crossbar pour les pin RX et TX (PTA1 et PTA2)
	// Pin Control Register n (PORTx_PCRn) --> UART = alternative 3
	PORTA->PCR[1]&=~PORT_PCR_MUX_MASK;
	PORTA->PCR[1]|=PORT_PCR_MUX(2);
	PORTA->PCR[2]&=~PORT_PCR_MUX_MASK;
	PORTA->PCR[2]|=PORT_PCR_MUX(2);
	
	// UART Control Register 1 (UARTx_C1)
	// PE = 0, pas de parité
	// M = 0 Normal --> start + 8 data bits (lsb first) + stop
	UART0->C1&= ~(UART_C1_M_MASK|UART_C1_PE_MASK );
	
	// Configuration de la vitesse de transmission
	// UART Baud Rate Registers:High (UARTx_BDH)
	// UART Baud Rate Registers: Low (UARTx_BDL)
	aSbr = (unsigned int)((kBusClockkHz*1000)/(kUART0BaudRate * 16));
	UART0->BDH&=(~UART_BDH_SBR_MASK);
	UART0->BDH|=UART_BDH_SBR(((aSbr & 0x1F00) >> 8));
	UART0->BDL= (unsigned char)(aSbr & UART_BDL_SBR_MASK);
	
	
	// Enable de l'interruption UART0 Rx au niveau du NVIC
	// Le vecteur du UART0 est le numéro 14
	NVIC_EnableIRQ(UART0_IRQn);
		
	// Configuration de la priorité de l'interruption PIT
	// O : plus haute priorité
	// 3 : plus basse priorité
	NVIC_SetPriority(UART0_IRQn, kUART0Prio);
	
	// Disable des interruptions
	// UART Control Register 2 (UARTx_C2)
	// UART Control Register 3 (UARTx_C3)
	UART0->C2&= (~(UART_C2_TIE_MASK|UART_C2_TCIE_MASK|UART_C2_RIE_MASK|UART_C2_ILIE_MASK));
	UART0->C3&= (~(UART_C3_ORIE_MASK|UART_C3_NEIE_MASK|UART_C3_FEIE_MASK|UART_C3_PEIE_MASK));
	
	// Enable de l'interruption RX
	// UART Control Register 2 (UARTx_C2)
	UART0->C2|=UART_C2_RIE_MASK;
}

//------------------------------------------------------------
// Enable/Disable de la transmission --> TX
//------------------------------------------------------------
void iUart_EnDisTx(UartEnDisEnum aEnDis)
{
	// UART Control Register 2 (UARTx_C2)
	if(kEn==aEnDis)
		{
			UART0->C2|=UART_C2_TE_MASK; 		// Enable TX
		}
	else if(kDis==aEnDis)
		{
			UART0->C2&=(~UART_C2_TE_MASK); // Disable TX
		}
}


//------------------------------------------------------------
// Enable/Disable de la rï¿½ception
//------------------------------------------------------------
void iUart_EnDisRx(UartEnDisEnum aEnDis)
{
	// UART Control Register 2 (UARTx_C2)
	if(kEn==aEnDis)
		{
			UART0->C2|=UART_C2_RE_MASK; 		// Enable RX
		}
	else if(kDis==aEnDis)
		{
			UART0->C2&=(~UART_C2_RE_MASK); // Disable RX
		}
}

//------------------------------------------------------------
// Get UART0 flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetStatus(UartStatusEnum aStatus)
{
	// UART Status Register 1 (UARTx_S1)
	return ((UART0->S1&aStatus)==aStatus);
}

//------------------------------------------------------------
// UART0 Data register write
// aData: datas to transmit
//------------------------------------------------------------
void iUart_SetData(unsigned char aData)
{
	// UART Data Register (UARTx_D)
	UART0->D=aData;
}

//------------------------------------------------------------
// Initialisation du buffer de réception
// !!! Il faut que l'interrupt soit disable --> problème de
// concurence !!!
// aVal: la valeur avec laquelle on initialise le buffer 
//------------------------------------------------------------
void iUart_InitRxBuffer(unsigned char aVal)

{
	UInt16 i=0;
	
	
	// Initialisation des index
	sUartRxBufStruct.InIndex=0;
	sUartRxBufStruct.OutIndex=0;
	
	// Reset du compteur de bytes
	sUartRxBufStruct.ByteCount=0;
	
	// Reset du flag indiquant que le buffer est plein
	sUartRxBufStruct.BufferIsFull=false;
	
	// Reset du buffer
	for(i=0;i<kRxBufSize;i++)
		{
			sUartRxBufStruct.RxBuf[i]=aVal; 
		}
	
}

//------------------------------------------------------------
// Contrôle si le buffer est vide
// Return : true --> buffer empty, false --> buffer not empty
//------------------------------------------------------------
bool iUart_IsBufferEmpty(void)
{
	bool aRet=false;
	
	if(sUartRxBufStruct.ByteCount==0)
		{
			aRet=true;
		}
	else
		{
			aRet=false;
		}

	return aRet;
}

//------------------------------------------------------------
// Lecture d'un byte dans le buffer
// Return : retourne le plus vieux byte
//------------------------------------------------------------
Int8 iUart_GetCharFromBuffer(void)
{
  Int8 aChar=0;
	
	// Byte read
	aChar=sUartRxBufStruct.RxBuf[sUartRxBufStruct.OutIndex];
	
	// Incrémentation de l'index
	sUartRxBufStruct.OutIndex++;
	
	// Buffer tournant
	//sUartRxBufStruct.OutIndex%=kRxBufSize;
	if(sUartRxBufStruct.OutIndex>=kRxBufSize)
		{
			sUartRxBufStruct.OutIndex=0;
		}
	
	// Décrémentation du compteur de bytes
	sUartRxBufStruct.ByteCount--;
	 
	// Retourne un byte du buffer
	return aChar;
}

//---------------------------------------------------------------------------
// RX interrupt 
//---------------------------------------------------------------------------
void UART0_IRQHandler(void)
{
	Int8 aVal;
	
	// Lecture de la donnée reçue et du registre de statut (clear du flag)
	UART0->S1;
	aVal=UART0->D;
	
	
	if(sUartRxBufStruct.ByteCount>=kRxBufSize)
	  {
	    // Buffer full flag
	    sUartRxBufStruct.BufferIsFull=true;
	  }
	else
	  {
	    // Buffer not full flag
	    sUartRxBufStruct.BufferIsFull=false;
	    
	    // Sauvegarde de la donnée reçue
	    sUartRxBufStruct.RxBuf[sUartRxBufStruct.InIndex]=aVal;
	    
	    // Incrémentation de l'index
	    sUartRxBufStruct.InIndex++;
	    
	    // Incrï¿½mentation du compteur de byte
	    sUartRxBufStruct.ByteCount++;
	
	    // Buffer tournant
	    // sUartRxBufStruct.InIndex%=kRxBufSize;
	    if(sUartRxBufStruct.InIndex>=kRxBufSize)
	      {
	        sUartRxBufStruct.InIndex=0;
	      }
	  }
}


