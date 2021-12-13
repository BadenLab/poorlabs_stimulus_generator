/*
 * Adapting this code to only ESP32:
 * Change the LEDX values (in the very beginning of the code), to values that correspond to pin numbers on the Beehive 0.3 board
 * change the "tlc.setPWM(ledX,0);" calls to "ledcWrite(ledX, value)"
 * remove or comment out "tlc.write()"
 * 
 * 
 */


/* 
 
        LED Zappelin' is an open hardware LED stimulator that can individually                                                                                                                                                                                                                                                                                                                                      
control up to 24 LEDs.

This Arduino file is designed to be uploaded on an Adafruit_HUZZAH ESP32 Feather or an Arduino Nano
Source : https://github.com/BadenLab/LED-Zappelin
The script relies on an incoming blanking signal, generated by a TTL, which updates the LEDs.
The stimulus speed has to be adjusted to the scan logic (scanning mirrors speed that corresponds to 
the blanking signal generated by the TTL): 1kHz/1ms, 500Hz/2ms, etc.
A trigger signal is generated at the begining of the stimulus and then every 1.0 seconds.

Library "Adafruit TLC5947" needs to be installed.
 
The default script is parametered for 4 LEDs (connected to pin 0 to 3), and their respective proxy LEDs 
(connected to pin 4 to 7)


This project is Open Source licensed, released under CERN OHL v1.2

by  M.J.Y. Zimmermann, 2019.02.05
Baden Lab, CRPC, School of Life Sciences, University of Sussex, United Kingdom
*/


//libraries
#include <SerialCommand.h>


/* ------------------------------------------------------------------------------------
                           Stimulus parameters
-------------------------------------------------------------------------------------*/                                                                                             

int           nLoops           = 500;     //  Number of repeats of the arrays (i.e. Loops)
const int     nArrayEntries    = 3;       //  Number of entries in the arrays, including preadaption at position 0
const int     Scan_Logic       = 2;       //  Scanning logic (1 for 1ms scans, 2 for 2ms scans, etc.)



// // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // STIMULUS SEQUENCE START // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // 

// The array_LED# values are expressed in percentage and should only be expressed from 0-100
const int8_t array_LED1[] PROGMEM = {
0,100,0
};
const int8_t array_LED2[] PROGMEM = {    
0,0,0
};
const int8_t array_LED3[] PROGMEM = {    
0,0,0
};
const int8_t array_LED4[] PROGMEM = {
0,0,0
};

const int8_t array_LED5[] PROGMEM = {
0,0,0
};
const int8_t array_LED6[] PROGMEM = {    
0,0,0
};
const int8_t array_LED7[] PROGMEM = {    
0,0,0
};
const int8_t array_LED8[] PROGMEM = {
0,0,0
};
const int8_t array_LED9[] PROGMEM = {
0,0,0
};
const int8_t array_LED10[] PROGMEM = {    
0,0,0
};
const int8_t array_LED11[] PROGMEM = {    
0,0,0
};
const int8_t array_LED12[] PROGMEM = {
0,0,0
};


// The array_Time values are expressed in milliseconds
const int16_t array_Time[] PROGMEM = {      
10,1000,1000
};

// // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // //  STIMULUS SEQUENCE END  // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // //


/* ------------------------------------------------------------------------------------
                           Microcontroller Board Selection
-------------------------------------------------------------------------------------*/

      //#define Arduino_Nano
      //#define Adafruit_ESP32

      
/* ------------------------------------------------------------------------------------
                    Internal Definitions (Change at your own risk)
-------------------------------------------------------------------------------------*/
// setting PWM properties
const int freq = 5000;
const int resolution = 12;

// defining LEDs arduino channels
const int led1Channel = 0;
const int led2Channel = 1;
const int led3Channel = 2;
const int led4Channel = 3;

const int led5Channel = 4;
const int led6Channel = 5;
const int led7Channel = 6;
const int led8Channel = 7;
const int led9Channel = 8;
const int led10Channel = 9;
const int led11Channel = 10;
const int led12Channel = 11;



