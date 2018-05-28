#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#define LED_RED     0
#define LED_GREEN   5
#define LED_BLUE    4
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;
float wave;
float red,blue,green;
float gamma1 =0.8;
float attenuation;
ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            wave = (payload[0]-'0') * 100 + (payload[1]-'0') * 10 + (payload[2]-'0');
            USE_SERIAL.printf("Value of wavelength received is : %d\n",wave);
            if(wave>=370 && wave<=760){
               if (wave >= 370 & wave <= 380) {
                 red=0;green=0;blue=0;
                  }   
              else if (wave >= 380 & wave <= 440) {
                  attenuation = 0.3 + 0.7 * (wave - 380) / (440 - 380);
                  red = pow(((-(wave - 440) / (440 - 380)) * attenuation), gamma1);
                  green = 0.0;
                  blue = pow((1.0 * attenuation),gamma1);
                  }
              else if (wave >= 440 & wave <= 490) {
                  red = 0.0;
                  green = pow(((wave - 440) / (490 - 440)), gamma1);
                  blue = 1.0;
                  }
              else if (wave >= 490 & wave <= 510) {
                  red = 0.0;
                  green = 1.0;
                  blue = pow((-(wave - 510) / (510 - 490)) , gamma1);
                  }
              else if (wave >= 510 & wave <= 580) {
                  red = pow(((wave - 510) / (580 - 510)), gamma1);
                  green = 1.0;
                  blue = 0.0;
                  }
              else if (wave >= 580 & wave <= 645) {
                  red = 1.0;
                  green = pow((-(wave - 645) / (645 - 580)),gamma1);
                  blue = 0.0;
                  }
              else if (wave >= 645 & wave<= 750) {
                  attenuation = 0.3 + 0.7 * (750 - wave) / (750 - 645);
                  red = pow((1.0 * attenuation), gamma1);
                  green = 0.0;
                  blue = 0.0;
                  }
              else {
                  red = 1;
                  green = 1;
                 blue = 1;
                  }
              Serial.println("Display Values without 1023 multiplier:");
             Serial.println("red:");
             Serial.println(red);
             Serial.println("green:");
             Serial.println(green);
             Serial.println("blue:");
             Serial.println(blue);
              red = red * 1023;
              green= green* 1023;
              blue=blue * 1023;                        
              }
