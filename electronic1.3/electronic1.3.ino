#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
//constant pour LCD (vien avec le code sur le site du manifacturier)
#define STARTUP_DELAY 500
#define RS232_DELAY 100
#define I2C_DELAY 100
#define SLAVE_ADDRESS 0x28
//nos constante
#define MAX_TOTAL 15
#define button1PinIn 12
#define button2PinIn 27
#define button3PinIn 33
#define voiturePinOut 18

unsigned char equation[10];
//génère l'équation selon le MAX_TOTAL qui est le total maximal que l'équation peut avoir
void GeneratorEquation(unsigned char* equation, int repArray[3]) {

    int premierChiffre = random(1,MAX_TOTAL + 1);
    int rep;
    int rep2;
    int rep3;
    switch (random(0,2)) //determine si l'équation est un soustraction ou un addition
    { 
        case 0: { //addition
            int tempMax = MAX_TOTAL - premierChiffre;
            int deuxiemeChiffre = random(1, tempMax +1);
            sprintf((char*)equation, "%d + %d", premierChiffre, deuxiemeChiffre);
            rep = premierChiffre + deuxiemeChiffre;
            break;
        }
        case 1: { //soustraction
            int tempMax_ = premierChiffre; 
            int deuxiemeChiffre_ = random(1,tempMax_ + 1);
            sprintf((char*)equation, "%d - %d", premierChiffre, deuxiemeChiffre_);
            rep = premierChiffre - deuxiemeChiffre_;
            break;
        }
  }
        //génère les reponses et les place dans un array
        rep2 = random(rep - 3, rep + 4);
        while(rep == rep2 || rep2 < 0)
        {
          rep2 = random(rep - 3, rep + 4);
        }
        rep3 = random(rep - 3, rep + 4);
        while(rep == rep3 || rep2 == rep3 || rep3 < 0)
        {
          rep3 = random(rep - 3, rep + 4);
        }
        repArray[0] = rep;
        repArray[1] = rep2;
        repArray[2] = rep3;
    
}
//change les réponses de place pour empêcher que la bonne réponse soit toujours sur le même bouton
void shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = random(0,i); // Random index between 0 and i
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
//affiche l'équation et gère la réponse
void Main() {
  clearScreen();
  unsigned char equation[20];
  int arrayReponse[3];
  GeneratorEquation(equation,arrayReponse);
  int bonneReponse = arrayReponse[0];
  shuffle(arrayReponse,3);
  setCursor(0x07);
  writeString((unsigned char*)equation);
  unsigned char rep1[2];
  sprintf((char*)rep1, "%d", arrayReponse[0]);
  setCursor(0x17);
  writeString((unsigned char*)"A:");
  writeString((unsigned char*)rep1);
  unsigned char rep2[2];
  sprintf((char*)rep2, "%d", arrayReponse[1]);
  setCursor(0x1c);
  writeString((unsigned char*)"B:");
  writeString((unsigned char*)rep2);
  unsigned char rep3[2];
  sprintf((char*)rep3, "%d", arrayReponse[2]);
  setCursor(0x21);
  writeString((unsigned char*)"C:");
  writeString((unsigned char*)rep3);
  bool estBonneReponse = false;
  bool reponseEstPasChoisi = true;
  while(reponseEstPasChoisi)
  {
    int btnState1 = digitalRead(button1PinIn);
    int btnState2 = digitalRead(button2PinIn);
    int btnState3 = digitalRead(button3PinIn);
    if(btnState1 == LOW){
      if(arrayReponse[0] == bonneReponse){
        estBonneReponse = true;
      }
      reponseEstPasChoisi = false;
    }
    if(btnState2 == LOW){
      if(arrayReponse[1] == bonneReponse){
        estBonneReponse = true;
      }
      reponseEstPasChoisi = false;
    }
    if(btnState3 == LOW){
      if(arrayReponse[2] == bonneReponse){
        estBonneReponse = true;
      }
      reponseEstPasChoisi = false;
    }
  }
  clearScreen();
  if(estBonneReponse){
    setCursor(0x43);
    writeString((unsigned char*)"Bonne Reponse!");
    clearScreen();
    setCursor(0x49);
    writeString((unsigned char*)"3");
    delay(1000);
    clearScreen();
    setCursor(0x49);
    writeString((unsigned char*)"2");
    delay(1000);
    clearScreen();
    setCursor(0x49);
    writeString((unsigned char*)"1");
    delay(1000);
    clearScreen();
    setCursor(0x46);
    writeString((unsigned char*)"GO!!!!!");
    digitalWrite(voiturePinOut,HIGH);
    delay(5000);
    digitalWrite(voiturePinOut,LOW);
  }
  else{
    setCursor(0x42);
    writeString((unsigned char*)"Mauvaise Reponse");
  }
  delay(1500);
} 