#include "LED_Values.h"                     // Include LED Values defined in the second tab
/*
#include <SPI.h>                            // Include the arduino serial port interface library
#include "Adafruit_TLC5947.h"               // Include the Adafruit TLC5947 libvrary. For better result one may change the number of channel defined in the Adafruit_TLC5947.cpp library


#ifdef Arduino_Nano
    #define   data            4             // SPI connection, DIN connected to pin D4
    #define   clock           5             // SPI connection, CLK connected to pin D5
    #define   latch           6             // SPI connection, LAT connected to pin D6
    //#define   oe              2           // Optional feature to prevent LED to light up when powering up the TLC board connected to pin D2
    const int      pSyncOut      =   A0;    // Trigger channel must be connected to pin A0
    const int      pBlank        =   3;     // Blanking signal channel must be connected to pin D3
#endif

#ifdef Adafruit_ESP32
    #define   data           18             // SPI connection, DIN connected to pin MOSI/18
    #define   clock           5             // SPI connection, CLK connected to pin SCK/5
    #define   latch          21             // SPI connection, LAT connected to pin 21
    //#define   oe            25            // Optional feature to prevent LED to light up when powering up the TLC board connected to pin A1/DAC1
    const int      pSyncOut      =   26;    // Trigger channel must be connected to pin A0/DAC2
    const int      pBlank        =   36;    // Blanking signal channel must be connected to pin A4/36
#endif

//    Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, clock, data, latch);

*/
    

/* Global variables */  

//char           sCmd;                        // Serial monitor 
SerialCommand sCmd;     // The demo SerialCommand object
int            i;                           // iteration loop factor
word           BaudRate      = 115200;      // Baud Rate
// Label LEDs with GPIO pins
int            led1Pin       = 16;           // Define 1st Channel (Beehive)       
int            led2          = 17;           // Define 2nd Channel
int            led3          = 5;            // Define 3rd Channel
int            led4          = 18;           // Define 4th Channel

int            led5          = 19;           // Define 5th Channel        
int            led6          = 21;           // Define 6th Channel
int            led7          = 22;           // Define 7th Channel 
int            led8          = 23;           // Define 8th Channel 
int            led9          = 14;           // Define 9th Channel (Beehive)
int            led10         = 27;           // Define 10th Channel (Beehive)
int            led11         = 26;          // Define 11th Channel (Beehive)
int            led12         = 25;          // Define 12th Channel (Beehive)


/* Global LED variables */
volatile word   stled1;                     // First LED value   
volatile word   stled2;                     // Second LED value
volatile word   stled3;                     // Third LED value
volatile word   stled4;                     // Fourth LED value

volatile word   stled5;                     // Fifth LED value   
volatile word   stled6;                     // Sixth LED value
volatile word   stled7;                     // Seventh LED value
volatile word   stled8;                     // Eighth LED value
volatile word   stled9;                     // Ninth LED value   
volatile word   stled10;                     // Tenth LED value
volatile word   stled11;                     // Eleventh LED value
volatile word   stled12;                     // Twelfth LED value

word            array1[nArrayEntries];      // First LED Array
word            array2[nArrayEntries];      // Second LED Array
word            array3[nArrayEntries];      // Third LED Array
word            array4[nArrayEntries];      // Fourth LED Array

word            array5[nArrayEntries];      // Fifth LED Array
word            array6[nArrayEntries];      // Sixth LED Array
word            array7[nArrayEntries];      // Seventh LED Array
word            array8[nArrayEntries];      // Eighth LED Array
word            array9[nArrayEntries];      // Ninth LED Array
word            array10[nArrayEntries];      // Tenth LED Array
word            array11[nArrayEntries];      // Eleventh LED Array
word            array12[nArrayEntries];      // Twelfth LED Array


/* Timing variables */
volatile word  pulse;                                             // Pulse number
word           Time[nArrayEntries];                               // Time Array
float          x_Logic            =   1.0/Scan_Logic;             // Factor applied to the timing according to the scanning logic

/* Trigger variables */
int     Trigger_Duration            =   25;                       // Length of the Trigger signal in ms
int     TriggerCounter_Duration     =   1000;                     // Lenght of the Trigger loop in ms
word    Trigger_dur        =   Trigger_Duration * x_Logic;        // Trigger signal duration adjusted
word    TriggerCounter_dur =   TriggerCounter_Duration * x_Logic; // Trigger loop duration adjusted  
    
