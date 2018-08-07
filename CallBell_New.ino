int Char_DB[50]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0xEF,0x3F,0x9D,0x7B,0xDF,0x8F,0xBD,0x6F,0x02,0x79,0x02,0x1D,0x02,0x2B,0x3B,0xCF,0x02,0x02,0x02,0x1F,0x39};
 
#include <RCSwitch.h>
#include <EEPROM.h>


int xc=0;

void EEPROMWritelonFixedCode(int address, long value)
{
  
byte four = (value & 0xFF);
byte three = ((value >> 8) & 0xFF);
byte two = ((value >> 16) & 0xFF);
byte one = ((value >> 24) & 0xFF);

EEPROM.write(address, four);
delay(50);
EEPROM.write(address + 1, three);
delay(50);
EEPROM.write(address + 2, two);
delay(50);
EEPROM.write(address + 3, one);
delay(50);

}

void EEPROMWritelonDigit5(int address, String value)
{
EEPROM.write(address, value[0]);
delay(50);
EEPROM.write(address + 1, value[1]);
delay(50);
EEPROM.write(address + 2, value[2]);
delay(50);
EEPROM.write(address + 3, value[3]);
delay(50);
EEPROM.write(address + 4, value[4]);
delay(50);
}

long EEPROMReadlongFixedCode(long address)
{
long four = EEPROM.read(address);
delay(50);
long three = EEPROM.read(address + 1);
delay(50);
long two = EEPROM.read(address + 2);
delay(50);
long one = EEPROM.read(address + 3);
delay(50);
return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}






RCSwitch mySwitch = RCSwitch();
unsigned int EntryDatabase[50];

int CurrentEntry=0;
int A = 7;
int B = A0;
int C = A1;
int D = 4;
int E = 5;
int F = 6;
int G = A3;
int DP = A2;
int ShiftClk = 8;
int SelectData = 9;

int i=0;



unsigned long address_entry[30];

char database_entry[30][5];

long RemoteDB[30]; 

char Disp_entry[30][5];

char Disp_entry1[30][5];

void WriteData(int x){
  digitalWrite(A, (x >> 7) & 1);
  digitalWrite(B, (x >> 6) & 1);
  digitalWrite(C, (x >> 5) & 1);
  digitalWrite(D, (x >> 4) & 1);
  digitalWrite(E, (x >> 3) & 1);
  digitalWrite(F, (x >> 2) & 1);
  digitalWrite(G, (x >> 1) & 1);
  digitalWrite(DP, (x >> 0) & 1);
}



void ClearDisp(){
  digitalWrite(SelectData, HIGH);
  for(int i = 0; i <= 100; i++){
     digitalWrite(ShiftClk, HIGH); 
     delayMicroseconds(40);        
     digitalWrite(ShiftClk, LOW); 
     delayMicroseconds(40);
  }
}


void SelpulseOut(){
  digitalWrite(SelectData, HIGH);
  digitalWrite(ShiftClk, HIGH); 
  delayMicroseconds(40);        
  digitalWrite(ShiftClk, LOW); 
  digitalWrite(SelectData, LOW);
  delayMicroseconds(40);
}


void PrintData(){
  SelpulseOut();
  for(int j=0;j<=9;j++){
    for(i=0; i<=5; i++){
      WriteData(0x00);
      digitalWrite(ShiftClk, HIGH); 
      WriteData(Disp_entry[j][i]);
      delayMicroseconds(40);        
      digitalWrite(ShiftClk, LOW);
      delayMicroseconds(40);
    }
  }
}






