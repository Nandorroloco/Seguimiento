//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//  Seguimiento_v2
//
//  Site: https://github.com/Nandorroloco/Seguimiento
//
//  License: http://creativecommons.org/licenses/by-sa/3.0/deed.es
//
//  Created by; Nandorroloco
//  year: 2021
//////////////////////////////////////////////////////////////////
//  beta: version 1   31/12/2021
// 		version 2		15/01/2022		4 botones
// --------------------------------
//////////////////////////////////////////////////////////////////


#include <TimerOne.h>

#define SLOW 40        // a calcular
#define MSLW 20
#define MIDD 5
#define FAST 1		// ojo, si la frecuencia es muy alta, igual el motorcito no lo puede seguir

// #define UTIMER  7333    // microsegundos a programar en el trimmer para una corona de 144
// #define UTIMER 8800    // para una corona de 120 dientes, son 5 vueltas a la hora
#define UTIMER 8600    // para una corona de 120 dientes, son 5 vueltas a la hora, valor empírico 2% menos
// #define UTIMER 11000    // para una corona de 96 dientes, son 4 vueltas a la hora
// #define UTIMER  4000    // para pruebas

#define B_SIDERAL       0b00000001
#define B_DIRECCION     0b00000010
#define B_SPEED_UP      0b00000100
#define B_SPEED_DOWN    0b00001000


// fases del motor unipolar

int fase1 = 12;
int fase2 = 11;
int fase3 = 10;
int fase4 = 9;

// los botones

int pin_B0 = 2;   // Sideral
int pin_B1 = 3;   // Dirección
int pin_B2 = 4;   // Speed up
int pin_B3 = 5;   // Speed down


// los leds

int pin_LED_S = 8;    // led de seguimiento
int pin_LED_V = 7;    // led de velocidad
int pin_LED_D = 6;    // led de dirección

int status_LED_S = 0;
int status_LED_V = 0;
int status_LED_D = 0;

int flash_S = 0;
int flash_V = 0;
int flash_D = 0; 

// Se cuentan por los "delays", no necesitan ser muy precisos

#define MIDELAY 25      // delay en milisegundos
#define MEDIOSEG 20     // cada 20 bucles es 500 ms
#define UNSEGUNDO 40         // cada 40 bucles es 1 segundo   40*25 = 1000
#define UNTERCIO 6
#define UNCUARTO 10

int pulsado = false;
int boton;
int direccion = 0;
int sideral  = false;
int paso = 0;
int tick;
int pulsos = SLOW;

void excita_motor(int paso)
{
  switch(paso)
  {
  case 0:         //paso 0
  digitalWrite(fase1, HIGH);
  digitalWrite(fase2, HIGH);  
  digitalWrite(fase3, LOW);
  digitalWrite(fase4, LOW);
    break;
  case 1:         //paso 1
  digitalWrite(fase1, LOW);
  digitalWrite(fase2, HIGH);  
  digitalWrite(fase3, HIGH);
  digitalWrite(fase4, LOW);
    break;
  case 2:         //paso 2
  digitalWrite(fase1, LOW);
  digitalWrite(fase2, LOW);  
  digitalWrite(fase3, HIGH);
  digitalWrite(fase4, HIGH);
    break;
  case 3:         //paso 3
  digitalWrite(fase1, HIGH);
  digitalWrite(fase2, LOW);  
  digitalWrite(fase3, LOW);
  digitalWrite(fase4, HIGH);
    break;
  }
}

void siguiente_paso(int direccion)
{

  if ( direccion )
  {
    if ( paso < 3 )
      paso++;
    else
      paso = 0;
  }
  else
  {
    if ( paso > 0 )
      paso--;
    else
      paso = 3;
  }
}


void interrup_t1()
{
  if ( tick == 0 )
  {
    if ( sideral )
    {
     siguiente_paso( direccion );
     excita_motor(paso);
    }
    tick = pulsos;
  }
  else
  {
    if ( sideral )
    {
      tick--;
    }
  }
}

int botones()
{
  int rc = 0;
 

   if( digitalRead(pin_B0)== LOW) rc += B_SIDERAL;
   if( digitalRead(pin_B1)== LOW) rc += B_DIRECCION;
   if( digitalRead(pin_B2)== LOW) rc += B_SPEED_UP;
   if( digitalRead(pin_B3)== LOW) rc += B_SPEED_DOWN;
       
 return(rc);
}

