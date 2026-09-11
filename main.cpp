#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Инициализация дисплея I2C (адрес 0x27, 16 символов, 2 строки)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int BTN_PLUS_PIN = 2;   // Кнопка Увеличить (+)
const int BTN_MINUS_PIN = 3;  // Кнопка Уменьшить (-)
const int PWM_OUT_PIN = 9;    // Выход ШИМ на управление силовой платой (Пин 9)

// Диапазон сварочного тока
int currentAmp = 80;          // Начальный ток 80 А
const int MIN_AMP = 1;       // Минимальный ток 1 А
const int MAX_AMP = 600;      // Максимальный ток 600 А

unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 120; // Задержка для плавного и быстрого нажатия

void updatePowerOutput() {
  // Пересчитываем Амперы (10-200) в ШИМ сигнал (0-255) для силовой платы
  int pwmValue = map(currentAmp, MIN_AMP, MAX_AMP, 0, 255);
  analogWrite(PWM_OUT_PIN, pwmValue);
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELDING CURRENT");
  lcd.setCursor(0, 1);
  lcd.print("SET: ");
  lcd.print(currentAmp);
  lcd.print(" A");
}

void setup() {
  pinMode(BTN_PLUS_PIN, INPUT_PULLUP);
  pinMode(BTN_MINUS_PIN, INPUT_PULLUP);
  pinMode(PWM_OUT_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  updatePowerOutput();
  updateDisplay();
}

void loop() {
  if (millis() - lastButtonTime > debounceDelay) {
    // Увеличение тока на +1 А
    if (digitalRead(BTN_PLUS_PIN) == LOW) {
      if (currentAmp +1 <= MAX_AMP) {
        currentAmp += 1;
        updatePowerOutput();
        updateDisplay();
      }
      lastButtonTime = millis();
    }

    // Уменьшение тока на -1 А
    if (digitalRead(BTN_MINUS_PIN) == LOW) {
      if (currentAmp - 1 >= MIN_AMP) {
        currentAmp -= 1;
        updatePowerOutput();
        updateDisplay();
      }
      lastButtonTime = millis();
    }
  }
}
