#include <SPI.h>
#include <MFRC522.h>
/* RFID NFC */


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
/* LCD I2C */
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


/*Using Hardware SPI of Arduino */
/*MOSI (11), MISO (12) and SCK (13) are fixed */
/*You can configure SS and RST Pins*/
#define SS_PIN 10  /* Slave Select Pin */
#define RST_PIN 9  /* Reset Pin */
int buzzer=4;
int ledY=2;
int ledG=3;

/* Create an instance of MFRC522 */
MFRC522 mfrc522(SS_PIN, RST_PIN);
/* Create an instance of MIFARE_Key */
MFRC522::MIFARE_Key key;

/* Set the block to which we want to write data */
/* Be aware of Sector Trailer Blocks */
int blockNum = 2;
/* Create an array of 16 Bytes and fill it with data */
/* This is the actual data which is going to be written into the card */

/* Create another array to read data from Block */
/* Legthn of buffer should be 2 Bytes more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];

MFRC522::StatusCode status;

void setup()
{
  pinMode(buzzer,OUTPUT);
  pinMode(ledY,OUTPUT);
  pinMode(ledG,OUTPUT);
  
  /* Initialize LCD */
 lcd.init(); // initialize the lcd 
  /* Initialize SPI bus */
  SPI.begin();
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();

 /* LCD Displaying "Scan Your Card" */
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Ready !");
  lcd.setCursor(0,1);
  lcd.print("Scan your Card :)");  
}

void loop()
{
  /* Prepare the ksy for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    
    return;
  }

  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }



   ReadDataFromBlock(blockNum, readBlockData);
     /*---------------------------------------- */
  tone(buzzer, 60, 80);
  delay(100);
  
  digitalWrite(ledY, HIGH); 
  delay(100);
  digitalWrite(ledY, LOW);  
  
   tone(buzzer, 60, 80);
   delay(100);
   
   digitalWrite(ledY, HIGH); 
  delay(100);
  digitalWrite(ledY, LOW); 
  
    digitalWrite(ledG, HIGH); 
  delay(600);
  digitalWrite(ledG, LOW);
  /*---------------------------------------- */

  /*Read data from the same block */
 
  lcd.setCursor(3,0);
  lcd.print("Card Detected");

  delay(450);  // Delay for 450 miliseconds 

  lcd.clear();  // Clear the screen after the delay

  //-------------------------------------------------------
  /* Serial.print("\n");
     Serial.println("**Card Detected**");
     // Print UID of the Card 
     Serial.print(F("Card UID:"));
     */
    

    /* 
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
        Serial.print("\n");
     */  
 //-------------------------------------------------------


  /*LCD Displays Read data from the same block */

  lcd.setCursor(0,0);
  lcd.print("Reading Data...");
  delay(450);  // Delay for 450 miliseconds 
  lcd.clear();  // Clear the screen after the delay

   ReadPrint();


  /* If you want to print the full memory dump, uncomment the next line */
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  /* Print the data read from block */

  lcd.setCursor(3,0);
  lcd.print(" --> ");
  lcd.setCursor(0,1);
  
  for (int j = 0 ; j < 16 ; j++)
  {
    lcd.write(readBlockData[j]);
     /* lcd.write(readBlockData[j]); */
  }
   /*
 delay(5000);  // Delay for 5000 miliseconds 
  lcd.clear();  // Clear the screen after the delay
 */
 
}



void ReadPrint(void)
{
  if (status != MFRC522::STATUS_OK)
  {

    lcd.setCursor(0,0);
    lcd.print("Acces Failed");

    lcd.setCursor(1,0);
    lcd.print(mfrc522.GetStatusCodeName(status));
    
    delay(450);  // Delay for 450 miliseconds 
    lcd.clear();  // Clear the screen after the delay

    return;
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Acces Succeful");
     delay(450);  // Delay for 450 miliseconds 
    lcd.clear();  // Clear the screen after the delay
  }
 }



void ReadDataFromBlock(int blockNum, byte readBlockData[])
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  
  /* Reading data from the Block */
   status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  /*  
 
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");
  }
  */

}