void CreateTable(long remote){
  for(int x=0; x<30;x++){
    if((RemoteDB[x]) == remote){
      if(EntryDatabase[x] > 50){
        digitalWrite(3, HIGH);
        Serial.print(x+1);
        Serial.print(" ");
        Serial.print(remote);
        Serial.println(" Call");
        xc=0;
        for(int y=0;y<5;y++){
          Disp_entry[CurrentEntry][y]=Disp_entry1[x][y];
        }
      EntryDatabase[x]=CurrentEntry;
      CurrentEntry++;
      }
    }
    
    if((RemoteDB[x]-3) == remote){
      if(EntryDatabase[x] < 50){
        Serial.print(x+1);
        Serial.print(" ");
        Serial.print(remote);
        Serial.println(" Reset");
        int TempEntry=EntryDatabase[x];
        for(int y=TempEntry;y<CurrentEntry;y++){
          for(int t=0;t<5;t++){
            Disp_entry[y][t]=Disp_entry[y+1][t];
          }
        }
        for(int p=0;p<30;p++){
          if(EntryDatabase[p] > TempEntry){
            EntryDatabase[p]=EntryDatabase[p]-1;
          }
        }
        CurrentEntry--;
        for(int y=0;y<5;y++){
          Disp_entry[CurrentEntry][y]=0x00;
        }
        EntryDatabase[x]=100;
      }
    }
  }
  if(CurrentEntry<=0){
    digitalWrite(3, LOW);
    CurrentEntry=0;
  }
}

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(ShiftClk, OUTPUT);
  pinMode(SelectData, OUTPUT);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(DP, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(ShiftClk, LOW);
  digitalWrite(SelectData, LOW);
  Serial.begin(9600);
  
  for(int y=0;y<30;y++){
    EntryDatabase[y]=100;
  }
  for(int x=0;x<30;x++){
    RemoteDB[x]=EEPROMReadlongFixedCode((x)*10);
    Serial.print(x);
    Serial.print("\t");
    Serial.println(RemoteDB[x]);
    delay(10);
    Disp_entry1[x][0]= Char_DB[EEPROM.read(((x)*10)+5)-48];
    delay(10);
    Disp_entry1[x][1]= Char_DB[EEPROM.read(((x)*10)+6)-48];
    delay(10);
    Disp_entry1[x][2]= Char_DB[EEPROM.read(((x)*10)+7)-48];
    delay(10);
    Disp_entry1[x][3]= Char_DB[EEPROM.read(((x)*10)+8)-48];
    delay(10);
    Disp_entry1[x][4]= Char_DB[EEPROM.read(((x)*10)+9)-48];
    delay(10);
  }
  mySwitch.enableReceive(0);
  ClearDisp(); 
  digitalWrite(3, HIGH);
  Welcome();
  digitalWrite(3, LOW);
}


void Welcome(){
  for(int k=0; k<=9; k++){
    for(int j=1; j<=50; j++){
      SelpulseOut();
      for(i=1; i<=51; i++){
        WriteData(0x00);
        digitalWrite(ShiftClk, HIGH); 
        WriteData(Char_DB[k]);
        delayMicroseconds(20);        
        digitalWrite(ShiftClk, LOW);
      }
    }
  }
}
long value = 0;

void loop() {
    if (mySwitch.available()) {
      value = mySwitch.getReceivedValue();
      if (value == 0) {
          Serial.print("Unknown encoding");
        } else {
          CreateTable(value);
      }
      mySwitch.resetAvailable(); 
    }else{
      delay(2);
    }
    if(Serial.available()){
      char data = Serial.read();
      if(data == '@'){
        while(Serial.available()){Serial.read();}
        Serial.println("Entry Mode :");
x:
        Serial.print("<Number> of New Remote: ");
        while(Serial.available()==0);
        delay(200);
        String para = Serial.readString();
        int int_pos=para.toInt();
        while(Serial.available()){Serial.read();}
        if(int_pos > 30 || int_pos < 1){
          Serial.println("Provide Value Between 1-30");
          goto x;
        }
        Serial.println(int_pos);
        Serial.print("Press the Call Button (New Remote): ");
        while(mySwitch.available()==0);
        delay(20);
        value = mySwitch.getReceivedValue();
        if (value == 0) {
          Serial.print("Unknown encoding");
        }else{
          Serial.println(value);
          EEPROMWritelonFixedCode((int_pos-1)*10,value);
          long b=EEPROMReadlongFixedCode((int_pos-1)*10);
          Serial.println(b);
        }
        while(Serial.available()){Serial.read();}
digit:
        Serial.print("<5-Digit> of New Remote: ");
        while(Serial.available()==0);
        delay(30);
        String Digit5 = Serial.readString();
        if(Digit5.length() > 5 || Digit5.length() < 5 ){
          Serial.println("Only 5-Digit Display");
          goto digit;
        }
        EEPROMWritelonDigit5(((int_pos-1)*10)+5,Digit5);
        Serial.println(Digit5);
        while(Serial.available()){Serial.read();}
        mySwitch.resetAvailable(); 
      }
    }
  PrintData();
  //xc++;
  //if(xc>=10){
  //  digitalWrite(3, LOW);
   // xc=0;
  //}
}

