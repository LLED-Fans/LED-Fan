//
// Created by Lukas Tenbrink on 07.07.20.
//

#ifndef LED_FAN_LLSPEEDTEST_H
#define LED_FAN_LLSPEEDTEST_H


#include <stdlib_noniso.h>
#include <Arduino.h>
#include "SPITools.h"


void llSpeedTest(void)
{
    register int i,j;
    volatile unsigned char c1,c2;
    volatile int v;
    volatile long l1,l2;
    volatile float f1,f2;
    int p,q;
    long int r;
    unsigned long m,n;
    float d, overhead;
    char buffer[30];

    Serial.println(F(""));
    Serial.println(F("Speed test"));
    Serial.println(F("----------"));

    Serial.print(F("F_CPU = "));
    Serial.print(F_CPU,DEC);
    Serial.println(F(" Hz"));
    Serial.print(F("1/F_CPU = "));
    Serial.print((1000000.0/(float)F_CPU),4);
    Serial.println(F(" us"));

    delay(800);    // Allow the Serial text to be transmitted

    Serial.print(F("  nop                       : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<100; i++)
    {
        for (j=0; j<10000; j++)
        {
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;           // in micro seconds
    // Calculate overhead with 'nop' instruction per loop in microseconds
    overhead = d - (20.0 * (1000000.0/(float)F_CPU));
    d -= overhead;
    d /= 20.0;             // per instruction
    Serial.print (d,3);
    Serial.println (F(" us"));

    Serial.print(F("  digitalRead               : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<10000; j++)
        {
            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);

            digitalRead(10);
            digitalRead(10);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  digitalWrite              : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<10000; j++)
        {
            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);

            digitalWrite(12, HIGH);
            digitalWrite(12, LOW);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  pinMode                   : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<10000; j++)
        {
            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);

            pinMode(13, INPUT);
            pinMode(13, OUTPUT);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  multiply byte             : "));
    c1 = millis();
    c2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<20; i++)
    {
        for (j=0; j<10000; j++)
        {
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
            c1 *= c2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  divide byte               : "));
    c1 = millis();
    c2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<10000; j++)
        {
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
            c1 /= c2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  add byte                  : "));
    c1 = millis();
    c2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<20; i++)
    {
        for (j=0; j<10000; j++)
        {
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
            c1 += c2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  multiply integer          : "));
    volatile int x,y;
    x = millis();
    y = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<10; i++)
    {
        for (j=0; j<10000; j++)
        {
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
            x *= y;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  divide integer            : "));
    x = millis();
    y = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<10000; j++)
        {
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
            x /= y;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  add integer               : "));
    x = millis();
    y = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<10; i++)
    {
        for (j=0; j<10000; j++)
        {
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
            x += y;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  mod integer               : "));
    x = millis();
    y = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<10; i++)
    {
        for (j=0; j<10000; j++)
        {
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
            x %= y;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  or/and integer            : "));
    x = millis();
    y = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<10; i++)
    {
        for (j=0; j<10000; j++)
        {
            x |= y;
            y &= x;
            x |= y;
            y /= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
            x |= y;
            y &= x;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  shift integer           : "));
    x = millis();
    y = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<10; i++)
    {
        for (j=0; j<10000; j++)
        {
            x <<= y;
            y <<= x;
            x >>= y;
            y >>= x;
            x <<= y;
            y <<= x;
            x >>= y;
            y >>= x;
            x <<= y;
            y <<= x;
            x >>= y;
            y >>= x;
            x <<= y;
            y <<= x;
            x >>= y;
            y >>= x;
            x <<= y;
            y <<= x;
            x >>= y;
            y >>= x;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  multiply long             : "));
    l1 = millis();
    l2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<10000; j++)
        {
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
            l1 *= l2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  divide long               : "));
    l1 = millis();
    l2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<2000; j++)
        {
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
            l1 /= l2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  add long                  : "));
    l1 = millis();
    l2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<4; i++)
    {
        for (j=0; j<10000; j++)
        {
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
            l1 += l2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  multiply float            : "));
    f1 = millis();
    f2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<10000; j++)
        {
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
            f1 *= f2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  divide float              : "));
    f1 = millis();
    f2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<2000; j++)
        {
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
            f1 /= f2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  add float                 : "));
    f1 = millis();
    f2 = micros();
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<10000; j++)
        {
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
            f1 += f2;
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  itoa()                    : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<10000; j++)
        {
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
            itoa(i,buffer,10);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    long int l = 314159L;
    Serial.print(F("  ltoa()                    : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<500; j++)
        {
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
            ltoa(l,buffer,10);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  dtostrf()                 : "));
    float d3;
    d3 = 3.14159265;
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<2; i++)
    {
        for (j=0; j<1000; j++)
        {
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
            dtostrf (d3, 6, 2, buffer);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



//    Serial.print(F("  random()                  : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<2; i++)
//    {
//        for (j=0; j<1000; j++)
//        {
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//            r=random(-2147483647,2147483647);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));



    Serial.print(F("  y |= (1<<x)               : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<20; i++)
    {
        for (j=0; j<10000; j++)
        {
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
            v |= _BV(12);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



    Serial.print(F("  bitSet()                  : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<20; i++)
    {
        for (j=0; j<10000; j++)
        {
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
            bitSet (v, 12);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



//    Serial.print(F("  analogReference()         : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<20; i++)
//    {
//        for (j=0; j<10000; j++)
//        {
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//            analogReference (EXTERNAL);
//            analogReference (DEFAULT);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));


    Serial.print(F("  analogRead()              : "));
    delay(70);     // Allow the Serial text to be transmitted
    m=millis();
    for (i=0; i<1; i++)
    {
        for (j=0; j<1000; j++)
        {
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
            analogRead (0);
            analogRead (1);
        }
    }
    n=millis();
    d = ((float)n - (float)m) / ((float)i * (float)j);
    d *= 1000.0;
    d -= overhead;
    d /= 20.0;
    Serial.print (d,3);
    Serial.println (F(" us"));



//    Serial.print(F("  analogWrite() PWM         : "));
//    // Using pin 13 (system led) for output.
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<1; i++)
//    {
//        for (j=0; j<10000; j++)
//        {
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//            analogWrite (9, 100);
//            analogWrite (9, 200);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));



//    Serial.print(F("  delay(1)                  : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<1; i++)
//    {
//        for (j=0; j<100; j++)
//        {
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//            delay(1);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));
//
//
//
//    Serial.print(F("  delay(100)                : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<1; i++)
//    {
//        for (j=0; j<2; j++)
//        {
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//            delay(100);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));
//
//
//
//    Serial.print(F("  delayMicroseconds(2)      : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<20; i++)
//    {
//        for (j=0; j<10000; j++)
//        {
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//            delayMicroseconds(2);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));
//
//
//
//    Serial.print(F("  delayMicroseconds(5)      : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<5; i++)
//    {
//        for (j=0; j<10000; j++)
//        {
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//            delayMicroseconds(5);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));
//
//
//
//    Serial.print(F("  delayMicroseconds(100)    : "));
//    delay(70);     // Allow the Serial text to be transmitted
//    m=millis();
//    for (i=0; i<1; i++)
//    {
//        for (j=0; j<1000; j++)
//        {
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//            delayMicroseconds(100);
//        }
//    }
//    n=millis();
//    d = ((float)n - (float)m) / ((float)i * (float)j);
//    d *= 1000.0;
//    d -= overhead;
//    d /= 20.0;
//    Serial.print (d,3);
//    Serial.println (F(" us"));


    Serial.println(F("-----------"));
}

void llSpeedTestExecute()
{
    Serial.begin(9600);

    pinMode(9, OUTPUT);
    pinMode(10, INPUT);

    Serial.println("Speed Test will begin momentarily.");
    Serial.println("");

    delay(4000);

    llSpeedTest();
}


#endif //LED_FAN_LLSPEEDTEST_H
