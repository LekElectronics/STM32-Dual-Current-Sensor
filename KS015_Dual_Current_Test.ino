#include "KS015.h"

char FW_Version[] = "KS015 Test v2.1";

#define STRESS_TEST_MS 2000

KS015 ks015;

void alert1_ISR(void)
{
  Serial.println("Alert 1");
}

void alert2_ISR(void)
{
  Serial.println("Alert 2");
}

void displayMenu(void)
{
  Serial.println("");
  Serial.println("KS015 Dual Current Sensor Test Menu");
  Serial.println("===================================");
  Serial.println("Press key:");
  Serial.println("  F = Read firmware version");
  Serial.println("  I = Show information");
  Serial.println("  1 = Read INA260 Channel 1");
  Serial.println("  2 = Read INA260 Channel 2");
  Serial.println("  A = Activate channel 1 output");
  Serial.println("  a = Deactivate channel 1 output");
  Serial.println("  B = Activate channel 2 output");
  Serial.println("  b = Deactivate channel 2 output");
  Serial.println("  S = Activate stress test channel 1");
  Serial.println("  s = Deactivate stress test channel 1");
  Serial.println("  T = Activate stress test channel 2");
  Serial.println("  t = Deactivate stress test channel 2");
}

void displayINA260Readings(KS015_Channel ch)
{
  Serial.println("");
  Serial.print("Current: ");
  Serial.print(ks015.getCurrent(ch));
  Serial.println(" mA");

  Serial.print("Bus Voltage: ");
  Serial.print(ks015.getVoltage(ch));
  Serial.println(" mV");

  Serial.print("Power: ");
  Serial.print(ks015.getPower(ch));
  Serial.println(" mW");

  Serial.println();
}

// Clears all bytes currently in the Serial buffer
void clearSerialBuffer() {
    while (Serial.available() > 0) {
        Serial.read();  // discard byte
    }
}

void processSingleStressTest(bool is_enabled, bool *last_enabled, uint32_t *ms, KS015_Channel ch)
{
  if(is_enabled)
  {
    if(*last_enabled == false)
    {
      *ms = millis();
    }
    *last_enabled = true;

    uint32_t this_ms = millis();
    if(this_ms < (*ms + STRESS_TEST_MS))
    {
      ks015.enableOutput(ch);
    }
    else if(this_ms < (*ms + 2*STRESS_TEST_MS))
    {
      ks015.disableOutput(ch);
    }
    else
    {
      *ms = this_ms;
    }
  }
  else
  {
    if((is_enabled == false)&&(*last_enabled == true))
    {
      *last_enabled = false;
      ks015.disableOutput(ch);
    }
  }
}

bool stress1_enabled = false;
bool stress2_enabled = false;
void processStressTest()
{
  static uint32_t ms_ch1_start = 0;
  static uint32_t ms_ch2_start = 0;
  static bool last_enabled_1 = false;
  static bool last_enabled_2 = false;

  processSingleStressTest(stress1_enabled, &last_enabled_1, &ms_ch1_start, CHANNEL1);
  processSingleStressTest(stress2_enabled, &last_enabled_2, &ms_ch2_start, CHANNEL2);

}

void setup() {
  // put your setup code here, to run once:
  ks015.begin();  
  Serial.println("Starting");
  displayMenu();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check if any data is available on the Serial port
  if (Serial.available() > 0) {
    // Read the incoming character without removing it from the buffer
    char incoming = Serial.read();
    switch(incoming)
    {
      case 'F':
        Serial.println(FW_Version);
        break;
      case 'I':
        Serial.println(ks015.showInformation());
        break;
      case '1':
        displayINA260Readings(CHANNEL1);
        break;
      case '2':
        displayINA260Readings(CHANNEL2);
        break;
      case 'A':
        ks015.enableOutput(CHANNEL1);
        break;
      case 'a':
        ks015.disableOutput(CHANNEL1);
        break;
      case 'B':
        ks015.enableOutput(CHANNEL2);
        break;
      case 'b':
        ks015.disableOutput(CHANNEL2);
        break;
      case 'S':
        stress1_enabled = true;
        break;
      case 's':
        stress1_enabled = false;
        break;
      case 'T':
        stress2_enabled = true;
        break;
      case 't':
        stress2_enabled = false;
        break;
      default:
        break;
    }
    delay(500);
    clearSerialBuffer();
    displayMenu();
  }
  processStressTest();
}
