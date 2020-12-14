#include "esp_camera.h"
#include <WiFi.h>

const char* ssid = "Sandbox-staff2.4G";
const char* password = "songfusongfu";

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define HREF_GPIO_NUM     23
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define PCLK_GPIO_NUM     22

// GPIO Setting
extern int MTR_IN4 =  2; // Left 1
extern int MTR_IN3 = 14; // Left 2
extern int MTR_IN2 = 15; // Right 1
extern int MTR_IN1 = 13; // Right 2
extern int EBD_LED =  4; // Light
extern String WiFiAddr ="";

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);  //开启WIFI库诊断输出
  Serial.println();

  pinMode(MTR_IN4, OUTPUT); //Left Backward
  pinMode(MTR_IN3, OUTPUT); //Left Forward
  pinMode(MTR_IN2, OUTPUT); //Right Forward
  pinMode(MTR_IN1, OUTPUT); //Right Backward
  pinMode(EBD_LED, OUTPUT); //Light

  //initialize
  digitalWrite(MTR_IN4, LOW);
  digitalWrite(MTR_IN3, LOW);
  digitalWrite(MTR_IN2, LOW);
  digitalWrite(MTR_IN1, LOW);
  digitalWrite(EBD_LED, LOW);

  //相机设置
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  //相机初始化
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  //降低分辨率提高帧率
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);


  //设置WIFI
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);

  //启动服务器
  startCameraServer();
  Serial.print("Drone Ready on: ");
  Serial.println(WiFi.localIP());
  WiFiAddr = WiFi.localIP().toString();
}

void loop() {
}
