#include "qlock.h";

#include <EEPROM.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.pool.ntp.org", 7200);

int lastRefreshTime = 0;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

String formatTime(const RtcDateTime& dt)
{
    char datestring[10];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u:%02u"),
            dt.Hour(),
            dt.Minute());
    return datestring;
}

String formatDate(const RtcDateTime& dt)
{
    char datestring[12];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%04u-%02u-%02u"),
            dt.Year(),
            dt.Month(),
            dt.Day());
    return datestring;
}

String processRoot(const String& text) {
    if (text == "VERSION") {
        return VERSION;
    } else if (text == "TIME") {
        return formatTime(getTime());
    } else if (text == "POWER") {
        Serial.println(IsOn());
        return IsOn() ? "checked" : "";
    } else if (text == "COLOR") {
        char outputString[9];
        itoa(ledColor, outputString, 16);
        return "#" + String(outputString);
    } else if (text == "AUTO_B") {
        return "checked";
    } else if (text == "NO_TRANSITION_SELECTED") {
        return String();
    } else if (text == "FADE_TRANSITION") {
        return "selected";
    }
    return String();
}

String processTime(const String& text) {
    if (text == "AUTO") {
        return autoTime ? "checked" : "";
    } else if (text == "DATE") {
        return formatDate(getTime());
    } else if (text == "TIME") {
        return formatTime(getTime());
    }
    return String();
}

String processUpdate(const String& text) {
    if (text == "VERSION") {
        return VERSION;
    }
    return String();
}

void beginServer() {
    if (!SPIFFS.begin()) {
        Serial.println("Can't initialize LittleFS");
    }

    server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, processRoot);
    });

    server->on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, processRoot);
    });

    server->on("/time.html", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/time.html", String(), false, processTime);
    });

    server->on("/update.html", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/update.html", String(), false, processUpdate);
    });

    server->on("/api/update", HTTP_POST, [&](AsyncWebServerRequest *request) {
        startUpdate = true;
        request->send(200, String(), "ok");
    });

    server->on("/api/version", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, String(), VERSION);
    });

    server->on("/api/reset", HTTP_POST, [&](AsyncWebServerRequest *request) {
        request->send(200, String(), "ok");
        
        restartESP = true;
    });

    server->on("/api/read", HTTP_GET, [&](AsyncWebServerRequest *request) {
       int sensor_brightness = analogRead(A0);

        double brightness_percentage = 1.0 - (double)sensor_brightness / 1024.0;

        double test = MIN_BRIGHTNESS + ((MAX_BRIGHTNESS-MIN_BRIGHTNESS) * brightness_percentage);

        request->send(200, String(), String(test));
    });

    server->on("/api/settings", HTTP_POST, [&](AsyncWebServerRequest *request) {
        if (request->hasParam("power") && request->getParam("power")->value() == "true") {
            SetOn(true);
        } else if (request->hasParam("power") && request->getParam("power")->value() == "false") {
            SetOn(false);
        }

        if (request->hasParam("color")) {
            Serial.println(request->getParam("color")->value());
            uint32_t color = atol(request->getParam("color")->value().c_str());
            if (color == 16777215) {
                SetColor(strip.Color(0,0,0,255));
            } else {
                SetColor(color);
            }
        }

        if (request->hasParam("autoTime") && request->getParam("autoTime")->value() == "true") {
            SetAutoTime(true);
        } else if (request->hasParam("autoTime") && request->getParam("autoTime")->value() == "false") {
            SetAutoTime(false);
        }

        if (request->hasParam("time")) {
            String time = request->getParam("time")->value();
            String hour = time.substring(0, 2);
            String minute = time.substring(2, 4);
            
            RtcDateTime now = getTime();

            RtcDateTime compiledTime = RtcDateTime(now.Year(), now.Month(), now.Day(), hour.toInt(), minute.toInt(), now.Second());
        }

        request->send(200, String(), "ok");
    });

    server->on("/api/factory_reset", HTTP_POST, [&](AsyncWebServerRequest *request) {
        WiFi.disconnect();

        request->send(200, String(), "ok");
        
        restartESP = true;
    });

    server->onNotFound(notFound);
    
    server->begin();
    timeClient.begin();
}

void loopServer() {
    if(millis() - lastRefreshTime >= 60000 && autoTime)
	{
		lastRefreshTime += 60000;
        timeClient.update();

        Serial.println("Get new RTC Time");

        RtcDateTime compiled = RtcDateTime(timeClient.getEpochTime()-946684800);

        setRtcTime(compiled);
    }

}