// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
void ui_event_Screen1( lv_event_t * e);
extern lv_obj_t *ui_Screen1;
extern lv_obj_t *ui_LabelExposureTime;
extern lv_obj_t *ui_LabelBreakTime;
void ui_event_TextAreaExposureTime( lv_event_t * e);
extern lv_obj_t *ui_TextAreaExposureTime;
void ui_event_TextAreaBreakTime( lv_event_t * e);
extern lv_obj_t *ui_TextAreaBreakTime;
extern lv_obj_t *ui_LabelTotalTime;
extern lv_obj_t *ui_LabelMaxOnTime;
void ui_event_TextAreaMaxOnTime( lv_event_t * e);
extern lv_obj_t *ui_TextAreaMaxOnTime;
void ui_event_ButtonStartStop( lv_event_t * e);
extern lv_obj_t *ui_ButtonStartStop;
void ui_event_LabelStartStop( lv_event_t * e);
extern lv_obj_t *ui_LabelStartStop;
extern lv_obj_t *ui_LabelTotalTimeValue;
void ui_event_ButtonYes( lv_event_t * e);
extern lv_obj_t *ui_ButtonYes;
extern lv_obj_t *ui_LabelYes;
void ui_event_ButtonNo( lv_event_t * e);
extern lv_obj_t *ui_ButtonNo;
extern lv_obj_t *ui_LabelNo;
extern lv_obj_t *ui_LabelTimeRemaining;
extern lv_obj_t *ui_LabelTimeRemainingValue;
void ui_event_TextAreaPowerLevel( lv_event_t * e);
extern lv_obj_t *ui_TextAreaPowerLevel;
extern lv_obj_t *ui_LabelPowerLevel;
void ui_event_Keyboard1( lv_event_t * e);
extern lv_obj_t *ui_Keyboard1;
extern lv_obj_t *ui____initial_actions0;





void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
