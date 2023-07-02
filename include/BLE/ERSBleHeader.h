// BLE

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

int scanTime = 1;               // duration in seconds
const int delayBLEScan = 100;   // delay between scans
const int RSSI_1M = -53;        // RSSI at 1 meter distance
const int SIGNAL_LOSS = 2;      // Signal loss factor per meter
const int RSSI_THRESHOLD = -70; // RSSI threshold for detection

BLEScan *pBLEScan;

double rssiToDistance(int rssi)
{
  double signalLoss = rssi - RSSI_1M;
  double distance = pow(10.0, signalLoss / (-10.0 * SIGNAL_LOSS));
  return distance;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.haveManufacturerData() == true)
    {
      int rssi = advertisedDevice.getRSSI();
      if (rssi < RSSI_THRESHOLD)
      {
        return;
      }

      // check that advertised name is "78-beacon-01" and if its not return
      if (advertisedDevice.getName() != "78-beacon-01")
      {
        return;
      }
      Serial.print("RSSI: ");
      Serial.println(rssi);

      std::string strManufacturerData = advertisedDevice.getManufacturerData();

      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

      if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
      {
        if (advertisedDevice.haveName())
        {
          Serial.print("Device name: ");
          Serial.println(advertisedDevice.getName().c_str());
        }
        BLEBeacon oBeacon = BLEBeacon();
        oBeacon.setData(strManufacturerData);
        Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
        Serial.print("iBeacon RSSI: ");
        Serial.println(rssi);
        // Convert RSSI to distance in centimeters
        // Convert RSSI to distance in centimeters
        double distance = rssiToDistance(rssi);
        Serial.print("Distance: ");
        Serial.println(distance);
        Serial.println();
      }
    }
  }
};