//             Serial.println("Display Values:");
//             Serial.println("red:");
//             Serial.println(red);
//             Serial.println("green:");
//             Serial.println(green);
//             Serial.println("blue:");
//             Serial.println(blue);
             analogWrite(LED_RED,   red );
             analogWrite(LED_GREEN,  green);
             analogWrite(LED_BLUE,   blue);
            break;
    }
}
void setup() {
    USE_SERIAL.begin(115200);
    Serial.begin(115200);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_RED, 1);
    digitalWrite(LED_GREEN, 1);
    digitalWrite(LED_BLUE, 1);
    WiFi.softAP("No", "connected2");
    IPAddress myIP = WiFi.softAPIP();
    USE_SERIAL.print("AP IP address: ");
    USE_SERIAL.println(myIP);
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    if(MDNS.begin("esp8266")) {
        USE_SERIAL.println("MDNS responder started");
    }
    server.on("/", []() {
        server.send(200, "text/html", 
        "<html><head><style>.slidecontainer {width: 100%;}.slider {-webkit-appearance: none;width: 100%;height: 25px;background: linear-gradient(to right,#000000 0%,#000000 2.5%, #610061 2.756%, #630064 3.012%, #640067 3.268%, #66006a 3.524%, #67006d 3.78%, #680070 4.036%, #690073 4.292%, #6a0076 4.548%, #6b0078 4.804%, #6c007b 5.06%, #6d007e 5.316%, #6e0081 5.572%, #6e0084 5.828%, #6f0087 6.084%, #6f0089 6.34%, #6f008c 6.596%, #6f008f 6.852%, #6f0092 7.108%, #6f0094 7.364%, #6f0097 7.62%, #6f009a 7.876%, #6f009c 8.132%, #6e009f 8.388%, #6e00a2 8.644%, #6d00a4 8.9%, #6c00a7 9.156%, #6c00aa 9.412%, #6b00ac 9.668%, #6a00af 9.924%, #6800b2 10.18%, #6700b4 10.436%, #6600b7 10.692%, #6500b9 10.948%, #6300bc 11.204%, #6100be 11.46%, #6000c1 11.716%, #5e00c4 11.972%, #5c00c6 12.228%, #5a00c9 12.484%, #5700cb 12.74%, #5500ce 12.996%, #5300d0 13.252%, #5000d3 13.508%, #4d00d5 13.764%, #4b00d8 14.02%, #4800da 14.276%, #4500dd 14.532%, #4100df 14.788%, #3e00e2 15.044%, #3a00e4 15.3%, #3700e6 15.556%, #3300e9 15.812%, #2f00eb 16.068%, #2a00ee 16.324%, #2600f0 16.58%, #2100f3 16.836%, #1c00f5 17.092%, #1600f7 17.348%, #1000fa 17.604%, #0900fc 17.86%, #0000ff 18.116%, #000bff 18.372%, #0013ff 18.628%, #001aff 18.884%, #0021ff 19.14%, #0028ff 19.396%, #002eff 19.652%, #0034ff 19.908%, #003aff 20.164%, #0040ff 20.42%, #0046ff 20.676%, #004bff 20.932%, #0051ff 21.188%, #0056ff 21.444%, #005cff 21.7%, #0061ff 21.956%, #0066ff 22.212%, #006bff 22.468%, #0070ff 22.724%, #0075ff 22.98%, #007aff 23.236%, #007fff 23.492%, #0084ff 23.748%, #0089ff 24.004%, #008dff 24.26%, #0092ff 24.516%, #0097ff 24.772%, #009bff 25.028%, #00a0ff 25.284%, #00a4ff 25.54%, #00a9ff 25.796%, #00adff 26.052%, #00b2ff 26.308%, #00b6ff 26.564%, #00bbff 26.82%, #00bfff 27.076%, #00c4ff 27.332%, #00c8ff 27.588%, #00ccff 27.844%, #00d1ff 28.1%, #00d5ff 28.356%, #00d9ff 28.612%, #00ddff 28.868%, #00e2ff 29.124%, #00e6ff 29.38%, #00eaff 29.636%, #00eeff 29.892%, #00f2ff 30.148%, #00f6ff 30.404%, #00faff 30.66%, #00ffff 30.916%, #00fff4 31.172%, #00ffea 31.428%, #00ffdf 31.684%, #00ffd5 31.94%, #00ffca 32.196%, #00ffbf 32.452%, #00ffb4 32.708%, #00ffa9 32.964%, #00ff9e 33.22%, #00ff92 33.476%, #00ff86 33.732%, #00ff7a 33.988%, #00ff6e 34.244%, #00ff61 34.5%, #00ff54 34.756%, #00ff46 35.012%, #00ff37 35.268%, #00ff28 35.524%, #00ff17 35.78%, #00ff00 36.036%, #08ff00 36.292%, #0eff00 36.548%, #14ff00 36.804%, #19ff00 37.06%, #1eff00 37.316%, #23ff00 37.572%, #28ff00 37.828%, #2cff00 38.084%, #31ff00 38.34%, #35ff00 38.596%, #3aff00 38.852%, #3eff00 39.108%, #42ff00 39.364%, #46ff00 39.62%, #4aff00 39.876%, #4eff00 40.132%, #52ff00 40.388%, #56ff00 40.644%, #59ff00 40.9%, #5dff00 41.156%, #61ff00 41.412%, #65ff00 41.668%, #68ff00 41.924%, #6cff00 42.18%, #6fff00 42.436%, #73ff00 42.692%, #77ff00 42.948%, #7aff00 43.204%, #7eff00 43.46%, #81ff00 43.716%, #84ff00 43.972%, #88ff00 44.228%, #8bff00 44.484%, #8fff00 44.74%, #92ff00 44.996%, #95ff00 45.252%, #99ff00 45.508%, #9cff00 45.764%, #9fff00 46.02%, #a2ff00 46.276%, #a6ff00 46.532%, #a9ff00 46.788%, #acff00 47.044%, #afff00 47.3%, #b3ff00 47.556%, #b6ff00 47.812%, #b9ff00 48.068%, #bcff00 48.324%, #bfff00 48.58%, #c2ff00 48.836%, #c5ff00 49.092%, #c9ff00 49.348%, #ccff00 49.604%, #cfff00 49.86%, #d2ff00 50.116%, #d5ff00 50.372%, #d8ff00 50.628%, #dbff00 50.884%, #deff00 51.14%, #e1ff00 51.396%, #e4ff00 51.652%, #e7ff00 51.908%, #eaff00 52.164%, #edff00 52.42%, #f0ff00 52.676%, #f3ff00 52.932%, #f6ff00 53.188%, #f9ff00 53.444%, #fcff00 53.7%, #ffff00 53.956%, #fffb00 54.212%, #fff800 54.468%, #fff500 54.724%, #fff200 54.98%, #ffef00 55.236%, #ffeb00 55.492%, #ffe800 55.748%, #ffe500 56.004%, #ffe200 56.26%, #ffdf00 56.516%, #ffdb00 56.772%, #ffd800 57.028%, #ffd500 57.284%, #ffd200 57.54%, #ffce00 57.796%, #ffcb00 58.052%, #ffc800 58.308%, #ffc400 58.564%, #ffc100 58.82%, #ffbe00 59.076%, #ffba00 59.332%, #ffb700 59.588%, #ffb300 59.844%, #ffb000 60.1%, #ffac00 60.356%, #ffa900 60.612%, #ffa500 60.868%, #ffa200 61.124%, #ff9e00 61.38%, #ff9b00 61.636%, #ff9700 61.892%, #ff9400 62.148%, #ff9000 62.404%, #ff8d00 62.66%, #ff8900 62.916%, #ff8500 63.172%, #ff8100 63.428%, #ff7e00 63.684%, #ff7a00 63.94%, #ff7600 64.196%, #ff7200 64.452%, #ff6f00 64.708%, #ff6b00 64.964%, #ff6700 65.22%, #ff6300 65.476%, #ff5f00 65.732%, #ff5b00 65.988%, #ff5700 66.244%, #ff5300 66.5%, #ff4e00 66.756%, #ff4a00 67.012%, #ff4600 67.268%, #ff4200 67.524%, #ff3d00 67.78%, #ff3900 68.036%, #ff3400 68.292%, #ff2f00 68.548%, #ff2a00 68.804%, #ff2500 69.06%, #ff2000 69.316%, #ff1b00 69.572%, #ff1500 69.828%, #ff0f00 70.084%, #ff0900 70.34%, #ff0000 70.596%, #fd0000 70.852%, #fc0000 71.108%, #fa0000 71.364%, #f90000 71.62%, #f80000 71.876%, #f60000 72.132%, #f50000 72.388%, #f40000 72.644%, #f20000 72.9%, #f10000 73.156%, #ef0000 73.412%, #ee0000 73.668%, #ed0000 73.924%, #eb0000 74.18%, #ea0000 74.436%, #e80000 74.692%, #e70000 74.948%, #e60000 75.204%, #e40000 75.46%, #e30000 75.716%, #e20000 75.972%, #e00000 76.228%, #df0000 76.484%, #dd0000 76.74%, #dc0000 76.996%, #da0000 77.252%, #d90000 77.508%, #d80000 77.764%, #d60000 78.02%, #d50000 78.276%, #d30000 78.532%, #d20000 78.788%, #d10000 79.044%, #cf0000 79.3%, #ce0000 79.556%, #cc0000 79.812%, #cb0000 80.068%, #c90000 80.324%, #c80000 80.58%, #c60000 80.836%, #c50000 81.092%, #c40000 81.348%, #c20000 81.604%, #c10000 81.86%, #bf0000 82.116%, #be0000 82.372%, #bc0000 82.628%, #bb0000 82.884%, #b90000 83.14%, #b80000 83.396%, #b60000 83.652%, #b50000 83.908%, #b30000 84.164%, #b20000 84.42%, #b00000 84.676%, #af0000 84.932%, #ad0000 85.188%, #ac0000 85.444%, #aa0000 85.7%, #a90000 85.956%, #a70000 86.212%, #a60000 86.468%, #a40000 86.724%, #a30000 86.98%, #a10000 87.236%, #a00000 87.492%, #9e0000 87.748%, #9d0000 88.004%, #9b0000 88.26%, #9a0000 88.516%, #980000 88.772%, #970000 89.028%, #950000 89.284%, #940000 89.54%, #920000 89.796%, #900000 90.052%, #8f0000 90.308%, #8d0000 90.564%, #8c0000 90.82%, #8a0000 91.076%, #890000 91.332%, #870000 91.588%, #850000 91.844%, #840000 92.1%, #820000 92.356%, #810000 92.612%, #7f0000 92.868%, #7d0000 93.124%, #7c0000 93.38%, #7a0000 93.636%, #780000 93.892%, #770000 94.148%, #750000 94.404%, #730000 94.66%, #720000 94.916%, #700000 95.172%, #6e0000 95.428%, #6d0000 95.684%, #6b0000 95.94%, #690000 96.196%, #680000 96.452%, #660000 96.708%, #640000 96.964%, #630000 97.22%, #610000 97.476%, #ffffff 97.86% , #ffffff 99%, #000000 100%);outline: none;opacity: 0.7;-webkit-transition: .2s;transition: opacity .2s;slider:hover {opacity: 1;}.slider::-webkit-slider-thumb {-webkit-appearance: none;appearance: none;width: 25px;height: 25px;background: #4CAF50;cursor: pointer;}.slider::-moz-range-thumb {width: 25px;height: 25px;background: #4CAF50;cursor: pointer;}</style><script>"
        "var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);"
        "connection.onopen = function () {  connection.send('Connect ' + new Date()); };"
        "connection.onerror = function (error) {    console.log('WebSocket Error ', error);};"
        "connection.onmessage = function (e) {  console.log('Server: ', e.data);};"
        "function sendRGB() {  "
        "var wavelength = parseInt(document.getElementById('wavelength').value);"
        "connection.send(wavelength); }"
        "</script></head><body>"
        "<h1>NodeMCU trial led:</h1><br/><br/>"
        "<p><h3>Slide through the available band:</h3></p>"
        "<div class=\"slidecontainer\"><input type=\"range\" min=\"370\" max=\"760\" value=\"500\" class=\"slider\" id=\"wavelength\" step=\"1\" onchange=\"sendRGB();\"><p>Value: <span id=\"demo\"></span></p></div><script>var slider = document.getElementById(\"wavelength\");var output = document.getElementById(\"demo\");output.innerHTML = slider.value;slider.oninput = function() {output.innerHTML = this.value;}</script>"
        "</body></html>");
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
    digitalWrite(LED_RED, 0);
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 0);
}
void loop() {
    webSocket.loop();
    server.handleClient();
}
