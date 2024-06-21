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

lv_timer_t * countdown_timer = NULL; // Global variable to store the timer
int *remaining_time_ptr = NULL; // Global variable to store the remaining time pointer

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

void computeTotalTime() {
    int max_on_time = atoi(lv_textarea_get_text(ui_TextAreaMaxOnTime));
    int exposure_time = atoi(lv_textarea_get_text(ui_TextAreaExposureTime));
    int break_time = atoi(lv_textarea_get_text(ui_TextAreaBreakTime));

    Serial.println(max_on_time);
    Serial.println(exposure_time);
    Serial.println(break_time);

    if(max_on_time > 0) {
        // Calculate the number of full cycles and the remaining time
        int full_cycles = exposure_time / max_on_time;
        int remaining_time = exposure_time % max_on_time;

        // Compute total time
        int total_time = full_cycles * (max_on_time + break_time); // Subtract last break time
        total_time += remaining_time;

        if(remaining_time == 0) {
            total_time -= break_time;
        }

        // Convert total time to string
        char str[20];
        sprintf(str, "%d", total_time);

        // Set the label text to display the total time
        lv_label_set_text(ui_LabelTotalTimeValue, str);
    }
}

void handleExposureChangeSetBreakTime(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    const char * text = lv_textarea_get_text(obj);
    int exposure_time = atoi(text);
    int max_on_time = atoi(lv_textarea_get_text(ui_TextAreaMaxOnTime));

    if(max_on_time >= exposure_time) {
        lv_textarea_set_text(ui_TextAreaBreakTime, "0");
        lv_obj_add_flag(ui_LabelBreakTime, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_TextAreaBreakTime, LV_OBJ_FLAG_HIDDEN);
    } else {
        // Check if objects are not hidden before attempting to clear the hidden flag
        if (lv_obj_has_flag(ui_LabelBreakTime, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(ui_LabelBreakTime, LV_OBJ_FLAG_HIDDEN);
        }

        if (lv_obj_has_flag(ui_TextAreaBreakTime, LV_OBJ_FLAG_HIDDEN)) {
            lv_textarea_set_text(ui_TextAreaBreakTime, "30");
            lv_obj_clear_flag(ui_TextAreaBreakTime, LV_OBJ_FLAG_HIDDEN);
        }
    }

    computeTotalTime();
}

void handleMaxOnTimeChangeSetBreakTime(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    const char * text = lv_textarea_get_text(obj);
    Serial.println("the max on time text is:");
    Serial.println(text);
    if(text == "" || text == "0") {
        lv_textarea_set_text(ui_TextAreaMaxOnTime, "1");
    }
    int max_on_time = atoi(lv_textarea_get_text(ui_TextAreaMaxOnTime));
    int exposure_time = atoi(lv_textarea_get_text(ui_TextAreaExposureTime));

    if(max_on_time >= exposure_time) {
        lv_textarea_set_text(ui_TextAreaBreakTime, "0");
        lv_obj_add_flag(ui_LabelBreakTime, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_TextAreaBreakTime, LV_OBJ_FLAG_HIDDEN);
    } else {
        // Check if objects are not hidden before attempting to clear the hidden flag
        if (lv_obj_has_flag(ui_LabelBreakTime, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(ui_LabelBreakTime, LV_OBJ_FLAG_HIDDEN);
        }

        if (lv_obj_has_flag(ui_TextAreaBreakTime, LV_OBJ_FLAG_HIDDEN)) {
            lv_textarea_set_text(ui_TextAreaBreakTime, "30");
            lv_obj_clear_flag(ui_TextAreaBreakTime, LV_OBJ_FLAG_HIDDEN);
        }
    }
    computeTotalTime();
}

void handleBreakTimeValueChange(lv_event_t * e)
{
    computeTotalTime();
}

void handleKeyboardIssue(lv_event_t * e)
{
    lv_obj_t * kb = lv_event_get_target(e); // Get the keyboard object
    uint32_t btn_id = lv_keyboard_get_selected_btn(kb); // Get the pressed button ID
    const char * btn_text = lv_keyboard_get_btn_text(kb, btn_id); // Get the button text

    if(btn_id == 7) {
        lv_obj_add_flag(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN);
        // _ui_flag_modify( ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
    }
}

void handleMakeCheckMarkWork(lv_event_t * e)
{
    Serial.println("Handling LVGL event: your maybe an idiot"); // Debug output
    
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_KEY) {
        uint32_t key = lv_indev_get_key(lv_indev_get_act()); // Assuming lv_indev is properly set up
        
        Serial.print("Key pressed (LVGL code): ");
        Serial.println(key); // Debug output
        
        // Replace LV_KEY_ENTER with the correct LVGL key code for the check mark button
        // if (key == LV_KEY_CHECKMARK) {
        //     Serial.println("Check mark button pressed: your an idiot"); // Debug output
        // }
    }
}

// Function to stop the countdown timer and reset
void handleSystemStop() {
    Serial.println("System has stopped!!!");

    // Check if the timer is running
    if (countdown_timer != NULL) {
        lv_timer_del(countdown_timer); // Stop the timer
        countdown_timer = NULL; // Reset the timer pointer
    }

    // Free allocated memory for the remaining time
    if (remaining_time_ptr != NULL) {
        free(remaining_time_ptr);
        remaining_time_ptr = NULL;
    }

    // Reset the ui_TimeRemaining label
    lv_label_set_text(ui_LabelTimeRemainingValue, "");

    lv_obj_add_flag(ui_LabelTimeRemaining, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelTimeRemainingValue, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(ui_LabelStartStop, "Start");


    // Additional logic to handle the system stop can be added here
}

// function to be called when the countdown completes
void onCountdownComplete() {
    Serial.println("Countdown complete!");
    handleSystemStop();
}

// Timer callback function to update the remaining time
void timer_callback(lv_timer_t * timer) {
    (*remaining_time_ptr)--;

    // Update label
    char time_str[10];
    sprintf(time_str, "%d", *remaining_time_ptr);
    lv_label_set_text(ui_LabelTimeRemainingValue, time_str);

    if (*remaining_time_ptr <= 0) {
        handleSystemStop();
    }
}

// Function to start the system and the countdown timer
void handleSystemStart() {
    Serial.println("System has started!!!");

    lv_obj_clear_flag(ui_LabelTimeRemaining, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelTimeRemainingValue, LV_OBJ_FLAG_HIDDEN);

     // Read the initial time from ui_LabelTotalTime
    const char * text = lv_label_get_text(ui_LabelTotalTimeValue);
    int initial_time = atoi(text);

    // Allocate memory to store the remaining time
    remaining_time_ptr = (int *)malloc(sizeof(int));
    *remaining_time_ptr = initial_time;

    // Create a timer that calls timer_callback every second
    countdown_timer = lv_timer_create(timer_callback, 1000, remaining_time_ptr);

    // Initialize the ui_TimeRemaining label with the initial time
    char time_str[10];
    sprintf(time_str, "%d", initial_time);
    lv_label_set_text(ui_LabelTimeRemainingValue, time_str);
}

void handleStartStopExecution() {
    const char * text = lv_label_get_text(ui_LabelStartStop);
    Serial.println(text);

    // change start/stop as needed and show
    if(strcmp(text, "Start") == 0) {
        lv_label_set_text(ui_LabelStartStop, "Stop");
        handleSystemStart();
    } else {
        lv_label_set_text(ui_LabelStartStop, "Start");
        handleSystemStop();
    }

    lv_obj_clear_flag(ui_LabelStartStop, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ButtonStartStop, LV_OBJ_FLAG_HIDDEN);
}

void handleNo(lv_event_t * e)
{
    // Just hide yes/no, show the start/stop again (no changes or executions)
    lv_obj_add_flag(ui_LabelYes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ButtonYes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelNo, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ButtonNo, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelStartStop, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ButtonStartStop, LV_OBJ_FLAG_HIDDEN);
}

void handleYes(lv_event_t * e)
{
    // Hide yes and no
    lv_obj_add_flag(ui_LabelYes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ButtonYes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelNo, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ButtonNo, LV_OBJ_FLAG_HIDDEN);

    // handle start/stop execution
    handleStartStopExecution();
    
    }


void handleStartStop(lv_event_t * e)
{
    // Show yes/no, hide the start/stop
    lv_obj_add_flag(ui_ButtonStartStop, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelStartStop, LV_OBJ_FLAG_HIDDEN); 

    lv_obj_clear_flag(ui_LabelYes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ButtonYes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelNo, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ButtonNo, LV_OBJ_FLAG_HIDDEN);

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