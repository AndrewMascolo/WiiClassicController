#ifndef WiiClassicControl_h
#define WiiClassicControl_h
/*
  Creator: Andrew Mascolo
  This library is a fixed version of the original library created by Tim Hirzel.
*/

#include <Arduino.h>
#include<Wire.h>

#define VERSION 1.02
/*
Version 1.01:
  Fixed button defines for X, Y, A and B.
  They were causing errors with my other sketches and libraries.

Version 1.02:
  Added two new functions:
    SetLeftStick_Factors(byte, byte)
    SetRightStick_Factors(byte, byte)
    These multiply the returned values from the joysticks. 1: Lowest(0 - 31 range) to 8: Highest(0 - 248 range)

Version 1.03:
  Fixed bug with getting the button pressed and .Begin() function.
*/
#define ON 0x1
#define OFF 0x0
#define NEUTRAL 0x2


#define _off(x) do{ pinMode(x, OUTPUT); digitalWrite(x, LOW); }while(0)
#define _on(x) do{ pinMode(x, OUTPUT); digitalWrite(x, HIGH); }while(0)
#define _neutral(x) do{ pinMode(x, INPUT); }while(0)

#define USE_MONITOR 0

/* BYTE 5*/
#define UD 0x01
#define LD 0x02
#define ZR 0x04
#define BX  0x08
#define BA  0x10
#define BY  0x20
#define BB  0x40
#define ZL 0x80

/* BYTE 4*/
#define RB 0x02
#define START 0x04
#define HOME 0x08
#define SELECT 0x10
#define LB 0x20
#define DD 0x40
#define RD 0x80

class WiiClassicControl
{
  public:
    void begin(byte Vcc = NEUTRAL, byte Gnd = NEUTRAL, TwoWire &theWire = Wire)
    {
      for (byte i = 0; i < 6; i++)
        data[i] = 0x00;

	  WIRE = &theWire;
      WIRE -> begin();
      WIRE -> beginTransmission(0x52);      // transmit to device @ BBAddress 0x52
      WIRE -> write((byte)0x40);           // write memory address
      WIRE -> write((byte)0x00);           // write memory address
      WIRE -> endTransmission();

      SetLeftStick_Factors(8, 8);
      SetRightStick_Factors(8, 8);
	  
	  switch (Vcc)
      {
        case ON: _on(A3); break;
        case NEUTRAL: _neutral(A3); break;
        default: break;
      }
	  
      switch (Gnd)
      {
        case OFF: _off(A2); break;
        default: break;
      }
    }

    void RawData()
    {
      WIRE -> requestFrom (0x52, 6); // request data from controller
      if (WIRE -> available ())
      {
        for (count = 0; count < 6; count++)
        {
          Serial.print(WIRE -> read(), BIN);
          Serial.print("|");
        }
        Serial.println();
        Zero();
      }
    }

    void CollectData()
    {
      count = 0;
      WIRE -> requestFrom (0x52, 6); // request data from controller
      while (WIRE -> available ())
      {
        data[count] = WIRE -> read();
        count++;
      }
      Zero();
    }

    void SetLeftStick_Factors(byte lx, byte ly)
    {
      LX = ((lx < 1) ? 1 : ((lx > 8) ? 8 : lx));
      LY = ((ly < 1) ? 1 : ((ly > 8) ? 8 : ly));
    }

    void SetRightStick_Factors(byte rx, byte ry)
    {
      RX = ((rx < 1) ? 1 : ((rx > 8) ? 8 : rx));
      RY = ((ry < 1) ? 1 : ((ry > 8) ? 8 : ry));
    }

    boolean leftShoulderPressed() {
      return GetButton(4, LB);
    }

    boolean rightShoulderPressed() {
      return GetButton(4, RB);
    }

    boolean lzPressed() {
      return GetButton(5, ZL);
    }

    boolean rzPressed() {
      return GetButton(5, ZR);
    }

    boolean leftDPressed() {
      return GetButton(5, LD);
    }

    boolean rightDPressed() {
      return GetButton(4, RD);
    }

    boolean upDPressed() {
      return GetButton(5, UD);
    }

    boolean downDPressed() {
      return GetButton(4, DD);
    }

    boolean selectPressed() {
      return GetButton(4, SELECT);
    }

    boolean homePressed() {
      return GetButton(4, HOME);
    }

    boolean startPressed() {
      return GetButton(4, START);
    }

    boolean xPressed() {
      return GetButton(5, BX);
    }

    boolean yPressed() {
      return GetButton(5, BY);
    }

    boolean aPressed() {
      return GetButton(5, BA);
    }

    boolean bPressed() {
      return GetButton(5, BB);
    }

    byte leftStickX() {
      return  (GetStick(0, 0x3F) >> 1) * LX;
    }

    byte leftStickY() {
      return  (GetStick(1, 0x3F) >> 1) * LY;
    }

    byte rightStickX() {
      return ((GetStick(0, 0xC0) >> 3) | (GetStick(1, 0xC0) >> 5) | (GetStick(2, 0xC0) >> 7)) * RX;
    }

    byte rightStickY() {
      return GetStick(2, 0x3F) * RY;
    }

#if USE_MONITOR
    void ShowData(byte col)
    {
      Serial.print(data[col], BIN);
      Serial.print(" | ");
    }
#endif

  private:

    boolean GetButton(byte col, byte D)
    {
      return (~data[col] & D);
    }

    int GetStick(byte col, byte mask)
    {
      return (data[col] & mask);
    }

    void Zero()
    {
      WIRE -> beginTransmission(0x52);   // transmit to device 0x52
      WIRE -> write((byte)0x00);           	// writes one byte
      WIRE -> endTransmission();    		// stop transmitting
    }

    byte 	count;
    byte 	data[6];
    byte 	RX, RY, LX, LY;
	TwoWire *WIRE;
};
#endif