void setup(){
  Serial.begin(9600);
  initLCD_SPI(22, 23, 21);
  pinMode(button1PinIn, INPUT_PULLUP);
  pinMode(button2PinIn, INPUT_PULLUP);
  pinMode(button3PinIn, INPUT_PULLUP);
  pinMode(voiturePinOut, OUTPUT);
}

void loop() {
  Main();
}

// SPI Interface
uint8_t _SCL; // 5
uint8_t _SDI; // 4
uint8_t _CS; // 3

// RS232 Interface
uint8_t _TX; // 2

//I2C Interface
uint8_t _SDA; // 4

enum Interface{
  I2C,
  SPI,
  RS232
};

Interface _interface;

/**
 * @brief Initialize selected IO ports for I2C.
 * 
 * @param SCL Serial clock pin assigment.
 * @param SDA Serial data pin assignment.
 * @return none
 */
void initLCD_I2C(uint8_t SCL, uint8_t SDA)
{
  _interface = I2C;

  // Store pin assigmnents globally
  _SCL = SCL;
  _SDA = SDA;

  // Set IO modes
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT);
  
  // Set starting pin states
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);
  
  // Wait for display to power ON
  delay(STARTUP_DELAY);
  clearScreen();
}

/**
 * @brief Initialize selected IO ports for SPI
 * 
 * @param SCL Serial clock pin assignment.
 * @param SDI Serial data pin assignment.
 * @param CS Chip/Slave select pin assignment.
 * @return none
 */
