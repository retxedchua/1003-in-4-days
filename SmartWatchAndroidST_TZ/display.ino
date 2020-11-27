const uint8_t displayStateHome = 0x01;
const uint8_t displayStateMenu = 0x02;
const uint8_t displayStateEditor = 0x03;
const uint8_t displayStateStopWatch = 0x04;

uint8_t currentDisplayState = displayStateHome;
void (*menuHandler)(uint8_t) = NULL;
uint8_t (*editorHandler)(uint8_t, int*, char*, void (*)()) = NULL;
uint8_t (*stopWatchHandler)(uint8_t, int*, char*, void (*)()) =NULL;


const uint8_t upButton = TSButtonUpperRight;
const uint8_t tempshortcut = TSButtonUpperRight;
const uint8_t stepshortcut = TSButtonUpperLeft;
const uint8_t downButton = TSButtonLowerRight;
const uint8_t selectButton = TSButtonLowerLeft;
const uint8_t backButton = TSButtonUpperLeft;
const uint8_t menuButton = TSButtonLowerLeft;
const uint8_t viewButton = TSButtonLowerRight;
const uint8_t clearButton = TSButtonLowerRight;
const uint8_t startButton = TSButtonLowerLeft;
const uint8_t lapButton = TSButtonLowerRight;
const uint8_t resetButton = TSButtonUpperRight;

void buttonPress(uint8_t buttons) {
  if (currentDisplayState == displayStateHome) {
    if (buttons == viewButton) {
      menuHandler = viewNotifications;
      menuHandler(0);
    } else if (buttons == menuButton) {
      menuHandler = viewMenu;
      menuHandler(0);
    }
    else if (buttons == tempshortcut) {
      menuHandler = displayTemp;
      menuHandler(0);
    }
    else if (buttons == stepshortcut) {
      menuHandler = displayFootTracker;
      menuHandler(0);
    }
  } else if (currentDisplayState == displayStateMenu) {
    if (menuHandler) {
      menuHandler(buttons);
    }
  } else if (currentDisplayState == displayStateEditor) {
    if (editorHandler) {
      editorHandler(buttons, 0, 0, NULL);
    }
  } else if (currentDisplayState == displayStateStopWatch){
      if(buttons == startButton){
          stopWatchLoop(buttons);
          }
      }
}

void stopWatch(uint8_t button) {
  display.clearWindow(0, 12, 96, 64);
  currentDisplayState = displayStateStopWatch;
  display.setFont(font10pt);
  display.setCursor(0, menuTextY[0]);
  display.println("Stop watch");
  display.setFont(font22pt);
  display.setCursor(0,menuTextY[2]);
  display.print("00:00:00");
  display.setFont(font10pt);
  display.print("00");
  
  if (button == backButton) {
    display.clearWindow(0, 12, 96, 64);
    menuHandler = viewMenu;
    menuHandler(0);
  }
}