/* Reset Parameters */
volatile word  iLoop                = 0;       // Actual loop number = iLoop + 1
volatile word  PulseCount           = 0;       // Count the number of pulses delivered by the blanking signal
volatile int   PulseCount_Trigger   = 0;       // Count the number of pulses generated for the trigger channel (A0 pin)
volatile int   ArrayPosition        = 0;       // Position array within the current loop

/* Flags */
bool   SettingFlag   =  true;
bool   BlankingFlag  =  true;


/* -----------------------------------------------------------------------------------
                                 Internal methods
 ------------------------------------------------------------------------------------*/

void setup(){

// Initialise the serial communication with PC
  Serial.begin(BaudRate);

// Initialise the Adafruit TLC driver 
  //tlc.begin();
  //tlc.write();  

// Set pins
  //pinMode(pSyncOut, OUTPUT);  
  //digitalWrite(pSyncOut, LOW);  
  //pinMode(pBlank, INPUT);
  ////digitalWrite(pBlank, HIGH);



// configure LED PWM functionalitites
  ledcSetup(led1Channel, freq, resolution);
  ledcSetup(led2Channel, freq, resolution);
  ledcSetup(led3Channel, freq, resolution);
  ledcSetup(led4Channel, freq, resolution);
  
  ledcSetup(led5Channel, freq, resolution);
  ledcSetup(led6Channel, freq, resolution);
  ledcSetup(led7Channel, freq, resolution);
  ledcSetup(led8Channel, freq, resolution);
  ledcSetup(led9Channel, freq, resolution);
  ledcSetup(led10Channel, freq, resolution);
  ledcSetup(led11Channel, freq, resolution);
  ledcSetup(led12Channel, freq, resolution);
  
  
// attach the channel to the GPIO to be controlled
  ledcAttachPin(led1Pin, led1Channel);
  ledcAttachPin(led2, led2Channel);
  ledcAttachPin(led3, led3Channel);
  ledcAttachPin(led4, led4Channel);
  
  ledcAttachPin(led5, led5Channel);
  ledcAttachPin(led6, led6Channel);
  ledcAttachPin(led7, led7Channel);
  ledcAttachPin(led8, led8Channel);
  ledcAttachPin(led9, led9Channel);
  ledcAttachPin(led10, led10Channel);
  ledcAttachPin(led11, led11Channel);
  ledcAttachPin(led12, led12Channel);
  
  ledcWrite(led1Channel, 0);
  // do for all leds
 // attach functions (after void loop) to callbacks
 sCmd.addCommand("a", a);
 sCmd.addCommand("b", b);
 sCmd.addCommand("off", Play0);
 sCmd.addCommand("playarrays", PlayArrays);
 
}// end setup

/* ----------------------------------------------------------------------------------*/
/* -------------------------------- Turn all LED off --------------------------------*/






/* ----------------------------------------------------------------------------------*/
/* ----------------------------------- Main Loop ------------------------------------*/

void loop(){

// Check if command received from PC
  //if(Serial.available()) {
    //sCmd = Serial.read();
    sCmd.readSerial();     // We don't do much, just process serial commands
}// end void loop
// Execute command
    //switch (sCmd) {


void Play0(){
      ledcWrite(led1Pin, 0);                            // Set First LED to 0
      ledcWrite(led2,0);                                // Set Second LED to 0
      ledcWrite(led3,0);                                // Set Third LED to 0 
      ledcWrite(led4,0);                                // Set Fourth LED to 0
      
      ledcWrite(led5,0);                                // Set Fifth proxy LED to 0
      ledcWrite(led6,0);                                // Set Sixth proxy LED to 0
      ledcWrite(led7,0);                                // Set Seventh proxy LED to 0 
      ledcWrite(led8,0);                                // Set Eighth proxy LED to 0
      ledcWrite(led9, 0);                               // Set Ninth LED to 0
      ledcWrite(led10,0);                               // Set Tenth LED to 0
      ledcWrite(led11,0);                               // Set Eleventh LED to 0 
      ledcWrite(led12,0);                               // Set Twelfth LED to 0
      
      //tlc.write();   
      Serial.println("-> All LEDs off"); 
      
//probably don't need these counters can just use inbuilt timer      
      PulseCount = 0;                                    // Reset Pulse counter
      PulseCount_Trigger = 0;                            // Reset Trigger pulse counter
      ArrayPosition = 0;                                 // Reset Array counter
      
      SettingFlag = true;                                // Return to initial condition
      BlankingFlag = true;                               // Return to initial condition
}


