#include <LiquidCrystal.h>
  #include <Adafruit_Fingerprint.h>
  #include <SoftwareSerial.h>

  LiquidCrystal lcd(11, 10, 9, 8, 7, 6);
  SoftwareSerial mySerial(2, 3);

  #define sw1 A0
  #define sw2 A1
  #define sw3 A2
  #define sw4 17
  #define sw5 18

  Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
  int id = 0;
  int vote1 = 0;
  int vote2 = 0;
  int vote3 = 0;
  int vote4 = 0;
  int vote_taken = 0;
  int finger_taken = 0;
  int arr[5] = {0};

  int fingerinput()
  {
    int id;
    lcd.setCursor(0, 0);
    lcd.print("Place Finger");
    id = getFingerprintIDez();
    return id;
    
  }

  void setup()
  {
    pinMode(sw1, INPUT); // setting all the buttons to receive input
    pinMode(sw2, INPUT);
    pinMode(sw3, INPUT);
    pinMode(sw4, INPUT);
    pinMode(sw5, INPUT);
    pinMode(13, OUTPUT); // probably to control the lcd
    pinMode(12, OUTPUT); // led output

    Serial.begin(9600);
    while (!Serial)
      ; // For Yun/Leo/Micro/Zero/...
    delay(100);
    Serial.println("\n\nAdafruit finger detect test");

    finger.begin(57600);

    if (finger.verifyPassword())
    {
      Serial.println("Found fingerprint sensor!");
    }
    else
    {
      Serial.println("Did not find fingerprint sensor :(");
      while (1)
      {
        delay(1);
      }
    }

    finger.getTemplateCount();
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
    Serial.println("Waiting for valid finger...");

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    delay(3000);
    lcd.print(" VOTING MACHINE ");
    lcd.setCursor(0, 1);
    lcd.print("  LCD DISPLAY");
    delay(3000);
    digitalWrite(sw1, HIGH); // ensuring all buttons are initialised to high state
    digitalWrite(sw2, HIGH);
    digitalWrite(sw3, HIGH);
    digitalWrite(sw4, HIGH);
    digitalWrite(sw5, HIGH);
    lcd.clear();
  }

  void loop()
  {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" A");
    lcd.setCursor(1, 1);
    lcd.print(vote1);
    lcd.setCursor(4, 0);
    lcd.print(" B");
    lcd.setCursor(5, 1);
    lcd.print(vote2);
    lcd.setCursor(8, 0);
    lcd.print(" C");
    lcd.setCursor(9, 1);
    lcd.print(vote3);
    lcd.setCursor(12, 0);
    lcd.print(" D");
    lcd.setCursor(13, 1);
    lcd.print(vote4);
    lcd.clear();

    vote_taken = 0;
    do
    {
      id = fingerinput();
      Serial.println(id);

      if(id==-1)
      {
        Serial.println("im here");
        lcd.clear();
        delay(1000);
        lcd.setCursor(0,0);
        lcd.print("Match Not");
        lcd.setCursor(0,1);
        lcd.print("Found!!");
        delay(1000);
        lcd.clear();
        continue;
      }
    } while (!id);

    lcd.clear();

    if (id > 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Your Voter ID ");
      lcd.print(id);
      lcd.setCursor(0, 1);
      lcd.print("Pls Vote");
      delay(2000);

      do
      {

        if (digitalRead(sw1) == 0)
        {
          if(arr[id - 1] == 0)
          {
            digitalWrite(12, HIGH);
            vote1++;
            while (digitalRead(sw1) == 0)
              ;
            digitalWrite(12, LOW);
            delay(1000);
            vote_taken = 1;
            arr[id - 1]++;
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Invalid");
            lcd.setCursor(0,1);
            lcd.print("Fingerprint");
            delay(2000);
            lcd.clear();
            vote_taken=1;
          }
          
        }


        if (digitalRead(sw2) == 0 )
        {
          if(arr[id - 1] == 0)
          {
            vote2++;
            digitalWrite(12, HIGH);
            while (digitalRead(sw2) == 0)
              ;
            digitalWrite(12, LOW);
            delay(1000);
            vote_taken = 1;
            arr[id - 1]++;

          }
          else
          {
            lcd.clear();
            lcd.print("Invalid");
            lcd.setCursor(0,1);
            lcd.print("Fingerprint");
            delay(2000);
            lcd.clear();
            vote_taken=1;
          }
        }

        if (digitalRead(sw3) == 0 )
        {
          if(arr[id - 1] == 0)
          {
            digitalWrite(12, HIGH);
            vote3++;
            while (digitalRead(sw3) == 0)
              ;
            digitalWrite(12, LOW);
            delay(1000);
            vote_taken = 1;
            arr[id - 1]++;
          }
          else
          {
            lcd.clear();
            lcd.print("Invalid");
            lcd.setCursor(0,1);
            lcd.print("Fingerprint");
            delay(2000);
            lcd.clear();
            vote_taken=1;
          }

        }
        if (digitalRead(sw4) == 0 )
        {
          if (arr[id - 1] == 0)
          {
            digitalWrite(12, HIGH);
            vote4++;
            while (digitalRead(sw4) == 0)
              ;
            digitalWrite(12, LOW);
            delay(1000);
            vote_taken = 1;
            arr[id - 1]++;
          }
          else
          {
            lcd.clear();
            lcd.print("Invalid");
            lcd.setCursor(0,1);
            lcd.print("Fingerprint");
            delay(2000);
            lcd.clear();
            vote_taken=1;
          }
        }

        if (digitalRead(sw5) == 0)
        {
          digitalWrite(13, HIGH);
          delay(2000);
          digitalWrite(13, LOW);
          int vote = vote1 + vote2 + vote3 + vote4;
          if (vote)
          {
            if ((vote1 > vote2 && vote1 > vote3 && vote1 > vote4))
            {
              lcd.clear();
              lcd.print("A Wins");
              delay(5000);
              lcd.clear();
            }
            else if ((vote2 > vote1 && vote2 > vote3 && vote2 > vote4))
            {
              lcd.clear();
              lcd.print("B Wins");
              delay(5000);
              lcd.clear();
            }
            else if ((vote3 > vote1 && vote3 > vote2 && vote3 > vote4))
            {
              lcd.clear();
              lcd.print("C Wins");
              delay(5000);
              lcd.clear();
            }
            else if (vote4 > vote1 && vote4 > vote2 && vote4 > vote3)
            {
              lcd.setCursor(0, 0);
              lcd.clear();
              lcd.print("D Wins");
              delay(5000);
              lcd.clear();
            }

            else
            {
              lcd.clear();
              lcd.print(" Tie Up Or ");
              lcd.setCursor(0, 1);
              lcd.print(" No Result ");
              delay(5000);
              lcd.clear();
            }
          }
          else
          {
            lcd.clear();
            lcd.print("No Voting...");
            delay(5000);
            lcd.clear();
          }
          vote1 = 0;
          vote2 = 0;
          vote3 = 0;
          vote4 = 0, vote = 0;
          lcd.clear();
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          vote_taken = 1;
        }

      } while (vote_taken == 0);

      lcd.setCursor(0, 0);
      lcd.print(" A");
      lcd.setCursor(1, 1);
      lcd.print(vote1);
      lcd.setCursor(4, 0);
      lcd.print(" B");
      lcd.setCursor(5, 1);
      lcd.print(vote2);
      lcd.setCursor(8, 0);
      lcd.print(" C");
      lcd.setCursor(9, 1);
      lcd.print(vote3);
      lcd.setCursor(12, 0);
      lcd.print(" D");
      lcd.setCursor(13, 1);
      lcd.print(vote4);
    }
    // else
    // {
    //   lcd.clear();
    //   lcd.print("Match not");
    //   lcd.setCursor(0,1);
    //   lcd.print("Found!!");
    //   delay(2000);
    //   lcd.clear();

    // }
  }

  uint8_t getFingerprintID()
  {
    uint8_t p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
    }

    // OK success!

    p = finger.image2Tz();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
    }

    // OK converted!
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK)
    {
      Serial.println("Found a print match!");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
      Serial.println("Communication error");
      return p;
    }
    else if (p == FINGERPRINT_NOTFOUND)
    {
      Serial.println("Did not find a match");
      return p;
    }
    else
    {
      Serial.println("Unknown error");
      return p;
    }

    // found a match!
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of ");
    Serial.println(finger.confidence);

    return finger.fingerID;
  }

  // returns -1 if failed, otherwise returns ID #
  int getFingerprintIDez()
  {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
      return -3;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
      return -3;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
      return -1;

    // found a match!
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of ");
    Serial.println(finger.confidence);
    // lcd.clear();
    return finger.fingerID;
  }