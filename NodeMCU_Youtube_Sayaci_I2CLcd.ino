/* DR.TRonik tarafından I2C 2*16 Lcd için YouTube Takipçi Sayacı olarak modifiye edilmiştir...
 * Açık kaynak lisansı altında kaynak belirtilerek kopyalanabilir, dağıtılabilir, kişisel olarak kullanılabilir...
 */

// ----------------------------
// Standard Libraries
// ----------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int takipci;


// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <YoutubeApi.h>
// Library for connecting to the Youtube API

// Search for "youtube" in the Arduino Library Manager
// https://github.com/witnessmenow/arduino-youtube-api

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

//------- Kendi değerleriniz ile değiştiriniz! ------
char ssid[] = "SSID adını tırnak içine yazınız";  //  SSID isminiz
char password[] = "SSID şifrenizi tırnak içine yazınız";  // şifreniz
#define API_KEY "youtube v3 api key tırnak içine yazınız"  // API Token iniz
#define CHANNEL_ID "UCrDlm8ldOOh5cG5prINb77w" // kendi url kimliğinizi tırnak içine yazınız
//------- ---------------------- ------

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests = 60000;
unsigned long nextRunTime;

long subs = 0;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // Required if you are using ESP8266 V2.5 or above
  client.setInsecure();

  // If you want to enable some extra debugging
  api._debug = true;
    lcd.begin();
    
    
}

void loop() {

  if (millis() > nextRunTime)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("TAKiP:");
      lcd.setCursor(6,0);
      lcd.print(api.channelStats.subscriberCount);
       
      lcd.setCursor(10,0);
      lcd.print("ViD:");
      lcd.setCursor(14,0);
      lcd.print(api.channelStats.videoCount);
      
      lcd.setCursor(1,1);
      lcd.print("iZLENME:");
      lcd.setCursor(10,1);
      lcd.print(api.channelStats.viewCount);
      
    }
    nextRunTime = millis() + timeBetweenRequests;
  }
}
//DR.TRonik >>> YouTube>>> https://www.youtube.com/channel/UCrDlm8ldOOh5cG5prINb77w
