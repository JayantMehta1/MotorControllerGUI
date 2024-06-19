#include <Arduino.h>

#include <esp32_smartdisplay.h>
#include <ui/ui.h>

#include <iostream>
#include <string>

#include <ServoEasing.hpp>

ServoEasing myServo;
int servoLowLimit = 500;
int servoHighLimit = 2500;
//int analogPin = 35;
//int digitalPin = 21;
int digitalPin = 22;

int testAngle1 = 75;
int testAngle2 = 105;
int servospeed = 19;//the following servo speeds are too jittery: 1-15. 20-25 starts to get smoother, but too fast. 
//Would not go faster than 30. 18-20 speed at 9v, no capacitor is maybe good enough

void changeState(lv_event_t * e){

    lv_obj_t * btn = lv_event_get_target(e);
    std::string button_name; // Define a string to store the button name
        //const char* name = lv_obj_get_name(btn); // Get the name of the button
        lv_obj_t * label = lv_obj_get_child(btn, NULL);
        const char * txt = lv_label_get_text(label);
        if (txt)
            button_name = txt;
        std::cout << "Button clicked: " << button_name << std::endl;
    
    Serial.print("Servo speed: ");
    Serial.println(servospeed);  
    myServo.easeTo(testAngle1,servospeed);
    Serial.println(testAngle1);  
    delay(5000);
    Serial.print("Servo speed: ");
    Serial.println(servospeed);  
    myServo.easeTo(testAngle2,servospeed);
    Serial.println(testAngle2); 
    delay(5000);
}

void handleExposureValueChange(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    const char * text = lv_textarea_get_text(obj);  
	Serial.println(text);
	
}

void handleStart(lv_event_t * e)
{
	// Your code here
    // if(lv_textarea_get_text(ui_TextAreaMaxOnTime) == lv_textarea_get_text(ui_TextAreaExposureTime)) {
    //     lv_textarea_set_text(ui_TextArea4, lv_textarea_get_text(ui_TextAreaMaxOnTime));
    //     return;
    // }

    int max_on_time = atoi(lv_textarea_get_text(ui_TextAreaMaxOnTime));
    int exposure_time = atoi(lv_textarea_get_text(ui_TextAreaExposureTime));
    int break_time = atoi(lv_textarea_get_text(ui_TextAreaBreakTime));

    Serial.println(max_on_time);
    Serial.println(exposure_time);
    Serial.println(break_time);


    int total_time = 0;
    int remaining_exposure_time = exposure_time;
    
    while (remaining_exposure_time > 0) {
        if (remaining_exposure_time <= max_on_time) {
            total_time += remaining_exposure_time;
            remaining_exposure_time = 0;
        } else {
            total_time += max_on_time;
            remaining_exposure_time -= max_on_time;
            total_time += break_time;
        }
    }

    char str[20];
    sprintf(str, "%d", total_time);
    
    lv_textarea_set_text(ui_TextArea4, str);

}

void handleStop(lv_event_t * e)
{
	// Your code here
}


void moveToZero(lv_event_t * e){

}

void saveSettings(lv_event_t * e){

}

void startZeroCalibration(lv_event_t * e){

}

void saveZeroCalibrationOffset(lv_event_t * e){

}




void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(5000);
#endif
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    log_i("Board: %s", BOARD_NAME);
    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());

    pinMode(digitalPin, OUTPUT);
    myServo.attach(digitalPin,servoLowLimit,servoHighLimit);


    smartdisplay_init();

    __attribute__((unused)) auto disp = lv_disp_get_default();
    lv_disp_set_rotation(disp, LV_DISP_ROT_90);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    ui_init();

   

}

ulong next_millis;


void loop()
{
    auto const now = millis();
    if (now > next_millis)
    {
        next_millis = now + 500;

        char text_buffer[32];
        sprintf(text_buffer, "%lu", now);
        //lv_label_set_text(ui_lblMillisecondsValue, text_buffer);

#ifdef BOARD_HAS_RGB_LED
        // auto const rgb = (now / 2000) % 8;
        // smartdisplay_led_set_rgb(rgb & 0x01, rgb & 0x02, rgb & 0x04);
        smartdisplay_led_set_rgb(0,1,0);
#endif

#ifdef BOARD_HAS_CDS
        auto cdr = analogReadMilliVolts(CDS);
        sprintf(text_buffer, "%d", cdr);
        //lv_label_set_text(ui_lblCdrValue, text_buffer);
#endif
    }

    /*
    Serial.print("Servo speed: ");
    Serial.println(servospeed);  
    myServo.easeTo(testAngle1,servospeed);
    Serial.println(testAngle1);  
    delay(5000);
    Serial.print("Servo speed: ");
    Serial.println(servospeed);  
    myServo.easeTo(testAngle2,servospeed);
    Serial.println(testAngle2); 
    delay(5000);
*/

    lv_timer_handler();
}