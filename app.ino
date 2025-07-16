#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>

// WiFi configs
const char* ssid = ""; // Network SSID
const char* password = ""; // Network password

// PcStatus
volatile byte pcStatus =  HIGH;

// Pins Configurations
int relay = 19;
int errorLedPin = 5;
int communicatorLed = 4;

// TELEGRAM - Configs;
#define BOTtoken ""  // your Bot Token (Get from Botfather)
#define CHAT_ID ""

// OLED - Screen Constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Servername
// Não coloquei um end point específico ainda, apenas para teste.
String serverName = "lab.felipeliandro.com.br";

void startUpCheck() {
  HTTPClient http;
  http.begin(serverName);
  int httpResponseCode = http.GET();
  if (httpResponseCode != 200) {
    pcStatus = LOW;
  }
}

void DISPLAY_defaultMessage () {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Welcome Mr.Felipe"));
  if (pcStatus == HIGH) {
    display.println(F("Status do pc:"));
    display.println(F("Ligado!"));
  }
  if (pcStatus == LOW) {
    display.println(F("Status do pc:"));
    display.println(F("Desligado!"));
  }
  display.display();
}

void DISPLAY_errorMessage (const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("ERRO - Mensagem de erro: "));
  display.println(F(message));
  display.display();

  delay(5000);

  DISPLAY_defaultMessage();  
}

void DISPLAY_succesMessage (const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("SUCESSO - Mensagem de log: "));
  display.println(F(message));
  display.display();

  delay(5000);

  DISPLAY_defaultMessage();  
}

void handleRelay () {
  digitalWrite(relay, LOW); // LOW DEFAULT LOW;
  delay(1500);
  digitalWrite(relay, HIGH);
}


void handleLed(int led, int blinkTimes, int delayTime) {
  // blinkTimes - how many times, for the led blink;
  // delayTime - how much time for the next delay in blink, default is 1s
  if (delayTime == NULL) {
    delayTime = 1000;
  }

  for (int blinkTime = 0; blinkTime < blinkTimes; blinkTime++) {
    digitalWrite(led, HIGH);
    delay(delayTime);
    digitalWrite(led, LOW);
    delay(delayTime);
  }

}
void informationLed(int type) {
  // Communication types:
  // 0 - WakeUp: blink 3 times in 1sec.
  // 1 - ShutDown: blink 2 times in 1seg.
  // 2 - Command success: blink 4times in 500ms
  switch (type) {
    case 0:
      handleLed(communicatorLed, 3, 1000);
      Serial.println("Success wake Up.");
      DISPLAY_succesMessage("Success wake up!");
      break;
    
    case 1:
      handleLed(communicatorLed, 2, 1000);
      Serial.println("Sucess to shut down!");
      DISPLAY_succesMessage("Success to shutdown!");
      break;

    case 2:
      handleLed(communicatorLed, 4, 50);
      Serial.println("Command success.");
      DISPLAY_succesMessage("Command success!");
      break;
    
  }
}

void errorLed (int type) {
  // Erros type:
  // 0 - Communication error can't connect to the pin or board;
  // 1 - Input error, example: try to wake up pc if is already turned on;

  switch (type) {
    case 0:
      handleLed(errorLedPin, 2, 1000);
      Serial.println("Error to communicate.");
      DISPLAY_errorMessage("Error to comunicate to the board...");
      break;
    case 1:
      handleLed(errorLedPin, 1, 50);
      Serial.println("Input error...");
      DISPLAY_errorMessage("Input error!");
      break;
  }
}

bool rebootComputer() {
  handleRelay();
  delay(7000);
  handleRelay();
  return true;
}

bool handleComputer (int state) {
  if (pcStatus == state) {
    Serial.println("Server is already running.");
    errorLed(1);
    return false;
  }

  if (state == HIGH) {      
    // Turns on the PC.
    handleRelay();
    Serial.println("Ligando o PC");
    pcStatus = state;
    informationLed(0);
    return true;
  } else {
    // Truns off the PC.
    handleRelay();
    Serial.println("Desligando o PC");
    pcStatus = state;
    informationLed(1);
    return true;
  }
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    // Indicates the status of arudino.
    if (text == "/eSt") {
      String boardRunningStatus = "EMK.32 - Is Running";
      bot.sendMessage(chat_id, boardRunningStatus);
    }

    if (text == "/std") {
      bool commandRes = handleComputer(LOW);
      if (commandRes) {
        bot.sendMessage(chat_id, "Succes! Computer turned off.");
      } else {
        bot.sendMessage(chat_id, "ERROR: Can't turned off the computer, try again later.");
      }
    }

    if (text == "rbt") {
      bool commandRes = rebootComputer();
      if (commandRes) {
        bot.sendMessage(chat_id, "Server rebooted!");
      } else {
        bot.sendMessage(chat_id, "Can't reboot the server.");
      }
    }

    if (text == "/pwp") {
      bool commandRes = handleComputer(HIGH);
      if (commandRes) {
        bot.sendMessage(chat_id, "Succes! Computer turned on.");
      } else {
        bot.sendMessage(chat_id, "ERROR: Can't turned on the computer, try again later.");
      }
    
    }

    if (text == "/pcState") {
      if (pcStatus == HIGH) {
        bot.sendMessage(chat_id, "Server is Running.");
      } else {
        bot.sendMessage(chat_id, "Server is closed.");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(errorLedPin, OUTPUT);
  pinMode(communicatorLed, OUTPUT);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif

  // Start the OLED monitor.
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Error to initialize the oled display.");
    while (true);
  }



  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  delay(5000);
  startUpCheck();
  DISPLAY_defaultMessage();
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
