/*
  Andrew Mascolo 7/13/2014
  Updated WiiController library
  
  !!!DOES NOT WORK WITH NUNCHUCK!!!
  
  If you are using the adapter in this link,http://www.ebay.com/itm/Wii-WiiChuck-Nunchuck-Adapter-Module-Board-for-Arduino-/351052353286?pt=LH_DefaultDomain_0&hash=item51bc59cf06 
  then you most likely have it plug into the arduino across pins A2 - A5. Please know that the controller needs 3.3V to work properly and NOT 5V. You may cause possible
  to the controller if it is powered by 5 volts. What I have done here is allowed the user to use pin A2 as ground, by setting the pin to LOW, and making pin A3 an input
  so the user can have a jumper to the 3.3V pin without harming the Arduino.
  
  **.begin(Vcc, Gnd, baud rate)**
  Vcc: Set to TRUE if you have the adapter in the pin A3 and are using a jumper to 3.3V pin.
  Vcc: Set to FALSE if you are using your own wires and are not using pin A3 for power.
  
  Gnd: Set to TRUE if you have the adapter in the pin A2, this will automatically set pin A2 to LOW.
  Gnd: Set to FALSE if you are using your own wires and are not using pin A2 for ground.
  
  Baud rate: Set USE_MONITOR in the .h file to TRUE to use the serial.print functions to see their data.
  
*/ 

#include<WiiClassicControl.h>
#include<Wire.h>

WiiClassicControl Wii;

void setup()
{
  Wii.begin(true, true); // SEE ABOVE
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop()
{
  Wii.CollectData();

  if(Wii.leftShoulderPressed())
    Serial.print("LB |");
  
  if(Wii.rightShoulderPressed())
    Serial.print("RB |");
    
  if(Wii.lzPressed())
    Serial.print("ZL |");
  
  if(Wii.rzPressed())
    Serial.print("ZR |");
  
  if(Wii.leftDPressed())
    Serial.print("Left |");
  
  if(Wii.rightDPressed())
    Serial.print("Right |");
  
  if(Wii.upDPressed())
    Serial.print("Up |");
  
  if(Wii.downDPressed())
    Serial.print("Down |");
    
  if(Wii.selectPressed())  
    Serial.print("Select |");
  
  if(Wii.startPressed())  
    Serial.print("Start |");
  
  if(Wii.homePressed())  
    Serial.print("Home |");
  
  if(Wii.xPressed())  
    Serial.print("X |");
  
  if(Wii.yPressed())  
    Serial.print("Y |");
  
  if(Wii.aPressed()){  
    Serial.print("A |");
    digitalWrite(13, HIGH);
  }
  
  if(Wii.bPressed()) { 
    Serial.print("B |");
    digitalWrite(13, LOW);
  }
  
  // the lowest value is 0, and the highest value is 248.
  Serial.print("LSX: "); Serial.print(Wii.leftStickX());
  Serial.print("| LSY: "); Serial.print(Wii.leftStickY());
  Serial.print("| RSX: "); Serial.print(Wii.rightStickX());
  Serial.print("| RSY: "); Serial.println(Wii.rightStickY());

  delay(100); // used to see changes in the serial monitor easier.
}

