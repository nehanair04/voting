#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Initialize the fingerprint sensor
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Initialize the LCD
LiquidCrystal lcd(11,10,9,8,7,6);

// Define the voting buttons
#define sw1 A0
#define sw2 A1
#define sw3 A2
#define sw4 17
#define sw5 18

// Initialize vote counters
int vote1=0;
int vote2=0;
int vote3=0;
int vote4=0;

void setup() {
 // Initialize the LCD and buttons
 lcd.begin(16, 2);
 pinMode(sw1, INPUT);
 pinMode(sw2, INPUT);
 pinMode(sw3, INPUT);
 pinMode(sw4, INPUT);
 pinMode(sw5, INPUT);
 pinMode(13, OUTPUT);
 pinMode(12, OUTPUT);

 // Initialize the fingerprint sensor
 finger.begin(57600);
 if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor ready");
 } else {
    Serial.println("Fingerprint sensor not found");
    while (1);
 }
}

void loop() {
 // Check if the fingerprint is enrolled
 uint8_t id = getFingerprintID();
 if (id == 0) {
    // Fingerprint not found, prompt to enroll
    Serial.println("Please enroll your fingerprint.");
    enrollFingerprint(); // Call the enrollment function
 } else {
    // Fingerprint found, proceed to voting
    Serial.println("Fingerprint recognized. Proceed to vote.");
    votingSystem(); // Call the voting function
 }
}

uint8_t getFingerprintID() {
 uint8_t p = finger.getImage();
 switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
 }

 // OK success!
 p = finger.image2Tz();
 switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
 }

 // OK converted!
 p = finger.createModel();
 if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
 } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return 0;
 } else {
    Serial.println("Unknown error");
    return 0;
 }

 // OK we have a model
 p = finger.storeModel(1);
 if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
 } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return 0;
 } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return 0;
 } else {
    Serial.println("Unknown error");
    return 0;
 }
 return 1; // Return the ID of the fingerprint
}


void enrollFingerprint() {
 Serial.println("Ready to enroll a fingerprint!");
 Serial.println("Please type in the ID # (from 1 to 127) you'd like to save this fingerprint as...");
 uint8_t id = 0;
 while (true) {
    while (! Serial.available());
    char c = Serial.read();
    if (! isdigit(c)) break;
    id *= 10;
    id += c - '0';
 }
 Serial.print("Enrolling ID #");
 Serial.println(id);

 while (! getFingerprintEnroll(id) );
}

uint8_t getFingerprintEnroll(uint8_t id) {
 uint8_t p = -1;
 Serial.print("Waiting valid finger to enroll as #"); Serial.println(id);
 while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
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
 }

 // OK success!
 p = finger.image2Tz(1);
 switch (p) {
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

 Serial.println("Remove finger");
 delay(2000);
 p = 0;
 while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
 }
 Serial.print("ID "); Serial.println(id);
 p = -1;
 Serial.println("Place same finger again");
 while (p != FINGERPRINT_OK) {
    p = finger.getImage();
 }

 // OK success!
 p = finger.image2Tz(2);
 switch (p) {
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
 Serial.print("Creating model for #"); Serial.println(id);
 p = finger.createModel();
 if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
 } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
 } else {
    Serial.println("Unknown error");
    return p;
 }

 Serial.print("Storing model #"); Serial.println(id);
 p = finger.storeModel(id);
 if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
 } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
 } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
 } else {
    Serial.println("Unknown error");
    return p;
 }
 return FINGERPRINT_OK;
}


void votingSystem() {
 // Display current votes on the LCD
 lcd.setCursor(0,0);
 lcd.print(" A");
 lcd.setCursor(1,1);
 lcd.print(vote1);
 lcd.setCursor(4,0);
 lcd.print(" B");
 lcd.setCursor(5,1);
 lcd.print(vote2);
 lcd.setCursor(8,0);
 lcd.print(" C");
 lcd.setCursor(9,1);
 lcd.print(vote3);
 lcd.setCursor(12,0);
 lcd.print(" D");
 lcd.setCursor(13,1);
 lcd.print(vote4);

 // Check for button presses and increment votes accordingly
 if(digitalRead(sw1)==0) {
    digitalWrite(12,HIGH);
    vote1++;
    while(digitalRead(sw1)==0); // Wait until button is released
    digitalWrite(12,LOW);
    delay(1000);
 }

 if(digitalRead(sw2)==0) {
    vote2++;
    digitalWrite(12,HIGH);
    while(digitalRead(sw2)==0);
    digitalWrite(12,LOW);
    delay(1000);
 }

 if(digitalRead(sw3)==0) {
    digitalWrite(12,HIGH);
    vote3++;
    while(digitalRead(sw3)==0);
    digitalWrite(12,LOW);
    delay(1000);
 }

 if(digitalRead(sw4)==0) {
    digitalWrite(12,HIGH);
    vote4++;
    while(digitalRead(sw4)==0);
    digitalWrite(12,LOW);
    delay(1000);
 }

 // Check for the final button press to determine the winner
 if(digitalRead(sw5)==0) {
    digitalWrite(13,HIGH); // Indicate final button press
    int totalVotes = vote1 + vote2 + vote3 + vote4;
    if(totalVotes > 0) {
      if(vote1 > vote2 && vote1 > vote3 && vote1 > vote4) {
        lcd.clear();
        lcd.print("A Wins");
        delay(5000);
        lcd.clear();
      } else if(vote2 > vote1 && vote2 > vote3 && vote2 > vote4) {
        lcd.clear();
        lcd.print("B Wins");
        delay(5000);
        lcd.clear();
      } else if(vote3 > vote1 && vote3 > vote2 && vote3 > vote4) {
        lcd.clear();
        lcd.print("C Wins");
        delay(5000);
        lcd.clear();
      } else if(vote4 > vote1 && vote4 > vote2 && vote4 > vote3) {
        lcd.clear();
        lcd.print("D Wins");
        delay(5000);
        lcd.clear();
      } else {
        lcd.clear();
        lcd.print(" Tie Up Or ");
        lcd.setCursor(0,1);
        lcd.print(" No Result ");
        delay(5000);
        lcd.clear();
      }
    } else {
      lcd.clear();
      lcd.print("No Voting….");
      delay(5000);
      lcd.clear();
    }
    // Reset votes and display
    vote1=0; vote2=0; vote3=0; vote4=0;
    lcd.clear();
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
 }
}
