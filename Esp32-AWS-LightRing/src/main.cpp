#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <FastLED.h>

/**
 * On 11/3/2020 I was able to get this example working.
 * This comes from the example at: https://aws.amazon.com/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/
 * The esp32 was able to connect to aws's mqtt
 * I was able to publish a message to esp32/sub in the aws console and i observed the message show in the 
 * serial monitor of the esp32.
 * 
 * Per the example I also a pre-existing project with 2xlambda functions, rule, and api endpoint
 * 
 * 
 * Takeaways
 * There are canned apps I can use
 * Apparently a single app can configure an api gw, can deploy multiple lambda functions, can setup "rules"
 * 
 * Was able to publish a message to the esp32
 * curl -d '{"text":"Hello world!"}' -H "Content-Type: application/json" -X POST https://xxxxxxxxx.execute-api.us-east-2.amazonaws.com/Prod/publish
 * 
 */

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

// LED STUFF
const int NUM_LEDS = 16;
CRGBArray<NUM_LEDS> leds;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGB backgroundColor = CRGB::Black; //CRGB::Aqua; // CRGB::Black; 
CRGB foregroundColor = CRGB::Crimson;

#define LED_PIN     5
#define NUM_LEDS    16
#define BRIGHTNESS  20
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

void messageHandler(String &topic, String &payload);

// non-led stuff
#define BLUE_LED 2

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to wifi");

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(1000);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");

  digitalWrite(BLUE_LED,HIGH);
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = 123; // analogRead(0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void updateLEDs(bool ledOn[]) {
  for( int i=0; i<NUM_LEDS; ++i ) {
    if( ledOn[i] ) {
      leds[i] = foregroundColor;
    }
    else {
      leds[i] = backgroundColor;
    }
  }

  FastLED.show();
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<300> doc;
  deserializeJson(doc, payload);

  // Is this a message with a list of button values
  if( doc.containsKey("buttons") ) {
    JsonArray buttons = doc["buttons"].as<JsonArray>();

    // Here we can process the button message
    Serial.printf("Got a buttons message, num buttons: %d\n", buttons.size());

    bool ledOn[NUM_LEDS];
    for( int i=0; i<NUM_LEDS && i<buttons.size(); ++i ) {
      ledOn[i] = buttons[i];
      Serial.printf("led[%d] = %d, ", i, ledOn[i]);
    }
    Serial.println();

    updateLEDs(ledOn);
  }

}

void setupLEDs() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 200);
    FastLED.setBrightness(  BRIGHTNESS );

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    for( int i=0; i<NUM_LEDS; ++i ) {
      leds[i] = backgroundColor;
    }

    FastLED.show();
}

void setup() {
  Serial.begin(9600);

  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED,LOW);

  connectAWS();

  setupLEDs();
}

void loop() {
  publishMessage();
  client.loop();
  delay(500);
}
