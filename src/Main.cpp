#include "OtaUpdate.h"
#include "RgbLed.h"
#include "Secrets.h"
#include "TwaiTaskBased.h"
#include "Globals.h"
#include <Arduino.h>
#include <driver/gpio.h>

RgbLed statusLed(8);
OtaUpdate otaUpdate(statusLed, 180000, ssid, password);

bool currentTxErrors = false;

void onCanRx(const twai_message_t &msg) {
  // We received a CAN bus message that may mean an update is available.
  if (msg.identifier == 0) {
    Serial.println("Got message for someone to update");
    // Create a char to hold the name of who the update is for.
    char updateForHostName[14];
    // Get the current host name of this device
    String currentHostName = otaUpdate.getHostName();
    // Fomrat the string of the upate so it matches the pattern used by espressif
    sprintf(updateForHostName, "esp32c6-%X%X%X", msg.data[0], msg.data[1],
            msg.data[2]);
    Serial.println(updateForHostName);
    // An update is ready for this device if the two match.
    if (currentHostName.equals(updateForHostName)) {
      debugln("Need to wait for OTA");
      // Pause and see if an OTA is needed
      otaUpdate.waitForOta();
    }
  }
}

void onCanTx(bool ok) {
  currentTxErrors = !ok;
  debugln(ok ? "TX OK" : "TX FAIL");
}

void setup() {
  // Always start serial even if only to output that debugging is disabled.
  Serial.begin(115200);
// These should be the only two Serial.print statements. All others will use
// debug, debugln, and debugf so they are removed for production.
#if DEBUG == 0
  Serial.println(
      "Debugging is disabled no more output will be written here...");
#else
  Serial.println("Debugging started");
#endif
  // Initialize the onboard LED first so we can use it to communicate status.
  statusLed.begin();
  statusLed.setBrightnessPercent(1);
  // Now set the callbacks and initialize the CAN helper.
  TwaiTaskBased::onReceive(onCanRx);
  TwaiTaskBased::onTransmit(onCanTx);
  TwaiTaskBased::begin(GPIO_NUM_15, GPIO_NUM_14, 500000);
  statusLed.green();
  debugln("Setup done");
}

void loop() {
  
}
