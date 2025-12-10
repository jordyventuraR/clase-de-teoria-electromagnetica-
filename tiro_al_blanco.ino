#include <Wire.h>
#include <U8g2lib.h>

// Inicializa la pantalla OLED 128x64 con controlador SSD1306 en modo I2C
// (usa los pines por defecto SDA=21, SCL=22 del ESP32)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define pulsador 2
#define rojo 12
#define verde 13
#define azul 3
#define potenciometro A0
#define sensor 14

int pot = 0;
int seleccion = 1;
int opcion = 0;
int signal32 = 0;
int acierto = 0;
int puntaje = 0;
int reinicio = 0;

int pantalla_inicio(){
  int accion = 0;
  do {
    digitalWrite(verde, LOW); // apaga
    digitalWrite(azul, HIGH); // apaga
    delay(500);
    digitalWrite(azul, LOW); // apaga
    delay(500);

    u8g2.clearBuffer();        // Limpia el buffer
    u8g2.setCursor(32, 15);     // Posición X=0, Y=10
    u8g2.print("Reiniciar");        
    u8g2.setCursor(32, 35);     // Posición X=0, Y=10
    u8g2.print("Continuar");  

     pot = analogRead(potenciometro);

     if(pot <500){
      u8g2.drawFrame(32, 5, 60, 15);
      accion = 1;
      u8g2.sendBuffer();
      delay(300);
      if(digitalRead(pulsador)==HIGH){
        return accion;
      }
    }else if(pot >500){ //continuar
      u8g2.drawFrame(32, 25, 60, 15);
      u8g2.sendBuffer();
      delay(300);
      accion = 2;
      if(digitalRead(pulsador)==HIGH){
        return accion;
      }
    }
  }while (digitalRead(pulsador)!=HIGH); 

  return accion;
}

void dificultad(int yo){
    u8g2.clearBuffer();        // Limpia el buffer
    u8g2.setCursor(32, 15);     // Posición X=0, Y=10
    u8g2.print("Facil");        // Text}
    u8g2.setCursor(32, 35);     // Posición X=0, Y=10
    u8g2.print("Medio"); 
    u8g2.setCursor(32, 55);     // Posición X=0, Y=10
    u8g2.print("Dificil"); 

    u8g2.drawFrame(30, yo, 60, 15);

    u8g2.sendBuffer();

}

void IRAM_ATTR ISR_boton() {
  // Código que se ejecuta cuando ocurre la interrupción
  acierto=1;

}

void win(int total){
  digitalWrite(azul, LOW);
  digitalWrite(verde, HIGH);
  u8g2.clearBuffer();        // Limpia el buffer
  u8g2.setCursor(32, 35);     // Posición X=0, Y=10
  u8g2.print("Victory!!!");
  u8g2.setCursor(35, 55);
  u8g2.print(total);
  u8g2.sendBuffer();
  delay(5000);
  digitalWrite(verde, LOW);
}

void lose(int total){
  digitalWrite(azul, LOW);
  digitalWrite(rojo, HIGH);
  u8g2.clearBuffer();        // Limpia el buffer
  u8g2.setCursor(32, 35);     // Posición X=0, Y=10
  u8g2.print("Loser!!!"); 
  u8g2.setCursor(35, 55);
  u8g2.print(total);
  u8g2.sendBuffer();
  delay(500);
  digitalWrite(rojo, LOW);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Inicio U8g2 con texto...");

  u8g2.begin();
  Serial.println("Pantalla inicializada correctamente");

  // Limpiar pantalla
  u8g2.clearBuffer();

  // Configurar fuente
  u8g2.setFont(u8g2_font_ncenB08_tr); // Fuente legible pequeña
  
  
  // Escribir texto
  
  u8g2.clearBuffer();
  u8g2.sendBuffer(); // refresca la pantalla

  pinMode(sensor, INPUT);
  pinMode(pulsador, INPUT);
  pinMode(rojo, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(potenciometro, INPUT);
  pinMode(2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(sensor), ISR_boton, RISING);
}

void loop() {
    digitalWrite(verde, LOW); // apaga
    digitalWrite(azul, HIGH); // apaga
    delay(500);
    digitalWrite(azul, LOW); // apaga
    delay(500);


    //cuando se reinicia
    if(puntaje!=0){
      if (pantalla_inicio() == 1) {
        puntaje=0;
      }
    }
    pot = analogRead(potenciometro);
    //Reiniciar
    
    if (pot <341){
      dificultad(5);
      delay(300);
      opcion = 1;

    }else if(pot <682){
       dificultad(25);
       delay(300);
       opcion = 2;
    }else{
      dificultad(45);
      delay(300);
      opcion = 3;
    }
    
    if(digitalRead(pulsador)==1){
      u8g2.clearBuffer();        // Limpia el buffer
      u8g2.setCursor(32, 35);     // Posición X=0, Y=10
      u8g2.print("Suerte..."); 
      u8g2.sendBuffer();
      switch(opcion){
        case 1:
          do{
            if(signal32==1){
              digitalWrite(azul, HIGH); // apaga
              delay(5000);
              if(acierto==1){
                acierto = 0;
                puntaje += 1;
                win(puntaje);
                delay(300);
                break;
              }else{
                lose(puntaje);
                delay(800);
                break;
              }
            }
          }while(signal32!=1);
        break;
        case 2:
          do{
            if(signal32==1){
              digitalWrite(azul, HIGH); // apaga
              delay(7000);
              if(acierto==1){
                acierto = 0;
                puntaje += 2;
                win(puntaje);
                delay(500);
                break;
              }else{
                lose(puntaje);
                delay(800);
                break;
              }
            }
          }while(signal32!=1);
        break;
        case 3:
          do{
            if(signal32==1){
              digitalWrite(azul, HIGH); // apaga
              delay(10000);
              if(acierto==1){
                acierto = 0;
                puntaje += 3;
                win(puntaje);
                delay(800);
                break;
              }else{
                lose(puntaje);
                delay(800);
                break;
              }
            }
          }while(signal32!=1);
        break;
      } 
    }
}
