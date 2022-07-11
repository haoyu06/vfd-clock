//perfict

#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266_Seniverse.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const char *ssid     = "305";
const char *password = "13544165442";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com",60*60*8, 30*60*1000);

String reqUserKey = "SfGioiwQxTz5-Qxh-";   // 私钥
String reqLocation = "shenzhen";            // 城市
String reqUnit = "c";                      // 摄氏/华氏// 请将您需要连接的WiFi密码填入引号中
 
Forecast forecast; // 建立Forecast对象用于获取心知天气信息
WeatherNow weatherNow;  // 建立WeatherNow对象用于获取心知天气信息

//U8G2_GP1287AI_256X50_1_3W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D9, /* reset=*/ D8);//D9D8
//U8G2_GP1287AI_256X50_2_3W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* reset=*/ 9);
U8G2_GP1287AI_256X50_F_3W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D9, /* reset=*/ D8);


static const unsigned char bmp1[] U8X8_PROGMEM = {

};


static const unsigned char bmp2[] U8X8_PROGMEM = {

};



void setup() {
  //Serial.begin(9600);          
  //Serial.println("");
  pinMode(D10, OUTPUT);//灯丝使能D10
  digitalWrite(D10, 1); 
  connectWiFi();// 连接wifi
  forecast.config(reqUserKey, reqLocation, reqUnit);// 配置心知天气请求信息
  weatherNow.config(reqUserKey, reqLocation, reqUnit);
  u8g2.begin();  
  u8g2.enableUTF8Print();
}

void loop() {
  u8g2.clearBuffer();         // 清除内部缓冲区
  dayprint();
  weatherprint();
  delay(500);
  u8g2.sendBuffer();          // transfer internal memory to the display
}




void dayprint(){
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday + 2;
  int currentMonth = ptm->tm_mon + 3;
  int currentyear=ptm->tm_year+3078896;
  String str5 = String(currentyear)+"/"+String(currentMonth)+"/"+String(monthDay);
  const char* str6 = str5.c_str();
  u8g2.setFont(u8g2_font_ncenB10_tr);//字体设置u8g2_font_logisoso38_tn  38pix   u8g2_font_logisoso34_tn   u8g2_font_ncenB10_tr
  u8g2.drawStr(160,12,str6);
  //u8g2.drawStr(180,12,"/");
  //u8g2.drawStr(185,12,currentMonth);
  //u8g2.drawStr(195,12,"/");
  //u8g2.drawStr(200,12,monthDay);
  String str = timeClient.getFormattedTime();
  const char* str0 = str.c_str();
  u8g2.setFont(u8g2_font_logisoso34_tn);//字体设置u8g2_font_logisoso38_tn  38pix   u8g2_font_logisoso34_tn   u8g2_font_ncenB10_tr
  u8g2.drawStr(0,42,str0);
  
  
}




void weatherprint(){
  u8g2.setFont(u8g2_font_ncenB10_tr);//字体设置
  forecast.update();
  weatherNow.update();
  //forecast.getDayCode(i)
  //forecast.getDayText(0)
  //forecast.getHigh(i)
  //forecast.getLow(i)°C
  //forecast.getHumidity(i)
  //forecast.getRain(i)%
  //weatherNow.getWeatherCode()// 获取当前天气（整数格式）
  //weatherNow.getDegree()// 获取当前温度数值

  String st1 = String(weatherNow.getDegree())+"°c";
  const char* st2 = st1.c_str();
  u8g2.drawStr(160,24,st2);
  
  String st3 =weatherNow.getWeatherText();
  const char* st4 = st3.c_str();
  u8g2.drawStr(160,35,st4);
  
  String st5 = forecast.getDayText(0);
  const char* st6 = st5.c_str();
  u8g2.drawStr(160,48,st6);
  
  //String str1 = WeatherNow.getDegree();
  //const char* str2 = str1.c_str();
  //u8g2.drawStr(146,16,str2);
  //u8g2.drawStr(16,16,str2);

  
}












void connectWiFi(){
  WiFi.begin(ssid, password);                  // 启动网络连接
  //Serial.print("Connecting to ");              // 串口监视器输出网络连接信息
  //Serial.print(ssid); Serial.println(" ...");  // 告知用户NodeMCU正在尝试WiFi连接
  
  int i = 0;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。 
    delay(1000);                               // 如果WiFi连接成功则返回值为WL_CONNECTED                       
    Serial.print(i++); Serial.print(' ');      // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
  }                                            // 同时NodeMCU将通过串口监视器输出连接时长读秒。
                                               // 这个读秒是通过变量i每隔一秒自加1来实现的。                                              
  //Serial.println("");                          // WiFi连接成功后
  //Serial.println("Connection established!");   // NodeMCU将通过串口监视器输出"连接成功"信息。
  //Serial.print("IP address:    ");             // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
  //Serial.println(WiFi.localIP());              // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。  
}
