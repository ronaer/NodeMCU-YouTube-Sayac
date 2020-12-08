/*Thanks to Bonny Useful YouTube cahennel to sharing...>>> https://www.youtube.com/channel/UCEDJLuOw8cld2rvy8Sic2xA
 * DR.TRonik tarafından P10 32*16 Led Matrix tabela için YouTube Takipçi Sayacı olarak modifiye edilmiştir...
 * Açık kaynak lisansı altında kaynak belirtilerek kopyalanabilir, dağıtılabilir, kişisel olarak kullanılabilir...
 */

#include <DMDESP.h>
#include <fonts/SystemFont5x7.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h>

char ssid[] = "SSID adını tırnak içine yazınız";  //  SSID isminiz
char password[] = "SSID şifrenizi tırnak içine yazınız";  // şifreniz
#define API_KEY "youtube v3 api key tırnak içine yazınız"  // API Token iniz
#define CHANNEL_ID "UCrDlm8ldOOh5cG5prINb77w" // kendi url kimliğinizi tırnak içine yazınız

//SETUP DMD
#define DISPLAYS_WIDE 1 // 1 panel eninde
#define DISPLAYS_HIGH 1 // 1 Panel boyunda
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  // 1 panel 1 panel
WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests = 180000;
unsigned long nextRunTime;

long subs = 0;

//----------------------------------------
void setup() {

  Disp.start(); // DMDESP başalr
  Disp.setBrightness(40); // Parlaklığı ayarlayalım
  Disp.setFont(SystemFont5x7); // Font tanımı

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
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
   api._debug = true;
}
//----------------------------------------
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
//      Serial.print("hiddenSubscriberCount: ");
//      Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");
}
    nextRunTime = millis() + timeBetweenRequests;
  }

  
 
  Disp.loop(); // Kütüphane özelliği

  Disp.drawText(6,0,String(api.channelStats.subscriberCount)); // String olarak Takipçi sayısı
  KayarMetin(8, 50); // Kayar metin fonksiyonona göndereceğimiz bilgi (y pozisyonu, hız);

}
//--------------------------
// Kayar Yazı Fonksiyonu

static char *tekst[] = {"Dr.TRonik YouTube TAKiPCi SAYISI"}; // Tırnak içine istedğinizi yazabilirsiniz

void KayarMetin(int y, uint8_t metin_hizi) {

  static uint32_t pM;
  static uint32_t x;
  int width = Disp.width();
  Disp.setFont(SystemFont5x7);
  int fullScroll = Disp.textWidth(tekst[0]) + width;
  if((millis() - pM) > metin_hizi) { 
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, tekst[0]);
  }  

}
//DR.TRonik >>> YouTube>>> https://www.youtube.com/channel/UCrDlm8ldOOh5cG5prINb77w
