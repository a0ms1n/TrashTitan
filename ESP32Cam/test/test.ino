#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "cam_pins.h"

// Replace with your network credentials
const char* ssid = "Pakin_2.4G";
const char* password = "pakin270651";

WebServer server(6767); // Create a web server on port 6767
bool is_camera_on = false;

camera_config_t config;

String getHtmlPage() {
  String page = "<!DOCTYPE html><html><head>";
  page += "<title>ESP32 Camera</title>";
  page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  page += "<style>body{text-align:center;font-family:Arial;margin-top:50px;}button{font-size:24px;padding:15px 30px;margin:20px;}</style>";
  page += "</head><body>";
  page += "<h1>ESP32 Camera</h1>";
  page += "<button onclick=\"toggleCamera('on')\">ON</button>";
  page += "<button onclick=\"toggleCamera('off')\">OFF</button>";

  // Show camera stream if on
  if (is_camera_on) {
    page += "<div><img src='/stream' width='320' height='240'></div>";
  }

  page += "<script>function toggleCamera(state){fetch('/'+state).then(()=>location.reload());}</script>";
  page += "</body></html>";
  return page;
}

// Handle root "/"
void handleRoot() {
  server.send(200, "text/html", getHtmlPage());
}

void handleOn() {
  Serial.println("ON");
  is_camera_on = true;
  digitalWrite(PWDN_GPIO_NUM, LOW); // enable camera
  server.sendHeader("Location", "/"); 
  server.send(303);
}

void handleOff() {
  Serial.println("OFF");
  is_camera_on = false;
  digitalWrite(PWDN_GPIO_NUM, HIGH); // disable camera
  server.sendHeader("Location", "/"); 
  server.send(303);
}


// Stream video
void handleStream() {
  if (!is_camera_on) {
    server.send(404, "text/plain", "Camera is OFF");
    return;
  }

  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  client.print(response);

  while (client.connected() && is_camera_on) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      continue;
    }

    client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);
    client.write(fb->buf, fb->len);
    client.print("\r\n");
    esp_camera_fb_return(fb);

    // Small delay to allow server to handle other requests
    delay(1); 
    server.handleClient(); // Allow handling OFF request
  }

  Serial.println("Client disconnected or camera turned off");
}


// Initialize camera
void initCamera() {
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
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
  } else {
    Serial.println("Camera initialized");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  initCamera();

  // Server routes
  server.on("/", handleRoot);
 server.on("/on", HTTP_GET, handleOn);
  server.on("/on", HTTP_POST, handleOn);

  server.on("/off", HTTP_GET, handleOff);
  server.on("/off", HTTP_POST, handleOff);
  server.on("/stream", HTTP_GET, handleStream);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}