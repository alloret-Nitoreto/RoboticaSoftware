/*
 *  readMcpdInfra8.c:
 *  read value from ADC MCPdInfra8
 *
 * Requires: wiringPi (http://wiringpi.com)
 * Copyright (c) 2015 http://shaunsbennett.com/piblog
 ***********************************************************************
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "mcp3004.h"
#include <softPwm.h>

#define RANGO       100
#define Izq       0 //El 17 en el GPIO
#define MotorPaso 4//EL 23 en el GPIO
#define Sentido 5
#define Der       3 //El 22 en el GPIO



void Parar(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 0 );

    //15 para abajo avanza
    softPwmWrite(Der, 0 );
    }

void avanzar(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 20 );

    //15 para abajo avanza
    softPwmWrite(Der, 10 );
}

void retroceder(){
    //15 el medio, 16 para arriba avanzar
    softPwmWrite(Izq, 10 );
    //15 para abajo avanza
    softPwmWrite(Der, 20 );
    delay(20);
}

void girarDerecha(){
  //Paro la rueda izquierda
    //15 el medio
	softPwmWrite(Izq, 20 );
    softPwmWrite(Der, 20 );
	delay(10);
   
}

void girarIzquierda(){
    //Paro la rueda izquierda
    //15 el medio
	softPwmWrite(Izq, 10 );
    softPwmWrite(Der, 10 );
	delay(5);

   
}

int main(int argc, char *argv[]) {

    int parado = 0;
	
    wiringPiSetup();
    mcp3004Setup(100, 0);
    pinMode (LED, OUTPUT) ;
    pinMode (Sentido, OUTPUT) ;
    int dInfra = 200;

    softPwmCreate (Der, 0, RANGO);//Derecho
    softPwmCreate (Izq, 0, RANGO);//Izquierdo
    while(1){
        int ch1 = analogRead(100);
        int ch2 = analogRead(101);
        int ch3 = analogRead(102);
        int ch4 = analogRead(103);

        if (parado == 1){
            if(ch1 < dInfra && ch2 < dInfra ){
            avanzar();
            parado = 0;
            }
            else {
                while (analogRead(100) >= dInfra && analogRead(101) < dInfra){
                    digitalWrite (Sentido, LOW) ;  
            
                    digitalWrite (MotorPaso, HIGH) ;	// On
                    delay (20) ;		// mS
                    digitalWrite (MotorPaso, LOW) ;	// Off
                    delay (20) ;		// mS

                }
                        

                while (analogRead(101) >= dInfra && analogRead(100) < dInfra){
                    digitalWrite (Sentido, HIGH) ;

                    digitalWrite (MotorPaso, HIGH) ;	// On
                    delay (20) ;		// mS
                    digitalWrite (MotorPaso, LOW) ;	// Off
                    delay (20) ;		// mS
                }
            }
            
        }

        else{

            //Sensor linea izquierda
            if(ch3 <= 600 ){
                girarIzquierda();
            }
            
            else if(ch4 <= 300 ){
		//sensor linea derecha
                girarDerecha();
            }else{
                avanzar();
            }

            if (ch3 < 600 && ch4 < 300){
                retroceder();
            }
            
            //Globo derecha
            if(ch1 >= dInfra){
                Parar();
                parado = 1;
                //Torreta
            }


            //globo izquierda
            if(ch2 >= dInfra ){
                Parar();
                parado = 1;
                //Torreta y algun calculo de distancia
            }
        }

    }
    return 0;
	
}