#define BLINKER_PRINT Serial
#define BLINKER_MIOT_LIGHT
#define BLINKER_WIFI
#include <Blinker.h>
#include <Adafruit_NeoPixel.h>
#include"leddata.h"
#include"hr.h"

char auth[] = "f03cbe963ff3";/****秘钥****/
#define PIN 15  //  DIN PIN (GPIO15, D8)
#define NUMPIXELS 60  // Number of you led
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
bool btn=false;

// 新建组件对象
BlinkerRGB RGB1("RGB");
BlinkerButton Button1("btn-breath");

int LED_R=0,LED_G=0,LED_B=0,LED_Bright=180;// RGB和亮度
bool WIFI_Status = true;
void smartConfig()//配网函数
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig...");
  WiFi.beginSmartConfig();//等待手机端发出的用户名与密码
  while (1)
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(1000);                      
    digitalWrite(LED_BUILTIN, LOW);    
    delay(1000);                      
    if (WiFi.smartConfigDone())//退出等待
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;
    }
  }
}
void WIFI_Set()//
{
    //Serial.println("\r\n正在连接");
    int count = 0;
    while(WiFi.status()!=WL_CONNECTED)
    {
        if(WIFI_Status)
        {
            Serial.print(".");
            digitalWrite(LED_BUILTIN, HIGH);  
            delay(500);                       
            digitalWrite(LED_BUILTIN, LOW);    
            delay(500);                 
            count++;
            if(count>=5)//5s
            {
                WIFI_Status = false;
                Serial.println("WiFi连接失败，请用手机进行配网"); 
            }
        }
        else
        {
            smartConfig();  //微信智能配网
        }
     }  
    /* Serial.println("连接成功");  
     Serial.print("IP:");
     Serial.println(WiFi.localIP());*/
}


/*void Rec(uint8_t* r_value, uint8_t* g_value, 
          uint8_t* b_value, uint8_t* bright_value){
    BLINKER_LOG("R value: ", *r_value);
    BLINKER_LOG("G value: ", *g_value);
    BLINKER_LOG("B value: ", *b_value);
    BLINKER_LOG("Rrightness value: ", *bright_value);
    //LED_Bright = *bright_value;
}*/

//APP RGB颜色设置回调
void rgb1_callback(uint8_t r_value, uint8_t g_value, 
                    uint8_t b_value, uint8_t bright_value)
{
    //Serial.printf("RGBB: %d,%d,%d,%d",r_value,g_value,b_value,Bright);
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);
    for (uint16_t i = 0; i < NUMPIXELS; i++) //把灯条变色
    {
        pixels.setPixelColor(i,r_value,g_value,b_value);
    }
    LED_Bright = bright_value;
    pixels.setBrightness(LED_Bright);//亮度
    pixels.show();    //送出显示
    }

void Led_init(uint8_t r_value, uint8_t g_value,
                uint8_t b_value, uint8_t bright_value){
  r_value=0;
  g_value=0;
  b_value=0;
  bright_value=0;
for (uint16_t i = 0; i < NUMPIXELS; i++) //把灯条变色
    {
        pixels.setPixelColor(i,r_value,g_value,b_value);
    }
    LED_Bright = bright_value;
    pixels.setBrightness(LED_Bright);//亮度
    pixels.show();    //送出显示
}

void breath_callback(const String & state){
  //Serial.println(state);
  uint8_t r,g,b,bright;
  Led_init(r,g,b,bright); //初始化灯
  if(state=="on"){
  btn = true;
  Button1.print("on");
  }
    else{
      btn=false;
      Button1.print("off");
  }
//}
}

void breath_led(){
  static long ran=0;
  static int count=0;
  ran = random(0,8);
  //while(true){
  if(count==0||count == 344){
  LED_R=(int)((colorArry[ran]>>16) & 0xFF);
  LED_G=(int)((colorArry[ran]>>8) & 0xFF);
  LED_B=(int)(colorArry[ran] & 0xFF);
  
  count=0;
  }
  LED_Bright=index_wave[count];
  for(uint8_t j =0;j<NUMPIXELS;++j){
    pixels.setPixelColor(j,LED_R,LED_G,LED_B);
  }
  pixels.setBrightness(LED_Bright);//亮度
  pixels.show();    //送出显示
  //Blinker.delay(10);
  ++count;
  Serial.printf("bright: %d",LED_Bright);
  //Serial.printf("count: %d\r\n",count);
  //Serial.printf("RGB: %d,%d,%d\r\n",r,g,b);
//}
}
void setup() {
    // 初始化串口
    Serial.begin(115200);

    randomSeed(analogRead(0));
    pixels.begin();//WS2812初始化
    pixels.show();
    pinMode(LED_BUILTIN, OUTPUT);
    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif
    //debug
    /*BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();*/

    
    WIFI_Set();
    // 初始化blinker
    Blinker.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
    RGB1.attach(rgb1_callback);//注册调节颜色的回调函数
    Button1.attach(breath_callback);    
}

void loop() {
    Blinker.run();
    if(btn==true){
      breath_led();
    }
}
