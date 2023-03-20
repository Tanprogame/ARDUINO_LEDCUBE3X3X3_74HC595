#define _clock 8
#define _latch 7
#define _data 6

#define GND1  0x0DFF
#define GND2  0x0BFF
#define GND3  0x07FF

#define BTN1  2

uint16_t mang_gnd[3]={GND1,GND2,GND3};
uint16_t mang_out[3][3]= {{0x0001,0x0002,0x0004},
                          {0x0008,0x0010,0x0020},
                          {0x0040,0x0080,0x0100}};
uint16_t a = 0x0E00;
int n,m,h,k,dem, h1,h2,h3, m1,m2,n1,n2, ran,r, cd;
int mode = 0;
unsigned long t1,t2;
void setup() {
  Serial.begin(9600);
  pinMode(_latch, OUTPUT);
  pinMode(_clock, OUTPUT);
  pinMode(_data, OUTPUT);
  digitalWrite(_clock, LOW);
  digitalWrite(_data, LOW);
  digitalWrite(_latch, LOW);
  t1=t2=0;
  ran = 0;
}

void out_pin(uint16_t data){
  digitalWrite(_latch, LOW);
  for(int i=0; i<16; i++){
    digitalWrite(_clock, 0);
    digitalWrite(_data, (data>>i)&0x01);
    digitalWrite(_clock, 1);
  } 
  digitalWrite(_latch, HIGH);
}
void on_led(){
  a = 0x0FFF;
  out_pin(a);
}
void off_led(){
  a = 0x0E00;
  out_pin(a);
}
void out_gnd(int z){
  a = a&mang_gnd[z];
  out_pin(a);
}
void out_gnd(int z1,int z2){
  a = a&mang_gnd[z1]&mang_gnd[z2];
  out_pin(a);
}
void out_gnd(int z1,int z2,int z3){
  a = a&mang_gnd[z1]&mang_gnd[z2]&mang_gnd[z3];
  out_pin(a);
}
void out_led(int x, int y){
  a = a|mang_out[x][y];
  out_pin(a);
}

void out_led(int x, int y,int z){
  a = ((a|mang_out[x][y])&mang_gnd[z]);
  out_pin(a);
}
void KT_BTN(){
  if(digitalRead(BTN1)==0){
    delay(20);
    if(digitalRead(BTN1)==0){
      mode++;
      dem=m=n=h=k=0;
      h1=h2=h3=ran=0;
      if(mode==7) if(cd==0) ran=1;
      if(mode==8) mode=0;
    }while(digitalRead(BTN1)==0);
  }
}
void loop() {
//  shiftOut(_data, _clock, LSBFIRST, i);
//  shiftOut(_data, _clock, LSBFIRST, i>>8);
  KT_BTN();
  if(mode==7){
    if(cd ==1){
//      do{
//        r = random(1,7);
//      }while(r==ran);
      ran++;
      if(ran==5)  ran=1;
      dem=m=n=h=k=0;
      h1=h2=h3=0;
      cd=0;
    }
  }
  if(mode==1||ran==1){                  //ONE LIGHT POINT MOTION
    if(h!=1){
      if(dem<3){
        n=dem;m=0;
      }else if(dem>=3 && dem<6){
        n=5-dem;m=1;
      }else if(dem>=6 && dem<9){
        n=dem-6;m=2;
      }
    }else{
      if(dem<3){
        n=2-dem;m=2;
      }else if(dem>=3 && dem<6){
        n=dem-3;m=1;
      }else if(dem>=6 && dem<9){
        n=8-dem;m=0;
      }
    }
    out_led(m,n,h);
    
    dem++;
    if(dem==9)  {
      dem=n=m=0;k++;
      if (k==4) {
        k=0;cd=1;
      }
      if(k<3) h=k;
      else  h = 1;
    }
    delay(100);
    off_led();
  }else if(mode==2||ran==2){          //ROTATIONAL MOTION
    if(dem<3){
      m=dem;n=0;
    }else{
      m=2;n=1;
    }
    out_led(1,1);
    out_led(m,n);
    out_led(2-m,2-n);
    out_gnd(h1,h2,h3);
    dem++;
    if(dem==4) {
      dem=0;
      if((k+1)%3==0) {
        if(k<8){
          h1=h2=h3=h1+1;
        }else if(k<11)  {
          h1=1;h2=h3=2;
        }else  if(k<14){
          h1=0;h2=1;h3=2;
        }else  if(k<17){
          h1=0;h2=h3=1;
        }
      }
      k++;
      if(k==18)  {
        k=0;h1=h2=h3=0;cd=1;
      }
    }
    delay(100);
    off_led();  
  }else if(mode==3||ran==3){          //PLANE MOTION
    if(dem<3){
      out_led(dem,0);
      out_led(dem,1);
      out_led(dem,2);
      out_gnd(0,1,2);
    }else if(dem<6){
      out_led(0,dem-3);
      out_led(1,dem-3);
      out_led(2,dem-3);
      out_gnd(0,1,2);
    }else if(dem<9){
      on_led();
      out_gnd(dem-6);
    }
    dem++;
    if(dem==9){
      dem=0;k++;
      if(k==3){
        k=0; cd=1;
      }
    }
    delay(200);
    off_led();
  }else if(mode==4||ran==4){        // ERUPTION MOTION
    if(dem<3){
      out_led(1,1,dem);
    }else if(dem<6){
      a = 0x0F45;
      out_pin(a);
      out_gnd(5-dem);
    }
    dem++;
    if(dem==6){
      dem=0;
      dem=0;k++;
      if(k==3){
        k=0; cd=1;
      }
    }
    delay(100);
    off_led();
  }else if(mode==5||ran==5){          // ZOOM OUT MOTION
    if(dem==0){
      out_led(1,1,1);
      delay(400);
    }else if(dem==1){
      for(int f=0; f<25;f++){
        for(int e=0; e<20;e++){
          out_led(1,1);
          out_gnd(0,1,2);
        }
        off_led();
        for(int e=0; e<20;e++){
          a = 0x0ABA;
          out_pin(a);
        }
        off_led();
      } 
    }else if(dem==2){
      a = 0x00BA;
      out_pin(a);
      delay(400);
    }else if(dem==3){
      on_led();
      out_gnd(0,1,2);
      delay(400);
    }
    dem++;
    if(dem==4){
      dem=0;k++;
      if(k==3){
        k=0; cd=1;
      }
    }
    off_led();
//    delay(100);
  }else if(mode==6||ran==6){  // TWO LIGHT POINT MOTION
    if(dem<3){
      m1 = dem;n1 = 0;
      m2 = 2; n2 = 2-dem;
    }else if(dem==3){
      m1 = 2;n1 = 1;
      m2 = 1;n2 = 0;
    }else if(dem>3&&dem<7){
      m1 = 6-dem; n1 = 2;
      m2 = 0;     n2 = dem-4;
    }else{
      m1 = 0; n1 = 1;
      m2 = 1; n2 = 2;
    }
    for(int f=0; f<10;f++){
      for(int e=0; e<10;e++){
        out_led(m1,n1,0);
      }
      off_led();
      for(int e=0; e<10;e++){
        out_led(1,1,1);
      }
      off_led();
      for(int e=0; e<10;e++){
        out_led(m2,n2,2);
      }
      off_led();
    }
    dem++;
    if(dem==8){
      dem=0;k++;
      if(k==3){
        k=0; cd=1;
      }
    }
  }
} 