void stopWatchLoop(uint8_t button){
  int cs1, cs10, s1, s10, m1, m10, h1, h10;
  uint32_t stopWstart, pauseTime, lapTime, difference, totalPauseTime;
  boolean pause = false,reset = false;
  byte pressed;
  uint8_t buttonReleasedtwo = 1;
  cs1 = 0;
  cs10 = 0;
  s1 = 0;
  s10 = 0;
  m1 = 0;
  m10 = 0;
  h1 = 0;
  h10 = 0;
  stopWstart = millis();
  difference = 0;
  pauseTime = 0;
  lapTime = 0;
  totalPauseTime=0;
  while(true){
  difference = millis()-totalPauseTime-stopWstart;
  h10 = difference / 36000000;      //3600 * 1000 * 10
  difference %= 36000000;  //3600 * 1000 * 10
  h1 = difference / 3600000;
  difference %= 3600000;
  m10 = difference / 600000;
  difference %= 600000;
  m1 = difference / 60000;
  difference %= 60000;
  s10 = difference / 10000;
  difference %= 10000;
  s1 = difference / 1000;
  difference %= 1000;
  cs10 = difference / 100;
  difference %= 100;
  cs1 = difference / 10;
  if(pause == false && h10 <10){
    display.setFont(font22pt);
    display.setCursor(0,menuTextY[2]);
    display.print(h10);
    display.print(h1);
    display.print(":");
    display.print(m10);
    display.print(m1);
    display.print(":");
    display.print(s10);
    display.print(s1);
    display.setFont(font10pt);
    display.print(cs10);
    display.print(cs1); 
  }
  
  pressed = display.getButtons();
  if (buttonReleasedtwo && pressed) {
      if(pressed == startButton){
        if(pause == true){
          if(reset == true){
            stopWstart = millis();
            reset = false;
          }else{
            totalPauseTime = millis()- pauseTime;
          }
          pause = false;
        }else{
          pauseTime = millis();
          pause = true;
        }
      } else if(pressed == resetButton){
        display.clearWindow(0, menuTextY[3], 96, 32);
        display.setFont(font22pt);
        display.setCursor(0,menuTextY[2]);
        display.print("00:00:00");
        display.setFont(font10pt);
        display.print("00");
        difference = 0;
        pauseTime = 0;
        pause = true;
        reset = true;
      } else if(pressed == lapButton && pause == false){
        display.setFont(font10pt);
        display.setCursor(66,menuTextY[0]); 
        display.print(m10);
        display.print(m1);
        display.print(":");
        display.print(s10);
        display.print(s1);
      } else if (pressed == backButton){
        break;
      }
    buttonReleasedtwo = 0;
  }
    if (!buttonReleasedtwo && !(pressed & 0x0F)) {
      buttonReleasedtwo = 1;
    }
    
  }
  
   display.clearWindow(0, 12, 96, 64);
   menuHandler = viewMenu;
   menuHandler(0);
  
}

void Timer(uint8_t button)  {
   display.clearWindow(0, 12, 96, 64);

}
void displayTemp(uint8_t button) {
  
    currentDisplayState = displayStateMenu;
   
   display.clearWindow(0, 12, 96, 64);
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.fontColor(defaultFontColor, defaultFontBG);
    
     display.setCursor(0, menuTextY[0]);
     display.println("Temp:");
     display.setFont(liberationSansNarrow_16ptFontInfo);
     display.setCursor(10, menuTextY[1]);
     display.println(temp);
     display.println("c");
     display.setFont(font10pt);
      char backStr[] = "Back >";
      int Xpos = 95 - display.getPrintWidth(backStr);
      display.setCursor(Xpos, menuTextY[3]);
      display.print(backStr);
 
    if (button == clearButton) {//actually back?
      currentDisplayState = displayStateHome;
      initHomeScreen();
    }
      
}

void displayFootTracker(uint8_t button) {
  
    currentDisplayState = displayStateMenu;
   
   display.clearWindow(0, 12, 96, 64);
    display.setFont(font10pt);
    display.fontColor(defaultFontColor, defaultFontBG);
    
     display.setCursor(0, menuTextY[0]);
     display.println("Steps:");
     display.setFont(liberationSansNarrow_16ptFontInfo);
     display.setCursor(30, menuTextY[1]);
     display.println(steptracker);
     display.setFont(font10pt);
      char backStr[] = "Back >";
      int Xpos = 95 - display.getPrintWidth(backStr);
      display.setCursor(Xpos, menuTextY[3]);
      display.print(backStr);
 
    if (button == clearButton) {//actually back?
      currentDisplayState = displayStateHome;
      initHomeScreen();
    } 
      
}