void initLCD_SPI(uint8_t SCL, uint8_t SDI, uint8_t CS)
{
  _interface = SPI;

  // Store pin assignments globally
  _SCL = SCL;
  _SDI = SDI;
  _CS = CS;

  // Set IO modes
  pinMode(CS, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(SDI, OUTPUT);

  // Set pin states
  digitalWrite(CS, HIGH);
  digitalWrite(SCL, HIGH);

  // Wait for display to power ON
  delay(STARTUP_DELAY);
  clearScreen();
}

/**
 * @brief Initalize selected IO ports for RS232.
 * 
 * @param TX Data transmit pin assignment.
 * @return none
 */
void initLCD_RS232(uint8_t TX)
{
  _interface = RS232;

  // Store pin assignments globally
  _TX = TX;

  // Set IO modes
  pinMode(TX, OUTPUT);
  digitalWrite(TX, HIGH);

  // Wait for display to power ON
  delay(STARTUP_DELAY);
  clearScreen();
}

/**
 * @brief Set chip/slave select HIGH and wait for 1ms.
 * 
 * @return none
 */
void setCS()
{
  digitalWrite(_CS, HIGH);
  delay(1);
}

/**
 * @brief Clear chip/slave select and wait for 1ms.
 * 
 * @return none
 */
void clearCS()
{
  digitalWrite(_CS, LOW);
  delay(1);
}

/**
 * @brief Clear the RX pin on the RS232 bus.
 * 
 * @return none
 */
void startBit()
{
  digitalWrite(_TX, LOW);
  delayMicroseconds(RS232_DELAY);
}

/**
 * @brief Set the RX pin on the RS232 bus.
 * 
 * @return none
 */
void stopBit()
{
  digitalWrite(_TX, HIGH);
  delayMicroseconds(RS232_DELAY);
}

/**
 * @brief Send a start condition on the I2C bus.
 * 
 * @return none
 */
void startCondition()
{
  clearSDA();
  clearSCL();
}

/**
 * @brief Send a stop condition on the I2C bus.
 * 
 * @return none
 */
void stopCondition()
{
  setSCL();
  setSDA();
}

/**
 * @brief Set the SDA/SDI pin high on the I2C/SPI bus.
 * 
 * @return none
 */
void setSDA()
{
  digitalWrite(_SDA, HIGH);
  delayMicroseconds(I2C_DELAY);
}

/**
 * @brief Clear the SDA/SDI pin on the I2C/SPI bus.
 * 
 * @return none
 */
void clearSDA()
{
  digitalWrite(_SDA, LOW);
  delayMicroseconds(I2C_DELAY);
}

/**
 * @brief Set the SCL/SCK pin on the I2C/SPI bus.
 * 
 * @return none
 */
void setSCL()
{
  digitalWrite(_SCL, HIGH);
  if(_interface == I2C)
  {
    delayMicroseconds(I2C_DELAY);
  }
}

/**
 * @brief Clear the SCL/SCK pin on the I2C/SPI bus.
 * 
 * @return none
 */
void clearSCL()
{
  digitalWrite(_SCL, LOW);
  if(_interface == I2C)
  {
    delayMicroseconds(I2C_DELAY);
  }
}

/**
 * @brief Set the I2C bus to write mode.
 * 
 * @return none
 */
void setWriteMode()
{
  putData_I2C((SLAVE_ADDRESS << 1) | 0x00);
}

/**
 * @brief Set the I2C bus to read mode.
 * 
 * @return none
 */
void setReadMode()
{
  putData_I2C((SLAVE_ADDRESS << 1) | 0x01);
}

/**
 * @brief Check if an ACK/NACK was received on the I2C bus.
 * 
 * @return uint8_t The ACK/NACK read from the display.
 */
uint8_t getACK()
{
  pinMode(_SDA, INPUT);
  setSCL();

  uint8_t ACK = digitalRead(_SDA);

  pinMode(_SDA, OUTPUT);
  clearSCL();

  return ACK;
}

/**
 * @brief Write 1 byte of data to the display.
 * 
 * @param data Byte of data to be written.
 * @return none
 */
void write(uint8_t data)
{
  switch(_interface)
  {
    case I2C:
      startCondition();
      setWriteMode();
      putData_I2C(data);
      stopCondition();
      break;
    case SPI:
      clearCS();
      putData_SPI(data);
      setCS();
      break;
    case RS232:
      startBit();
      putData_RS232(data);
      stopBit();
      break;
    default:
      break;
  }
  delayMicroseconds(150);
}

/**
 * @brief Write an array of characters to the display.
 * 
 * @param data Pointer to the array of characters.
 * @return none
 */
void writeString(unsigned char* data)
{
  // Iterate through data until null terminator is found.
  while(*data != '\0')
  {
    write(*data);
    data++; // Increment pointer.
  }
}

/**
 * @brief Clock each bit of data on the I2C bus and read ACK.
 * 
 * @param data Byte of data to be put on the I2C data bus.
 * @return none
 */
void putData_I2C(uint8_t data)
{
  for(int i = 7; i >= 0; i--)
  {
    digitalWrite(_SDA, (data >> i) & 0x01);

    setSCL();
    clearSCL();
  }

  getACK();
}

/**
 * @brief Put each bit of data on the SPI data bus.
 * This function sends MSB (D7) first and LSB (D0) last.
 * 
 * @param data Byte of data to be put on the SPI data bus.
 * @return none
 */
void putData_SPI(uint8_t data)
{
  // Write data byte MSB first -> LSB last
  for(int i = 7; i >= 0; i--)
  {
    clearSCL();

    digitalWrite(_SDI, (data >> i) & 0x01);
    
    setSCL();
  }
}

/**
 * @brief Put each bit of data on the RS232 data bus.
 * This function sends LSB (D0) first and MSB (D7) last.
 * 
 * @param data Byte of data to be put on the RS232 data bus.
 * @return none
 */
void putData_RS232(uint8_t data)
{
  // Write data byte LSB first -> MSB last
  for(int i = 0; i <= 7; i++)
  {
    digitalWrite(_TX, (data >> i) & 0x01);
    delayMicroseconds(RS232_DELAY);
  }
}

/**
 * @brief Send the prefix data byte (0xFE).
 * 
 * @return none
 */
void prefix()
{
  write(0xFE);
}

/**
 * @brief Turn the display ON.
 * Display is turned ON by default.
 * 
 * @return none
 */
void displayON()
{
  prefix();
  write(0x41);
}

/**
 * @brief Turn the display OFF.
 * Display is turned ON by default.
 * 
 * @return none
 */
void displayOFF()
{
  prefix();
  write(0x42);
}

/**
 * @brief Set the display cursor position via DDRAM address.
 * 
 * @param position Desired DDRAM address.
 * @return none
 */
void setCursor(uint8_t position)
{
  prefix();
  write(0x45);
  write(position);
}

/**
 * @brief Move the cursor to line 1, column 1.
 * 
 * @return none
 */
void home()
{
  prefix();
  write(0x46);
}

/**
 * @brief Clear the display screen.
 * 
 * @return none
 */
void clearScreen()
{
  prefix();
  write(0x51);
  delay(2);
}

/**
 * @brief Set the display's contrast.
 * 0x00 <= contrast <= 0x32
 * Default: 0x28
 * 
 * @param contrast Desired contrast setting.
 * @return none 
 */
void setContrast(uint8_t contrast)
{
  prefix();
  write(0x52);
  write(contrast);
}

/**
 * @brief Set the display's brightness.
 * 0x01 <= brightness <= 0x08
 * brightness = 0x01 | Backlight OFF
 * brightness = 0x08 | Backlight ON (100%)
 * 
 * @param brightness Desired brightness setting.
 * @return none
 */
void setBrightness(uint8_t brightness)
{
  prefix();
  write(0x53);
  write(brightness);
}

/**
 * @brief Turn the underline cursor ON.
 * 
 * @return none
 */
void underlineCursorON()
{
  prefix();
  write(0x47);
}