/* ----------------------------------------------------------------------------------*/
/* ------------------------------ Play Stimulus Array -------------------------------*/

void PlayArrays() {

/* Defining timing according to the current array position */
    pulse = Time[ArrayPosition];
    
/* Trigger ------------------------------------------------------------------------*/

    /* Keep Trigger on for the trigger duration */
    if(PulseCount_Trigger <= Trigger_dur && ArrayPosition > 0){
        PulseCount_Trigger += 1;                         // Trigger Pulse Counter, move to the next pulse
    } 
    /* Keep Trigger off for the rest of the trigger duration */
    if(PulseCount_Trigger > Trigger_dur && ArrayPosition > 0){
        //digitalWrite(pSyncOut, LOW);                     // Turn Trigger off
        PulseCount_Trigger += 1;                         // Trigger Pulse Counter, move to the next pulse
    }

    /* Once Trigger duration is reached, reset counter and turn trigger on */
    if(PulseCount_Trigger == TriggerCounter_dur+1 && ArrayPosition > 0){
        PulseCount_Trigger = 0;                          // Reset Trigger pulse counter
        //digitalWrite(pSyncOut, HIGH);                    // Turn Trigger on
    }

/* Moving Through the stimulus ----------------------------------------------------*/

    if(PulseCount == pulse){                             // When the pulse counter reaches the current position array duration ...
        
        if(ArrayPosition == 0){                                // Only at the very end of pre-adaptation ...
            Serial.println("   Loop 1");                       // ... Print the first loop
            //digitalWrite(pSyncOut, HIGH);                      // ... Turn Trigger on
        }

        ArrayPosition += 1;                              // ... Move to the next position array
        PulseCount = 0;                                  // ... Reset the pulse counter
        
        

        if(0 < ArrayPosition <= nArrayEntries){
           /* Defining LED values according to the current array position */
               stled1      =   array1[ArrayPosition];     // Set First LED value according to its position within the array
               stled2      =   array2[ArrayPosition];     // Set Second LED value according to its position within the array
               stled3      =   array3[ArrayPosition];     // Set Third LED value according to its position within the array
               stled4      =   array4[ArrayPosition];     // Set Fourth LED value according to its position within the array
                 
               stled5      =   array5[ArrayPosition];     // Set Fifth LED value according to its position within the array
               stled6      =   array6[ArrayPosition];     // Set Sixth LED value according to its position within the array
               stled7      =   array7[ArrayPosition];     // Set Seventh LED value according to its position within the array
               stled8      =   array8[ArrayPosition];     // Set Eighth LED value according to its position within the array
               stled9      =   array9[ArrayPosition];     // Set Ninth LED value according to its position within the array
               stled10     =   array10[ArrayPosition];     // Set Tenth LED value according to its position within the array
               stled11     =   array11[ArrayPosition];     // Set Eleventh LED value according to its position within the array
               stled12     =   array12[ArrayPosition];     // Set Twelfth LED value according to its position within the array
               
                       
           /* Feeding LED values according to the current position array */    
              ledcWrite(led1Pin ,stled1);                    // Update First LED value according to its position within the array
              ledcWrite(led2,stled2);                    // Update Second LED value according to its position within the array
              ledcWrite(led3,stled3);                    // Update Third LED value according to its position within the array
              ledcWrite(led4,stled4);                    // Update Fourth LED value according to its position within the array
              
              ledcWrite(led5 ,stled5);                    // Update Fifth LED value according to its position within the array
              ledcWrite(led6,stled6);                    // Update Sixth LED value according to its position within the array
              ledcWrite(led7,stled7);                    // Update Seventh LED value according to its position within the array
              ledcWrite(led8,stled8);                    // Update Eighth LED value according to its position within the array
              ledcWrite(led9 ,stled9);                    // Update Ninth LED value according to its position within the array
              ledcWrite(led10,stled10);                    // Update Tenth LED value according to its position within the array
              ledcWrite(led11,stled11);                    // Update Eleventh LED value according to its position within the array
              ledcWrite(led12,stled12);                    // Update Twelfth LED value according to its position within the array
              
              //tlc.write();
        }

        if(ArrayPosition == nArrayEntries){
            ArrayPosition = 1;                          // Reset the position array and skip the preadaptation (ArrayPosition = 0)
               
            ledcWrite(led1Pin, array1[1]);                 // Update First LED to its first array position value
            ledcWrite(led2,array2[1]);                 // Update Second LED to its first array position value
            ledcWrite(led3,array3[1]);                 // Update Third LED to its first array position value
            ledcWrite(led4,array4[1]);                 // Update Fourth LED to its first array position value
            
            ledcWrite(led5,array5[1]);                 // Update Fifth LED to its first array position value
            ledcWrite(led6,array6[1]);                 // Update Sixth LED to its first array position value
            ledcWrite(led7,array7[1]);                 // Update Seventh LED to its first array position value
            ledcWrite(led8,array8[1]);                 // Update Eighth LED to its first array position value
            ledcWrite(led9, array9[1]);                 // Update Ninth LED to its first array position value
            ledcWrite(led10,array10[1]);                 // Update Tenth LED to its first array position value
            ledcWrite(led11,array11[1]);                 // Update Eleventh LED to its first array position value
            ledcWrite(led12,array12[1]);                 // Update Twelfth LED to its first array position value
            
            //tlc.write();       

            iLoop += 1;                                 // Move to the next loop      
            if(iLoop < nLoops){                         // If Stimulus hasn't finish its last loop...
                Serial.print("   Loop ");               // ... Print new loop value
                Serial.println(iLoop+1);  
            } 
        }
    }

    if(iLoop == nLoops){                                // When Array has completed its last loop 
      ledcWrite(led1Pin, 0);                               // Set First LED to 0
      ledcWrite(led2,0);                               // Set Second LED to 0
      ledcWrite(led3,0);                               // Set Third LED to 0 
      ledcWrite(led4,0);                               // Set Fourth LED to 0
      
      ledcWrite(led5, 0);                               // Set 5th LED to 0
      ledcWrite(led6,0);                               // Set 6th LED to 0
      ledcWrite(led7,0);                               // Set 7th LED to 0 
      ledcWrite(led8,0);                               // Set 8th LED to 0
      ledcWrite(led9, 0);                               // Set 9th LED to 0
      ledcWrite(led10,0);                               // Set 10th LED to 0
      ledcWrite(led11,0);                               // Set 11th LED to 0 
      ledcWrite(led12,0);                               // Set 12th LED to 0
      
      //tlc.write();   
      
      PulseCount = 0;                                   // Reset Pulse counter
      PulseCount_Trigger = 0;                           // Reset Trigger pulse counter
      ArrayPosition = 0;                                // Reset Array counter
      //digitalWrite(pSyncOut, LOW);                      // Turn Trigger off
  
      Serial.println(" ...Done");                       // End of stimulus print 
      Serial.println("-> All LEDs off");
      
      SettingFlag = true;                               // Return to initial condition
      BlankingFlag = true;                              // Return to initial condition
    }
}// end playarrays


