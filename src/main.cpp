/**
 * @file main.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-25
*  https://docs.m5stack.com/en/core/M5StickC%20PLUS2*
 * @copyright Copyright (c) 2023
 * https://randomnerdtutorials.com/esp32-http-get-post-arduino/
 *
 */
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

#include "lcd/lcd.h"
#include "imu/imu.h"

#define POWER_HOLD_PIN 4

#define WIFI_RETRIES 20
const char* ssid0     = "S21_BBC"; // Change this to your WiFi SSID
const char* password0 = "Magnetos"; // Change this to your WiFi password
const char* ssid     = "Cabovisao-E30F"; // Change this to your WiFi SSID
const char* password = "e0cec31ae30f"; // Change this to your WiFi password
const char* ssid2     = "Vodafone-C850D2"; // Change this to your WiFi SSID
const char* password2 = "Guiga+Tiz"; // Change this to your WiFi password
const char* udpServer = "192.168.63.231"; // Change this to your Server's IP
const int udpPort = 44444;
const int sampleSize = 2 * 3 * 4; // 6-axis imuStream
//Your Domain name with URL path or IP address with path
String serverName = "http://epics.ipfn.tecnico.ulisboa.pt/udp-server";

const int printPeriod = 100;
const int nSamples = 10;
const int lcdPeriod = 100;
const int samplePeriod = lcdPeriod / nSamples;

// const uint8_t IR_SEND_PIN = 19;

String serverUdp;
                                             //
imuStream::LCD lcd_device;
imuStream::IMU imu_device;

imu_accel_gyro_3d_t imu_data[2][nSamples];

WiFiMulti WiFiMulti;
//create UDP instance
WiFiUDP udp;

void get_udp_server(){
    HTTPClient http;

    String serverPath = serverName; //  + "?temperature=24.37";

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        serverUdp = http.getString();
        Serial.println(serverUdp);
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}

void setup()
{

    /* Hold pwr pin */
    gpio_reset_pin((gpio_num_t)POWER_HOLD_PIN);
    pinMode(POWER_HOLD_PIN, OUTPUT);
    digitalWrite(POWER_HOLD_PIN, HIGH);
//    pinMode(IR_SEND_PIN, OUTPUT);
//    digitalWrite(IR_SEND_PIN, HIGH);
    // For IMU
    lcd_device.setup();

    Serial.begin(115200);
    // connecting to a WiFi network
    WiFiMulti.addAP(ssid0, password0);
    WiFiMulti.addAP(ssid, password);
    WiFiMulti.addAP(ssid2, password2);
    Serial.print("Waiting for WiFi... ");
    for(int i = 0; i < WIFI_RETRIES; i++) {
        if(WiFiMulti.run() != WL_CONNECTED) {
            //while(WiFiMulti.run() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }
        else {
            Serial.print(". WiFi connected. ");
            Serial.print("IP address: ");
            IPAddress ip = WiFi.localIP();
            Serial.println(ip);
            get_udp_server();
            break;
        }
    }
    imu_device.setup();
    delay(2000);
}

void loop()
{
    static unsigned long nextUdpTime = printPeriod + 7;
    static unsigned long nextItime = 0;
    static unsigned long nextLcdtime = 2000;
    static unsigned bufferWrt = 0;
    static unsigned bufferRd = 0;
    static unsigned sample = 0;

    unsigned long time_dt[2];
    unsigned long now_ms = millis();
    if ( now_ms > nextUdpTime ) {
        nextUdpTime = now_ms + printPeriod;
        //Serial.println("IMU ");
        printf("%f \t %f \t %f\n", imu_data[bufferRd][sample].accel_x, imu_data[bufferRd][sample].accel_y, imu_data[bufferRd][sample].accel_z);

    }
    if ( now_ms > nextItime ) {
        nextItime = now_ms + samplePeriod;

        imu_device.get_data(&imu_data[bufferWrt][sample++]);
        if(sample >= nSamples){
            sample = 0;
            if(bufferWrt == 0)
                bufferWrt = 1;
            else
                bufferWrt = 0;
        }
    }

    if ( now_ms > nextLcdtime ) {
        nextLcdtime = now_ms + lcdPeriod;

        //printf("%f \t %f \t %f\n", accX, accY, accZ);
        lcd_device.draw_accel(imu_data[0][0].accel_x,
                imu_data[0][0].accel_y,
                imu_data[0][0].accel_z);
        //Check WiFi connection status
        if(WiFi.status()== WL_CONNECTED){
        //This initializes udp and transfer buffer
            time_dt[0] = now_ms;
            time_dt[1] = 1;
            udp.beginPacket(serverUdp.c_str(), udpPort);
            //udp.write(buffer, 11);
            udp.write((uint8_t *) time_dt, 8);
            //udp.write((uint8_t *) data.value, packetSize);
            for(int i = 0; i < nSamples; i++) {
                udp.write((uint8_t *) &imu_data[bufferRd][i], sampleSize);
                //udp.write((uint8_t *) &imu_data[bufferRd][1], sampleSize);
            }
            if(bufferRd == 0)
                bufferRd = 1;
            else
                bufferRd = 0;
            udp.endPacket();
        }
        else {
            Serial.println("WiFi Disconnected");
        }
        //checkReboot();
    }

}
