#include <Adafruit_NeoPixel.h>
#define pinData 3
#define analo A2

int NUMPIXELS  = 45;
int level_delay[15] ={15,14,13,12,11,10,9,8,7,6,5,4,3,2};
int level_leds[15] = {10,9,8,7,7,6,6,5,5,4,4,3,3,2,2};
#define push 2
void funPush();
int levelContador = 0;
long long int t0 = micros();
long long int t1=  micros();
long long int t2 = micros();
long long int t3 = millis();

int m[10][4] ={
  {0,0,0,0},//0
  {0,0,0,1},//1
  {0,0,1,0},//2
  {0,0,1,1},//3
  {0,1,0,0},//4
  {0,1,0,1},//5
  {0,1,1,0},//6
  {0,1,1,1},//7
  {1,0,0,0},//8
  {1,0,0,1} //9 - >15 %10 -> 5 | 15/10 -> 1%10 = 1
};
volatile bool reset = false;
int velocidade = 3;
bool proximo_level = false;
bool perdeu = false;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS,pinData,NEO_GBR+NEO_KHZ800);
void setup() {
  pinMode(push,INPUT);
  pinMode(analo,INPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);

  
  randomSeed(analogRead(analo));
  attachInterrupt(digitalPinToInterrupt(push), funPush, RISING);
  pixels.begin();
  pixels.setBrightness(25);
  pixels.show();
  Serial.begin(9600);
}
int r2 ,g2 ,b2,r1 ,g1,b1;
int posfix ;
int  posvari;

void led_variavel(int posf,int posv ,int r1, int g1, int b1,int r2, int g2, int b2);
void led_fixo(int pos,int correntPos, int numLeds,int r, int g, int b);
void display7seg(int n)


void loop() {
     r2 = random(0,64);
     g2 = random(0,127);
     b2 = random(0,255);  
     r1 = random(0,255);
     g1 = random(0,127);
     b1 = random(0,64);
     posfix =random(0,44);
     posvari =random(0,44);
    led_variavel(posfix,posvari,r1,g1,b1,r2,g2,b2);
    digitalWrite(push, LOW);
    display7seg(levelContador+1);
}


void led_fixo(int pos,int correntPos, int numLeds,int r, int g, int b){
    
    if(numLeds +pos <= 45)
      for(int i = pos; i < numLeds+pos; i++){
         if(correntPos != i)
        pixels.setPixelColor(i,pixels.Color(b,g,r));
      }
    else{
      int d = numLeds-pos;
      for(int i = pos; i < 45; i++){
        if(correntPos != i)
        pixels.setPixelColor(i,pixels.Color(b,g,r));
      }
       for(int i = 0; i < pos-(45-numLeds); i++){
        if(correntPos != i)
        pixels.setPixelColor(i,pixels.Color(b,g,r));}
    }
}
void led_variavel(int posf,int posv ,int r1, int g1, int b1,int r2, int g2, int b2){

    for( int i = posv; i < NUMPIXELS+posf;){
    display7seg(levelContador+1);
    led_fixo(posf,i,level_leds[levelContador],r1,g1,b1);
    if(i == 0)
      pixels.setPixelColor(44,pixels.Color(0,0,0));
    pixels.setPixelColor(i-1,pixels.Color(0,0,0));
    if(micros()-t2 > 0){
        pixels.show();
        t2 = micros();
     }   
    if(millis()-t0 > level_delay[levelContador]*velocidade){
        t0 = millis();
        i+=1;
    }    
    pixels.setPixelColor(i,pixels.Color(b2,g2,r2));
    if(i == NUMPIXELS){
        i = 0;
    }
    if(reset){
       Serial.println(digitalRead(2));
       digitalWrite(push, LOW);
       reset = false;
      
        if(i>= posf && i <= posf+level_leds[levelContador]){
            proximo_level = true;
            levelContador +=1;
            perdeu = false;
       }else{
         perdeu = true;
         levelContador = 0;
         proximo_level = false;         
       }
        for(int k = 0; k < NUMPIXELS; k++)
          pixels.setPixelColor(k,pixels.Color(0,0,0));
        break;
    }
  }
  if(levelContador > 15){

    if(levelContador == 10){
      for(int i = 0; i <NUMPIXELS; i++)
        pixels.setPixelColor(i,pixels.Color(0,255,0));
      pixels.show();
      delay(1000);
      for(int i = 0; i <NUMPIXELS; i++)
        pixels.setPixelColor(i,pixels.Color(0,0,0));
      pixels.show();
  }
    levelContador = 0;
  }
  if(proximo_level){
    for(int i = 0; i <NUMPIXELS; i++)
      pixels.setPixelColor(i,pixels.Color(255,0,0));
      pixels.show();
    delay(1000);
    for(int i = 0; i <NUMPIXELS; i++)
      pixels.setPixelColor(i,pixels.Color(0,0,0));
      pixels.show();
    proximo_level = false;
  }else
    if(perdeu){
      for(int i = 0; i <NUMPIXELS; i++)
        pixels.setPixelColor(i,pixels.Color(0,0,255));
        pixels.show();
      delay(1000);
      for(int i = 0; i <NUMPIXELS; i++)
        pixels.setPixelColor(i,pixels.Color(0,0,0));
        pixels.show();
      perdeu = false;
    }
}
void funPush(){
  t3 = micros() ;
  if(micros()-t3 > 1){
    reset = true;
    t3 = micros();
  }
  else
   reset = false;
}


void display7seg(int n){

      digitalWrite(12,m[n%10][3]);
      digitalWrite(11,m[n%10][2]);
      digitalWrite(10,m[n%10][1]);
      digitalWrite(9 ,m[n%10][0]);
      digitalWrite(7, LOW);
      digitalWrite(8 , HIGH);
      delay(1);
      digitalWrite(12,m[(n/10)%10][3]);
      digitalWrite(11,m[(n/10)%10][2]);
      digitalWrite(10,m[(n/10)%10][1]);
      digitalWrite(9,m[(n/10)%10][0]);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
  
}