void a(){                                         // Array Stimulus at Power Intensity 1

          if(SettingFlag==true){
            //digitalWrite(pSyncOut,LOW);
            Serial.println("Playing Arrays, LEDs set at intensities 1 (press '0' to abort)");  
            Serial.print(nLoops);
            Serial.println(" Loops");
            PulseCount = 0;                             // Reset pulse counter
            PulseCount_Trigger = 0;                     // Reset trigger pulse counter
            ArrayPosition = 0;                          // Reset position array
            iLoop = 0;                                  // Reset loop counter 
        
            for(i=0; i<nArrayEntries; i++){             // Fetch LED Values from the attached library
              array1[i] = array_LED1[i] * max1_LED1/100;
              array2[i] = array_LED2[i] * max1_LED2/100;
              array3[i] = array_LED3[i] * max1_LED3/100;
              array4[i] = array_LED4[i] * max1_LED4/100;
              
              array5[i] = array_LED5[i] * max1_LED5/100;
              array6[i] = array_LED6[i] * max1_LED6/100;
              array7[i] = array_LED7[i] * max1_LED7/100;
              array8[i] = array_LED8[i] * max1_LED8/100;
              array9[i] = array_LED9[i] * max1_LED9/100;
              array10[i] = array_LED10[i] * max1_LED10/100;
              array11[i] = array_LED11[i] * max1_LED11/100;
              array12[i] = array_LED12[i] * max1_LED12/100;
              
              Time[i]   = array_Time[i] / Scan_Logic;
            }        
          
            Serial.println("Preadapt...");              // Set all LEDs to their preadaptation value
            ledcWrite(led1Pin, array_LED1[0]);
            ledcWrite(led2,array_LED2[0]);
            ledcWrite(led3,array_LED3[0]);
            ledcWrite(led4,array_LED4[0]);
            
            ledcWrite(led5, array_LED5[0]);
            ledcWrite(led6,array_LED6[0]);
            ledcWrite(led7,array_LED7[0]);
            ledcWrite(led8,array_LED8[0]);
            ledcWrite(led9, array_LED9[0]);
            ledcWrite(led10,array_LED10[0]);
            ledcWrite(led11,array_LED11[0]);
            ledcWrite(led12,array_LED12[0]);
            
            //tlc.write();
          
            SettingFlag = false;
          }
          
         while (iLoop<nLoops){
          
            //while(digitalRead(pBlank)==LOW){
                if(BlankingFlag==true){
                    PlayArrays();
                    PulseCount += 1;
                    BlankingFlag = false;          
                }
            //}
            
            BlankingFlag = true;

            // Option to interrupt 
            //if(Serial.available()) {
            //    sCmd = Serial.read(); 
            //    switch (sCmd) {
            //        case '0':
            //        Serial.println("...aborted!");
            //        Play0();
            //        return;
                //}
            //}
        
         }// end while
}// end a

