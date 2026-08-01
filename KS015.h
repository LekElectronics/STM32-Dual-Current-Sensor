#ifndef KS015_H_
#define KS015_H_

#include <Arduino.h>
#include "CurrentSense.h"
#include "HeartbeatLED.h"

// Number of current-sensing channels on the PCBA
#define NUM_CHANNELS 2

// Channel 1 configuration
#define CH1_I2C_ADDRESS   0x40   // I2C address of INA260 on channel 1
#define CH1_ALERT_PIN     PB11   // Alert interrupt pin for channel 1
#define CH1_SW_PIN        PB0    // Output switch control pin for channel 1

// Channel 2 configuration
#define CH2_I2C_ADDRESS   0x41   // I2C address of INA260 on channel 2
#define CH2_ALERT_PIN     PB12   // Alert interrupt pin for channel 2
#define CH2_SW_PIN        PB1    // Output switch control pin for channel 2

// Heartbeat LED pin on the PCBA
#define HEARTBEAT_LED_PIN PA3

// Channel selection enum
typedef enum {
    CHANNEL1 = 0,  // maps to channels[0]
    CHANNEL2 = 1   // maps to channels[1]
} KS015_Channel;

// User callback type for alert event
typedef void (*AlertCallback)(KS015_Channel ch);

class KS015 {
public:
    /**
     * Constructor
     * Initializes internal CurrentSense channels and HeartbeatLED objects.
     * Actual hardware initialization occurs in begin().
     */
    KS015();

    /**
     * begin()
     * Initializes the PCBA hardware:
     * - Initializes each INA260 channel
     * - Configures alert interrupt pins
     * - Configures output switch pins
     * - Initializes heartbeat LED
     */
    void begin();

    /**
     * enableOutput(ch)
     * Turns on the output switch for the specified channel.
     */
    void enableOutput(KS015_Channel ch);

    /**
     * disableOutput(ch)
     * Turns off the output switch for the specified channel.
     */
    void disableOutput(KS015_Channel ch);

    /**
     * getCurrent(ch)
     * Returns the measured current (in mA) for the specified channel.
     */
    float getCurrent(KS015_Channel ch);

    /**
     * getVoltage(ch)
     * Returns the measured bus voltage (in V) for the specified channel.
     */
    float getVoltage(KS015_Channel ch);

    /**
     * getPower(ch)
     * Returns the measured power (in mW) for the specified channel.
     */
    float getPower(KS015_Channel ch);

    /**
     * showInformation()
     * Returns a string showing important information about the KS015.
     */
    const char *showInformation(void);

    /**
     * setAlertCallback()
     * Registers a callback function that is called
     * when an alert event occurs on any channel.
     */
     void setAlertCallback(AlertCallback cb);


private:
    // Static global instance pointer for ISRs
    static KS015 *instance;

    /**
     * Alert1_ISR / Alert2_ISR
     * Static interrupt service routines for each channel.
     * Triggered when the INA260 alert occurs.
     */
    static void Alert1_ISR();
    static void Alert2_ISR();

    /**
     * updateHeartbeat()
     * Toggles the heartbeat LED to indicate system activity.
     * Called from a 1ms timer.
     */
    void updateHeartbeat();

    /**
     * handleAlert(ch)
     * Internal helper function to process an alert event for the given channel.
     * Called by the channel-specific ISR.
     * If a user callback is registered (alert_cb), it is invoked.
     * @param ch The channel on which the alert event occurred (CHANNEL1, CHANNEL2, etc.)
     */
    void handleAlert(KS015_Channel ch);

    /**
    * Timer4 ISR - 1ms interval
    * Monitors the 2 channels and checks for any dangerous conditions that may result in 
    * overheating the switch MOSFET
    */
    static void Timer4_ISR();

    // Callback function pointer for alert events
    static AlertCallback alert_cb;

    // Array of current-sensing channels
    CurrentSense channels[NUM_CHANNELS];

    // Heartbeat LED object
    HeartbeatLED heartbeat;
};

#endif
