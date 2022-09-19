#include <LiquidCrystal.h>
#include <AD9833.h>

#define FSyncPin 12
#define buttonRightPin 8
#define buttonEnterPin 9
#define buttonLeftPin 10
#define voltagePin A0
#define wifiPin A1
#define lowBatteryModePin A2

AD9833 signalGenerator(FSyncPin);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int menuVar;
String result;
bool menuFlag;
bool wifiFlag;
double lastVoltage;
byte batLevel[8];

void setup() {
  lcd.begin(20, 4);
  lcd.print("  DELTA ELEKTRONiK");
  lcd.setCursor(0, 2);
  lcd.print(" ALAN TARAMA CiHAZI");
  delay(4000);
  lcd.clear();
  pinMode(buttonRightPin, INPUT);
  pinMode(buttonEnterPin, INPUT);
  pinMode(buttonLeftPin, INPUT);
  pinMode(voltagePin, INPUT);
  pinMode(wifiPin, INPUT);
  pinMode(lowBatteryModePin, OUTPUT);
  menuVar = 1;
  menuFlag = true;
  result = getMenuString();
  yaz("ALAN TARAMA CiHAZI", 0);
  yaz("Seciniz " + result, 2);
  lastVoltage = -1;
  wifiFlag = false;
  batLevel[0] = B01110;
  signalGenerator.Begin();
}

void loop() {
  if (!wifiFlag && digitalRead(wifiPin)) {
    lcd.setCursor(15, 3);
    lcd.print("((O))");
    wifiFlag = true;
  } else if (wifiFlag && !digitalRead(wifiPin)) {
    lcd.setCursor(15, 3);
    lcd.print("     ");
    wifiFlag = false;
  }

  double voltage = 4.1;
  if (abs(voltage - lastVoltage) > 1) {
    batterylevel(voltage);
    lastVoltage = voltage;
    if (voltage > 3 && !digitalRead(lowBatteryModePin)) {
      digitalWrite(lowBatteryModePin, HIGH);
    } else if (voltage < 3 && digitalRead(lowBatteryModePin)) {
      digitalWrite(lowBatteryModePin, LOW);
    }
  }

  if (digitalRead(buttonRightPin) && menuFlag) {
    menuVar += 1;
    if (menuVar > 8) {
      menuVar = 1;
    }
    result = getMenuString();
    yaz("Seciniz " + result, 2);
    delay(250);
  }

  if (digitalRead(buttonLeftPin) && menuFlag) {
    menuVar -= 1;
    if (menuVar < 1) {
      menuVar = 8;
    }
    result = getMenuString();
    yaz("Seciniz " + result, 2);
    delay(250);
  }

  if (digitalRead(buttonEnterPin)) {
    menuFlag = !menuFlag;
    if (!menuFlag) {
      yaz("", 1);
      yaz(result, 2);
      signalGenerator.ApplySignal(SINE_WAVE, REG0, getFrequency());
      signalGenerator.EnableOutput(true);
    } else {
      yaz("", 2);
      yaz("Seciniz " + result, 2);
      signalGenerator.EnableOutput(false);
    }
    delay(250);
  }
}

String getMenuString() {
  if (menuVar == 1) {
    return "ALTIN";
  } else if (menuVar == 2) {
    return "GUMUS";
  } else if (menuVar == 3) {
    return "BRONZ";
  } else if (menuVar == 4) {
    return "BAKIR";
  } else if (menuVar == 5) {
    return "DEMiR";
  } else if (menuVar == 6) {
    return "BOSLUK";
  } else if (menuVar == 7) {
    return "KEMiK";
  } else if (menuVar == 8) {
    return "  SU  ";
  }
}

int getFrequency() {
  if (menuVar == 1) {
    return 1;
  } else if (menuVar == 2) {
    return 2;
  } else if (menuVar == 3) {
    return 3;
  } else if (menuVar == 4) {
    return 4;
  } else if (menuVar == 5) {
    return 5;
  } else if (menuVar == 6) {
    return 6;
  } else if (menuVar == 7) {
    return 7;
  } else if (menuVar == 8) {
    return 8;
  }
}

void yaz(String yazi, byte satir) {
  for (int i = 0; i < 20; i++) {
    lcd.setCursor(i, (satir % 4));
    lcd.print(" ");
  }
  lcd.setCursor(((20 - yazi.length()) / 2), (satir % 4));
  lcd.print(yazi);
}

void solaYaz(String yazi, byte satir) {
  for (int i = 0; i < 20; i++) {
    lcd.setCursor(i, (satir % 4));
    lcd.print(" ");
  }
  lcd.setCursor(0, (satir % 4));
  lcd.print(yazi);
}

double getVoltage() {
  int x = random(0, 3);
  if (x == 0) {
    return 11.59;
  } else if (x == 1) {
    return 12.76;
  } else if (x == 2) {
    return 11.88;
  } else {
    return 12.37;
  }
}

void batterylevel(double curvolt) {
  if (curvolt <= 4.2 && curvolt > 4.0) {
    batLevel[1] = B11111;
    batLevel[2] = B11111;
    batLevel[3] = B11111;
    batLevel[4] = B11111;
    batLevel[5] = B11111;
    batLevel[6] = B11111;
    batLevel[7] = B11111;
  }
  if (curvolt <= 4.0 && curvolt > 3.8) {
    batLevel[1] = B10001;
    batLevel[2] = B11111;
    batLevel[3] = B11111;
    batLevel[4] = B11111;
    batLevel[5] = B11111;
    batLevel[6] = B11111;
    batLevel[7] = B11111;
  }
  if (curvolt <= 3.8 && curvolt > 3.6) {
    batLevel[1] = B10001;
    batLevel[2] = B10001;
    batLevel[3] = B11111;
    batLevel[4] = B11111;
    batLevel[5] = B11111;
    batLevel[6] = B11111;
    batLevel[7] = B11111;
  }
  if (curvolt <= 3.6 && curvolt > 3.4) {
    batLevel[1] = B10001;
    batLevel[2] = B10001;
    batLevel[3] = B10001;
    batLevel[4] = B11111;
    batLevel[5] = B11111;
    batLevel[6] = B11111;
    batLevel[7] = B11111;
  }
  if (curvolt <= 3.4 && curvolt > 3.2) {
    batLevel[1] = B10001;
    batLevel[2] = B10001;
    batLevel[3] = B10001;
    batLevel[4] = B10001;
    batLevel[5] = B11111;
    batLevel[6] = B11111;
    batLevel[7] = B11111;
  }
  if (curvolt <= 3.2 && curvolt > 3.0) {
    batLevel[1] = B10001;
    batLevel[2] = B10001;
    batLevel[3] = B10001;
    batLevel[4] = B10001;
    batLevel[5] = B10001;
    batLevel[6] = B11111;
    batLevel[7] = B11111;
  }
  if (curvolt < 3.0) {
    batLevel[1] = B10001;
    batLevel[2] = B10001;
    batLevel[3] = B10001;
    batLevel[4] = B10001;
    batLevel[5] = B10001;
    batLevel[6] = B10001;
    batLevel[7] = B11111;
  }
  lcd.createChar(0, batLevel);
  lcd.setCursor(0, 3);
  lcd.write(byte(0));
}