void viewNotifications(uint8_t button) {
  if (!button) {
    if (menu_debug_print)SerialMonitorInterface.println("viewNotificationsInit");
    currentDisplayState = displayStateMenu;
    display.clearWindow(0, 12, 96, 64);
    display.setFont(font10pt);
    display.fontColor(defaultFontColor, defaultFontBG);
    if (amtNotifications) {
      if (menu_debug_print)SerialMonitorInterface.println("amtNotifications=true");
      //display.setCursor(0, menuTextY[1]);
      //display.setCursor(0, 0);
      //display.print(ANCSNotificationTitle());

      int line = 0;
      int totalMessageChars = strlen(notificationLine2);
      int printedChars = 0;
      while (printedChars < totalMessageChars && line < 3) {
        char tempPrintBuff[40] = "";
        int tempPrintBuffPos = 0;
        while (display.getPrintWidth(tempPrintBuff) < 90 && printedChars < totalMessageChars) {
          if (!(tempPrintBuffPos == 0 && notificationLine2[printedChars] == ' ')) {
            tempPrintBuff[tempPrintBuffPos] = notificationLine2[printedChars];
            tempPrintBuffPos++;
          }
          printedChars++;
          tempPrintBuff[tempPrintBuffPos] = '\0';
        }
        display.setCursor(0, menuTextY[line]);
        display.print((char*)tempPrintBuff);
        line++;
      }



      display.setCursor(0, menuTextY[3]);
      display.print(F("< "));
      display.print("Clear");

      char backStr[] = "Back >";
      int Xpos = 95 - display.getPrintWidth(backStr);
      display.setCursor(Xpos, menuTextY[3]);
      display.print(backStr);
    } else {
      if (menu_debug_print)SerialMonitorInterface.println("amtNotifications=false");
      display.setCursor(0, menuTextY[0]);
      display.print(F("  No notifications."));
      char backStr[] = "Back >";
      int Xpos = 95 - display.getPrintWidth(backStr);
      display.setCursor(Xpos, menuTextY[3]);
      display.print(backStr);
    }
  } else {
    if (button == clearButton) {//actually back?
      currentDisplayState = displayStateHome;
      initHomeScreen();
    } else if (button == selectButton) { //do action
      amtNotifications = 0;
      //ANCSPerformNotificationNegativeAction();
      currentDisplayState = displayStateHome;
      initHomeScreen();
    }
  }
}


void initHomeScreen() {
  display.clearWindow(0, 12, 96, 64);
  rewriteTime = true;
  rewriteMenu = true;
  updateMainDisplay();
}

uint8_t lastDisplayedDay = -1;
uint8_t lastDisplayedMonth = -1;
uint8_t lastDisplayedYear = -1;

