// constants won't change. They're used here to 
// set pin numbers:
const int ledPin =  13;      // the number of the LED pin
const int hallPin[]={
    A0,A1,A2,A3,A4,A5};
const int motorPin[]={11,12,13};

int hall[6];
int avg[3];

int phase[][3]={{0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0},{1,0,1}};
int phasePnt=0;
int hallPos[]={6,2,3,1,5,4}; // 5,1,2,0,4,3
int hallPosAddr[]={3,1,2,5,4,0};    // addr(hallPos[]-1)
int bit[3];

int rpm=0;
int lastBit0=0;
unsigned long time;

void setup() 
{
    // initialize the LED pin as an output:
    pinMode(ledPin, OUTPUT);      

    for(int i=0;i<6;i++)
    {
        pinMode(hallPin[i], INPUT_PULLUP);
        pinMode(motorPin[i/2], OUTPUT);        
    }

    Serial.begin(9600);
}

void loop()
{
    int i,k;
    
    //delay(5);
    for(i=0;i<6;i++)
    {
        hall[i] = analogRead(hallPin[i]);
    }          
    for(i=0;i<3;i++)
    {
        bit[i]=(hall[i*2+1]>hall[i*2]);        
    }
    k=(bit[0]<<2)|(bit[1]<<1)|(bit[2]);
    
    k--;
    /*Serial.print(k);
    Serial.print(" ");
    Serial.println(hallPosAddr[k]);*/
    
    for(i=0;i<3;i++)
    {
         digitalWrite(motorPin[i],phase[(hallPosAddr[k]+2)%6][i]);   
    }       
    
    if (bit[0]>lastBit0) rpm++;
    lastBit0=bit[0];
    
    if (rpm==0) time=millis();
    if (rpm==10) 
    {
        Serial.print("rpm: ");
        Serial.println(10.0/((millis()-time)/60000.0));
        rpm=0;   
    }
}

void loopa()
{
    int    i;
    int    bit[3];
    
    delay(100);    // 6,2,3,1,5,4
    for(i=0;i<3;i++)
    {
         digitalWrite(motorPin[i],phase[phasePnt][i]);   
    }
    
    // read the state of the pushbutton value:
    Serial.print("[");

    for(int i=0;i<6;i++)
    {
        hall[i] = analogRead(hallPin[i]);   
    }  
    for(i=0;i<3;i++)
    {
        bit[i]=(hall[i*2+1]>hall[i*2]);     
        Serial.print(bit[i]);
        Serial.print(" ");            
    }    
    Serial.print((bit[0]<<2)|(bit[1]<<1)|(bit[2]));
    Serial.print(" ");     
    Serial.print(phasePnt);
    Serial.println("]");   
    phasePnt=(phasePnt+1)%6;    
}