void b(){
          if(SettingFlag==true){
            //digitalWrite(pSyncOut,LOW);
            Serial.println("Playing Arrays, LEDs set at intensities 2 (press '0' to abort)");  
            Serial.print(nLoops);
            Serial.println(" Loops");
            PulseCount = 0;                             // Reset pulse counter
            PulseCount_Trigger = 0;                     // Reset trigger pulse counter
            ArrayPosition = 0;                          // Reset position array
            iLoop = 0;                                  // Reset loop counter 
        
            for(i=0; i<nArrayEntries; i++){             // Fetch LED Values from the attached library 
                array1[i] = array_LED1[i] * max2_LED1/100;
                array2[i] = array_LED2[i] * max2_LED2/100;
                array3[i] = array_LED3[i] * max2_LED3/100;
                array4[i] = array_LED4[i] * max2_LED4/100;
                
                array5[i] = array_LED5[i] * max2_LED5/100;
                array6[i] = array_LED6[i] * max2_LED6/100;
                array7[i] = array_LED7[i] * max2_LED7/100;
                array8[i] = array_LED8[i] * max2_LED8/100;
                array9[i] = array_LED9[i] * max2_LED9/100;
                array10[i] = array_LED10[i] * max2_LED10/100;
                array11[i] = array_LED11[i] * max2_LED11/100;
                array12[i] = array_LED12[i] * max2_LED12/100;
                
                Time[i]   = array_Time[i] / Scan_Logic;
            }        
          
            Serial.println("Preadapt...");              // Set all LEDs to their preadaptation value
            ledcWrite(led1Pin, array_LED1[0]);
            ledcWrite(led2,array_LED2[0]);
            ledcWrite(led3,array_LED3[0]);
            ledcWrite(led4,array_LED4[0]);
            
            ledcWrite(led5, array_LED5[0]);
            ledcWrite(led6,array_LED6[0]);
            ledcWrite(led7,array_LED7[0]);
            ledcWrite(led8,array_LED8[0]);
            ledcWrite(led9, array_LED9[0]);
            ledcWrite(led10,array_LED10[0]);
            ledcWrite(led11,array_LED11[0]);
            ledcWrite(led12,array_LED12[0]);
            
            //tlc.write();
          
            SettingFlag = false;
          }
          
         while (iLoop<nLoops){
          
            //while(digitalRead(pBlank)==LOW){
                if(BlankingFlag==true){
                    PlayArrays();
                    PulseCount += 1;
                    BlankingFlag = false;          
                }
            //}
            
            BlankingFlag = true;
/*
            // Option to interrupt // MAY BE TOO SLOW
            if(Serial.available()) {
                sCmd = Serial.read(); 
                switch (sCmd) {
                    case '0':
                    Serial.println("...aborted!");
                    Play0();
                    return;
                }
            }
  */      
        }


}// end void b
  