void updateDateDisplay() {
#if defined (ARDUINO_ARCH_AVR)
  int currentDay = day();
  int currentMonth = month();
  int currentYear = year();
#elif defined(ARDUINO_ARCH_SAMD)
  int currentDay = RTCZ.getDay();
  int currentMonth = RTCZ.getMonth();
  int currentYear = RTCZ.getYear();
#endif
  if ((lastDisplayedDay == currentDay) &&
      (lastDisplayedMonth == currentMonth) &&
      (lastDisplayedYear == currentYear))
    return;
  lastDisplayedDay = currentDay;
  lastDisplayedMonth = currentMonth;
  lastDisplayedYear = currentYear;
  display.setFont(font10pt);
  display.fontColor(defaultFontColor, defaultFontBG);
  display.setCursor(2, 2);
#if defined (ARDUINO_ARCH_AVR)
  display.print(dayShortStr(weekday()));
  display.print(' ');
  display.print(month());
  display.print('/');
  display.print(day());
  display.print(F("  "));
#elif defined(ARDUINO_ARCH_SAMD)
  const char * wkday[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  time_t currentTime = RTCZ.getEpoch();
  struct tm* wkdaycalc = gmtime(&currentTime);
  display.print(wkday[wkdaycalc->tm_wday]);
  display.print(' ');
  display.print(RTCZ.getMonth());
  display.print('/');
  display.print(RTCZ.getDay());
  display.print(F("  "));
#endif
  ble_connection_displayed_state = ~ble_connection_state;
  updateBLEstatusDisplay();
}

void updateMainDisplay() {
  if (lastSetBrightness != brightness) {
    display.setBrightness(brightness);
    lastSetBrightness = brightness;
  }
  updateDateDisplay();
  updateBLEstatusDisplay();
  displayBattery();
  if (currentDisplayState == displayStateHome) {
    updateTimeDisplay();
    if (rewriteMenu || lastAmtNotificationsShown != amtNotifications) {
      lastAmtNotificationsShown = amtNotifications;
      display.setFont(font10pt);
      display.clearWindow(0, menuTextY[2], 96, 13);
      if (amtNotifications) {
        int printPos = 48 - (display.getPrintWidth(notificationLine1) / 2);
        if (printPos < 0)printPos = 0;
        display.setCursor(printPos, menuTextY[2]);
        display.print(notificationLine1);
      }
      display.setCursor(0, menuTextY[3]);
      display.print(F("< Menu          "));
      char viewStr[] = "View >";
      int Xpos = 95 - display.getPrintWidth(viewStr);
      display.setCursor(Xpos, menuTextY[3]);
      display.print(viewStr);
      rewriteMenu = false;
    }
  }
  lastMainDisplayUpdate = millisOffset();
}


uint8_t lastAMPMDisplayed = 0;
uint8_t lastHourDisplayed = -1;
uint8_t lastMinuteDisplayed = -1;
uint8_t lastSecondDisplayed = -1;

void updateTimeDisplay() {
  int currentHour, currentMinute, currentSecond;
#if defined (ARDUINO_ARCH_AVR)
  currentHour = hour();
  currentMinute = minute();
  currentSecond = second();
#elif defined(ARDUINO_ARCH_SAMD)
  currentHour = RTCZ.getHours();
  currentMinute = RTCZ.getMinutes();
  currentSecond = RTCZ.getSeconds();
#endif
  if (currentDisplayState != displayStateHome)
    return;
  char displayX;
  int hour12 = currentHour;
  int AMPM = 1;
  if (hour12 > 12) {
    AMPM = 2;
    hour12 -= 12;
  }
  display.fontColor(defaultFontColor, defaultFontBG);
  if (rewriteTime || lastHourDisplayed != hour12) {
    display.setFont(font22pt);
    lastHourDisplayed = hour12;
    displayX = 0;
    display.setCursor(displayX, timeY);
    if (lastHourDisplayed < 10)display.print('0');
    display.print(lastHourDisplayed);
    display.write(':');
    if (lastAMPMDisplayed != AMPM) {
      if (AMPM == 2)
        display.fontColor(inactiveFontColor, inactiveFontBG);
      display.setFont(font10pt);
      display.setCursor(displayX + 80, timeY - 0);
      display.print(F("AM"));
      if (AMPM == 2) {
        display.fontColor(defaultFontColor, defaultFontBG);
      } else {
        display.fontColor(inactiveFontColor, inactiveFontBG);
      }
      display.setCursor(displayX + 80, timeY + 11);
      display.print(F("PM"));
      display.fontColor(defaultFontColor, defaultFontBG);
    }
  }

  if (rewriteTime || lastMinuteDisplayed != currentMinute) {
    display.setFont(font22pt);
    lastMinuteDisplayed = currentMinute;
    displayX = 14 + 14 - 1;
    display.setCursor(displayX, timeY);
    if (lastMinuteDisplayed < 10)display.print('0');
    display.print(lastMinuteDisplayed);
    display.write(':');
  }

  if (rewriteTime || lastSecondDisplayed != currentSecond) {
    display.setFont(font22pt);
    lastSecondDisplayed = currentSecond;
    displayX = 14 + 14 + 14 + 14 - 2;
    display.setCursor(displayX, timeY);
    if (lastSecondDisplayed < 10)display.print('0');
    display.print(lastSecondDisplayed);
  }
  rewriteTime = false;
}

void updateBLEstatusDisplay() {
  if (ble_connection_state == ble_connection_displayed_state)
    return;
  ble_connection_displayed_state = ble_connection_state;
  int x = 62;
  int y = 6;
  int s = 2;
  uint8_t color = 0x03;
  if (ble_connection_state)
    color = 0xE0;
  display.drawLine(x, y + s + s, x, y - s - s, color);
  display.drawLine(x - s, y + s, x + s, y - s, color);
  display.drawLine(x + s, y + s, x - s, y - s, color);
  display.drawLine(x, y + s + s, x + s, y + s, color);
  display.drawLine(x, y - s - s, x + s, y - s, color);
}

void displayBattery() {
  int result = 0;
#if defined (ARDUINO_ARCH_AVR)
  //http://forum.arduino.cc/index.php?topic=133907.0
  const long InternalReferenceVoltage = 1100L;
  ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
  delay(10);
  ADCSRA |= _BV( ADSC );
  while ( ( (ADCSRA & (1 << ADSC)) != 0 ) );
  result = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L;
  //SerialMonitorInterface.println(result);
  //if(result>440){//probably charging
  uint8_t charging = false;
  if (result > 450) {
    charging = true;
  }
  result = constrain(result - 300, 0, 120);
  uint8_t x = 70;
  uint8_t y = 3;
  uint8_t height = 5;
  uint8_t length = 20;
  uint8_t amtActive = (result * length) / 120;
  uint8_t red, green, blue;
  display.drawLine(x - 1, y, x - 1, y + height, 0xFF); //left boarder
  display.drawLine(x - 1, y - 1, x + length, y - 1, 0xFF); //top border
  display.drawLine(x - 1, y + height + 1, x + length, y + height + 1, 0xFF); //bottom border
  display.drawLine(x + length, y - 1, x + length, y + height + 1, 0xFF); //right border
  display.drawLine(x + length + 1, y + 2, x + length + 1, y + height - 2, 0xFF); //right border
  for (uint8_t i = 0; i < length; i++) {
    if (i < amtActive) {
      red = 63 - ((63 / length) * i);
      green = ((63 / length) * i);
      blue = 0;
    } else {
      red = 32;
      green = 32;
      blue = 32;
    }
    display.drawLine(x + i, y, x + i, y + height, red, green, blue);
  }
#elif defined(ARDUINO_ARCH_SAMD)
  //http://atmel.force.com/support/articles/en_US/FAQ/ADC-example
  SYSCTRL->VREF.reg |= SYSCTRL_VREF_BGOUTEN;
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  ADC->SAMPCTRL.bit.SAMPLEN = 0x1;
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  ADC->INPUTCTRL.bit.MUXPOS = 0x19;         // Internal bandgap input
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  ADC->CTRLA.bit.ENABLE = 0x01;             // Enable ADC
  // Start conversion
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  ADC->SWTRIG.bit.START = 1;
  // Clear the Data Ready flag
  ADC->INTFLAG.bit.RESRDY = 1;
  // Start conversion again, since The first conversion after the reference is changed must not be used.
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  ADC->SWTRIG.bit.START = 1;
  // Store the value
  while ( ADC->INTFLAG.bit.RESRDY == 0 );   // Waiting for conversion to complete
  uint32_t valueRead = ADC->RESULT.reg;
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  ADC->CTRLA.bit.ENABLE = 0x00;             // Disable ADC
  while (ADC->STATUS.bit.SYNCBUSY == 1);
  SYSCTRL->VREF.reg &= ~SYSCTRL_VREF_BGOUTEN;
  result = (((1100L * 1024L) / valueRead) + 5L) / 10L;
  uint8_t x = 70;
  uint8_t y = 3;
  uint8_t height = 5;
  uint8_t length = 20;
  uint8_t red, green;
  if (result > 325) {
    red = 0;
    green = 63;
  } else {
    red = 63;
    green = 0;
  }
  display.drawLine(x - 1, y, x - 1, y + height, 0xFF); //left boarder
  display.drawLine(x - 1, y - 1, x + length, y - 1, 0xFF); //top border
  display.drawLine(x - 1, y + height + 1, x + length, y + height + 1, 0xFF); //bottom border
  display.drawLine(x + length, y - 1, x + length, y + height + 1, 0xFF); //right border
  display.drawLine(x + length + 1, y + 2, x + length + 1, y + height - 2, 0xFF); //right border
  for (uint8_t i = 0; i < length; i++) {
    display.drawLine(x + i, y, x + i, y + height, red, green, 0);
  }
#endif
}