void pulsados()
{

  boton = botones();    // test de los botones
  
  if ( boton != 0 )     // se ha pulsado
  {
    pulsado = boton;      
    return;         // señalizamos que se ha pulsado el botón, pero sólo actuaremos cuando se libere
  }
  else
  {
   if ( !pulsado )      
   return;        // no hay botón y no se había pulsado... no hacemos nada
  }
  
  switch( pulsado )
  {
  case B_SIDERAL:    // boton velocidad sideral
    pulsos = SLOW;
    sideral = !sideral;
    break;
  case B_DIRECCION:    // boton dirección
    direccion = !direccion;
    break;
  case B_SPEED_UP:    // boton incrementar velocidad
    switch (pulsos)
   {
   case SLOW:
         tick = pulsos = MSLW;
         break;
   case MSLW:
         tick = pulsos = MIDD;
         break;
   case MIDD:
   case FAST:
         tick =pulsos = FAST;
         break;
     }
    break;
  case B_SPEED_DOWN:    // boton decrementar velocidad
    switch (pulsos)
   {
   case FAST:
         tick =pulsos = MIDD;
         break;
   case MIDD:
         tick =pulsos = MSLW;
         break;
   case SLOW:
   case MSLW:
         tick =pulsos = SLOW;
         break;
     }
    break;
  }

pulsado = false;    // aquí llegamos si hemos ejecutado una acción
}

void parpadea()
{

// Seguimiento... si está activo, parpadea cada segundo,
if ( sideral )
   {
   if ( flash_S >= UNSEGUNDO)
      {
      if ( status_LED_S == 1)
   {
     digitalWrite(pin_LED_S, HIGH);   //apaga el led
     status_LED_S = 0;
     }
      else
     {
     digitalWrite(pin_LED_S, LOW);    //enciende el led
     status_LED_S = 1;
     }
     flash_S = 0; 
     }
     else
     flash_S++;
   }

// Dirección... si está activo, parpadea cada cuarto de segundo, cuando va al revés
if ( direccion )
   {
   if ( flash_D >= MEDIOSEG)
      {
      if ( status_LED_D == 1)
   {
     digitalWrite(pin_LED_D, HIGH);   //apaga el led
     status_LED_D = 0;
     }
      else
     {
     digitalWrite(pin_LED_D, LOW);    //enciende el led
     status_LED_D = 1;
     }
     flash_D = 0; 
     }
     else
     flash_D++;
   }
 else
   {
   if ( !status_LED_D )
      {
      digitalWrite(pin_LED_D, LOW);   //enciende el led
      status_LED_D = 1;
      }
   }
   
//  marcaremos la velocidad si el motor está en movimiento   
   
if ( sideral )
   {
   if ( ((flash_V >= UNSEGUNDO) && (pulsos == SLOW))  ||
        ((flash_V >= MEDIOSEG) && (pulsos == MSLW))     ||
        ((flash_V >= UNTERCIO) && (pulsos == MIDD))     ||
        ((flash_V >= UNCUARTO) && (pulsos == FAST))     )
      {
      if ( status_LED_V == 1)
   {
     digitalWrite(pin_LED_V, HIGH);   //apaga el led
     status_LED_V = 0;
     }
      else
     {
     digitalWrite(pin_LED_V, LOW);    //enciende el led
     status_LED_V = 1;
     }
     flash_V = 0; 
     }
     else
     flash_V++;
   }
 else
   {
   if ( !status_LED_V )
      {
      digitalWrite(pin_LED_V, LOW);   //enciende el led
      status_LED_V = 1;
      }
   }

}

// TODO: put your setup code here, to run once:

void setup() {

  
  // initialize the button pin as a input:
  pinMode(pin_B0, INPUT);
  digitalWrite(pin_B0, HIGH);       // turn on pullup resistors
  pinMode(pin_B1, INPUT);
  digitalWrite(pin_B1, HIGH);       // turn on pullup resistors
  pinMode(pin_B2, INPUT);
  digitalWrite(pin_B2, HIGH);       // turn on pullup resistors
  pinMode(pin_B3, INPUT);
  digitalWrite(pin_B3, HIGH);       // turn on pullup resistors
  
  
  // initialize the motor phases:
  pinMode(fase1, OUTPUT);
  pinMode(fase2, OUTPUT);
  pinMode(fase3, OUTPUT);
  pinMode(fase4, OUTPUT); 
  
   // initialize LEDs:
  pinMode(pin_LED_S, OUTPUT);
  pinMode(pin_LED_V, OUTPUT);
  pinMode(pin_LED_D, OUTPUT);
  
  digitalWrite(pin_LED_S, LOW);   //enciende los LEDs
  digitalWrite(pin_LED_V, LOW);  
  digitalWrite(pin_LED_D, LOW);
  
    // initialize timer1... UTIMER corresponde al cálculo de la velocidad sideral

  Timer1.initialize(UTIMER);
  Timer1.attachInterrupt(interrup_t1);
  


}

void loop() {

// TODO: put your main code here, to run repeatedly:

    pulsados();
    parpadea();
    delay(MIDELAY);

}