/*
      case '0':
        // All LEDs off
        Play0();                                        // All LED to 0
       // //digitalWrite(pSyncOut,LOW);                  
      break;

  
      case '+':
        // ALL LEDs at max value
        ledcWrite(led1Pin,  max1_LED1);
        ledcWrite(led2, max1_LED2);
        ledcWrite(led3, max1_LED3);
        ledcWrite(led4, max1_LED4);
        
        ledcWrite(led5,  max1_LED5);
        ledcWrite(led6, max1_LED6);
        ledcWrite(led7, max1_LED7);
        ledcWrite(led8, max1_LED8);
        ledcWrite(led9,  max1_LED9);
        ledcWrite(led10, max1_LED10);
        ledcWrite(led11, max1_LED11);
        ledcWrite(led12, max1_LED12);
        
        //tlc.write();
        Serial.println("-> First LED to maximum power");                                        
        Serial.println("-> Second LED to maximum power");                                        
        Serial.println("-> Third LED to maximum power");                                        
        Serial.println("-> Fourth LED to maximum power");  
          
        Serial.println("-> Fifth LED to maximum power");
        Serial.println("-> Sixth LED to maximum power");
        Serial.println("-> Seventh LED to maximum power");
        Serial.println("-> Eighth LED to maximum power");
        Serial.println("-> Ninth LED to maximum power");
        Serial.println("-> Tenth LED to maximum power");
        Serial.println("-> Eleventh LED to maximum power");
        Serial.println("-> Twelfth LED to maximum power");
        
                                                                    
      break;     

      case '-':
        // ALL LEDs at min value
        ledcWrite(led1Pin,  max2_LED1);
        ledcWrite(led2, max2_LED2);
        ledcWrite(led3, max2_LED3);
        ledcWrite(led4, max2_LED4);
        
        ledcWrite(led5,  max2_LED5);
        ledcWrite(led6, max2_LED6);
        ledcWrite(led7, max2_LED7);
        ledcWrite(led8, max2_LED8);
        ledcWrite(led9,  max2_LED9);
        ledcWrite(led10, max2_LED10);
        ledcWrite(led11, max2_LED11);
        ledcWrite(led12, max2_LED12);
        
        //tlc.write();
        Serial.println("-> First LED to minimum power");                                        
        Serial.println("-> Second LED to minimum power");                                        
        Serial.println("-> Third LED to minimum power");                                        
        Serial.println("-> Fourth LED to minimum power");  
         
        Serial.println("-> Fifth LED to minimum power");
        Serial.println("-> Sixth LED to minimum power");
        Serial.println("-> Seventh LED to minimum power");
        Serial.println("-> Eighth LED to minimum power");
        Serial.println("-> Ninth LED to minimum power");
        Serial.println("-> Tenth LED to minimum power");
        Serial.println("-> Eleventh LED to minimum power");
        Serial.println("-> Twelfth LED to minimum power");
        
                                                                           
      break;  

      case '1':
        // First LEDs at max value
        ledcWrite(led1Pin,  max1_LED1);
        //tlc.write();
        Serial.println("-> First LED to full power");                                                                                                              
      break; 

      case '2':
        // First LEDs at minimum value
        ledcWrite(led1Pin,  max2_LED1);
        //tlc.write();
        Serial.println("-> First LED to minimum power");                                                                                                              
      break; 

      case '3':
        // Second LEDs at max value
        ledcWrite(led2, max1_LED2);
        //tlc.write();
        Serial.println("-> Second LED to full power");                                                                                                              
      break; 

      case '4':
        // Second LEDs at minimum value
        ledcWrite(led2, max2_LED2);
        //tlc.write();
        Serial.println("-> Second LED to minimum power");                                                                                                              
      break; 

      case '5':
        // Third LEDs at max value
        ledcWrite(led3, max1_LED3);
        //tlc.write();
        Serial.println("-> Third LED to full power");                                                                                                              
      break; 

      case '6':
        // Third LEDs at minimum value
        ledcWrite(led3, max2_LED3);
        //tlc.write();
        Serial.println("-> Third LED to minimum power");                                                                                                              
      break; 

      case '7':
        // Fourth LEDs at max value
        ledcWrite(led4, max1_LED4);
        //tlc.write();
        Serial.println("-> Fourth LED to full power");                                                                                                              
      break; 

      case '8':
        // Fourth LEDs at minimum value
        ledcWrite(led4, max2_LED4);
        //tlc.write();
        Serial.println("-> Fourth LED to minimum power");                                                                                                              
      break;

      case '9':
        // Fifth LEDs at max value
        ledcWrite(led5,  max1_LED5);
        //tlc.write();
        Serial.println("-> Fifth LED to full power");                                                                                                              
      break; 

      case '10':
        // Fifth LEDs at minimum value
        ledcWrite(led5,  max2_LED5);
        //tlc.write();
        Serial.println("-> Fifth LED to minimum power");                                                                                                              
      break;

      case '11':
        // Sixth LEDs at max value
        ledcWrite(led6, max1_LED6);
        //tlc.write();
        Serial.println("-> Sixth LED to full power");                                                                                                              
      break; 

      case '12':
        // Sixth LEDs at minimum value
        ledcWrite(led6, max2_LED6);
        //tlc.write();
        Serial.println("-> Sixth LED to minimum power");                                                                                                              
      break; 

      case '13':
        // Seventh LEDs at max value
        ledcWrite(led7, max1_LED7);
        //tlc.write();
        Serial.println("-> Seventh LED to full power");                                                                                                              
      break; 

      case '14':
        // Seventh LEDs at minimum value
        ledcWrite(led7, max2_LED7);
        //tlc.write();
        Serial.println("-> Seventh LED to minimum power");                                                                                                              
      break; 

      case '15':
        // Eighth LEDs at max value
        ledcWrite(led8, max1_LED8);
        //tlc.write();
        Serial.println("-> Eigth LED to full power");                                                                                                              
      break; 

      case '16':
        // Eigth LEDs at minimum value
        ledcWrite(led8, max2_LED8);
        //tlc.write();
        Serial.println("-> Eigth LED to minimum power");                                                                                                              
      break;

      case '17':
        // Ninth LEDs at max value
        ledcWrite(led9,  max1_LED9);
        //tlc.write();
        Serial.println("-> Ninth LED to full power");                                                                                                              
      break; 

      case '18':
        // Ninth LEDs at minimum value
        ledcWrite(led9,  max2_LED9);
        //tlc.write();
        Serial.println("-> Ninth LED to minimum power");                                                                                                              
      break; 

      case '19':
        // Tenth LEDs at max value
        ledcWrite(led10, max1_LED10);
        //tlc.write();
        Serial.println("-> Tenth LED to full power");                                                                                                              
      break; 

      case '20':
        // Tenth LEDs at minimum value
        ledcWrite(led10, max2_LED10);
        //tlc.write();
        Serial.println("-> Tenth LED to minimum power");                                                                                                              
      break;

      case '21':
        // Eleventh LEDs at max value
        ledcWrite(led11,  max1_LED11);
        //tlc.write();
        Serial.println("-> Eleventh LED to full power");                                                                                                              
      break; 

      case '22':
        // Eleventh LEDs at minimum value
        ledcWrite(led11,  max2_LED11);
        //tlc.write();
        Serial.println("-> Eleventh LED to minimum power");                                                                                                              
      break; 

      case '23':
        // Twelfth LEDs at max value
        ledcWrite(led12, max1_LED12);
        //tlc.write();
        Serial.println("-> Twelfth LED to full power");                                                                                                              
      break; 

      case '24':
        // Twelfth LEDs at minimum value
        ledcWrite(led12, max2_LED12);
        //tlc.write();
        Serial.println("-> Twelfth LED to minimum power");                                                                                                              
      break;
  
    //}// end switch
  
  //}// end if  
  
*/