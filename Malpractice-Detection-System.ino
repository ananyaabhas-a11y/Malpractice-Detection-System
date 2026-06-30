#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <LiquidCrystal.h>
#include <Preferences.h>
#include <string.h>


LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
Preferences prefs;


#define RED_LED    23
#define GREEN_LED  18
#define BLUE_LED   2


#define BUZZER_PIN     19
#define STORE_BUTTON   4
#define DELETE_BUTTON  5
#define POT_PIN        34


#define WIFI_THRESHOLD  -65
#define BLE_THRESHOLD   -70
#define POT_THRESHOLD   2000


BLEScan *pBLEScan;


char bypassWiFiMAC[18] = {0};
char bypassBLEMAC[18]  = {0};

int wifiStored = 0;
int bleStored  = 0;


bool wifiFlag = false;
bool bleFlag  = false;
bool rfFlag   = false;

volatile bool storeRequest   = false;
volatile bool deleteRequest  = false;
volatile bool restartCycle   = false;


void IRAM_ATTR storeISR() {
    storeRequest = true;
}

void IRAM_ATTR deleteISR() {
    deleteRequest = true;
}


int isBypassed(char *mac, char *storedMac, int stored) {
    if (stored == 0) return 0;
    return (strcmp(mac, storedMac) == 0);
}


void storeInvigilator() {
    lcd.clear();
    lcd.print("STORING...");
    delay(500);

    int strongest = -100;
    char mac[18] = {0};

  
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) {
        if (WiFi.RSSI(i) > strongest) {
            strongest = WiFi.RSSI(i);
            strncpy(mac, WiFi.BSSIDstr(i).c_str(), sizeof(mac));
        }
    }

    strncpy(bypassWiFiMAC, mac, sizeof(bypassWiFiMAC));
    wifiStored = 1;

    prefs.begin("bypass", false);
    prefs.putString("wifiMAC", String(bypassWiFiMAC));
    prefs.end();

    delay(500);

  
    int strongBLE = -100;
    BLEScanResults *devices = pBLEScan->start(3);

    for (int i = 0; i < devices->getCount(); i++) {
        BLEAdvertisedDevice d = devices->getDevice(i);

        if (d.getRSSI() > strongBLE) {
            strongBLE = d.getRSSI();
            strncpy(bypassBLEMAC,
                    d.getAddress().toString().c_str(),
                    sizeof(bypassBLEMAC));
        }
    }

    pBLEScan->clearResults();
    bleStored = 1;

    prefs.begin("bypass", false);
    prefs.putString("bleMAC", String(bypassBLEMAC));
    prefs.end();

    lcd.clear();
    lcd.print("ADDRESS");
    lcd.setCursor(0, 1);
    lcd.print("STORED");

    delay(2000);
    restartCycle = true;
}


void deleteStored() {
    wifiStored = 0;
    bleStored  = 0;

    prefs.begin("bypass", false);
    prefs.remove("wifiMAC");
    prefs.remove("bleMAC");
    prefs.end();

    lcd.clear();
    lcd.print("DELETED");

    delay(1500);
    restartCycle = true;
}


void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(STORE_BUTTON, INPUT_PULLUP);
    pinMode(DELETE_BUTTON, INPUT_PULLUP);

    digitalWrite(BLUE_LED, LOW);
    delay(100);
    digitalWrite(BLUE_LED, HIGH);

    lcd.begin(16, 2);
    lcd.print("PLEASE WAIT");
    lcd.setCursor(0, 1);
    lcd.print("SYSTEM READY");

    delay(3000);

    WiFi.mode(WIFI_STA);

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);

    
    prefs.begin("bypass", true);

    String wmac = prefs.getString("wifiMAC", "");
    String bmac = prefs.getString("bleMAC", "");

    if (wmac != "") {
        strncpy(bypassWiFiMAC, wmac.c_str(), sizeof(bypassWiFiMAC));
        wifiStored = 1;
    }

    if (bmac != "") {
        strncpy(bypassBLEMAC, bmac.c_str(), sizeof(bypassBLEMAC));
        bleStored = 1;
    }

    prefs.end();

    attachInterrupt(digitalPinToInterrupt(STORE_BUTTON), storeISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(DELETE_BUTTON), deleteISR, FALLING);

    restartCycle = true;
}


void loop() {

    
    if (storeRequest) {
        storeRequest = false;
        WiFi.scanDelete();
        pBLEScan->stop();
        storeInvigilator();
        return;
    }

   
    if (deleteRequest) {
        deleteRequest = false;
        WiFi.scanDelete();
        pBLEScan->stop();
        deleteStored();
        return;
    }

    if (restartCycle) {
        restartCycle = false;
        return;
    }

    
    wifiFlag = false;
    bleFlag  = false;
    rfFlag   = false;

    lcd.clear();
    lcd.print("NEW CYCLE");
    lcd.setCursor(0, 1);
    lcd.print("STARTING...");
    delay(2000);

    
    lcd.clear();
    lcd.print("SCANNING");
    lcd.setCursor(0, 1);
    lcd.print("WIFI...");
    delay(1000);

    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; i++) {
        char mac[18] = {0};
        strncpy(mac, WiFi.BSSIDstr(i).c_str(), sizeof(mac));

        if (WiFi.RSSI(i) > WIFI_THRESHOLD &&
            !isBypassed(mac, bypassWiFiMAC, wifiStored)) {
            wifiFlag = true;
        }
    }

    delay(1000);

   
    lcd.clear();
    lcd.print("SCANNING");
    lcd.setCursor(0, 1);
    lcd.print("BLE...");
    delay(1000);

    BLEScanResults *devices = pBLEScan->start(5);

    for (int i = 0; i < devices->getCount(); i++) {
        BLEAdvertisedDevice d = devices->getDevice(i);

        char mac[18] = {0};
        strncpy(mac,
                d.getAddress().toString().c_str(),
                sizeof(mac));

        if (d.getRSSI() > BLE_THRESHOLD &&
            !isBypassed(mac, bypassBLEMAC, bleStored)) {
            bleFlag = true;
        }
    }

    pBLEScan->clearResults();
    delay(1000);

    
    lcd.clear();
    lcd.print("SCANNING");
    lcd.setCursor(0, 1);
    lcd.print("RF...");
    delay(1500);

    if (analogRead(POT_PIN) > POT_THRESHOLD) {
        rfFlag = true;
    }

    lcd.clear();

    
    bool malpracticeDetected = (wifiFlag || bleFlag || rfFlag);

    if (malpracticeDetected) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        tone(BUZZER_PIN, 1000);

        int pos = 0;

        if (wifiFlag) {
            lcd.setCursor(pos, 0);
            lcd.print("WiFi");
            pos += 5;
        }

        if (bleFlag) {
            lcd.setCursor(pos, 0);
            lcd.print("BLE");
            pos += 4;
        }

        if (rfFlag) {
            lcd.setCursor(pos, 0);
            lcd.print("RF");
        }

        lcd.setCursor(0, 1);

        if (wifiFlag && bleFlag && rfFlag)
            lcd.print("MALPRACT DETECT");
        else
            lcd.print("DEVICE DETECTED");

        delay(5000);
    } else {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        noTone(BUZZER_PIN);

        lcd.setCursor(0, 0);
        lcd.print("NO DEVICE FOUND");

        lcd.setCursor(0, 1);
        lcd.print("ENV SAFE");

        delay(4000);
    }

    restartCycle = true;
}