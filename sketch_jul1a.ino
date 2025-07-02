#include <SH1106.h>  // Laura Kirsch's SH1106 library
#include <Wire.h>

// === OLED Setup ===
SH1106 display;  // Default constructor, suitable for I2C 128x64 SH1106

// === Pin Configuration ===
#define PINKY_PIN   A0 
#define INDEX_PIN   A3
#define MIDDLE_PIN  A2
#define RING_PIN    A1
#define THUMB_PIN   2  // Digital pin, digital pin cause arduino only has 6 analog pins and 2 analog i needed for oled display so remaining 4 were for 4 fingers and so thumb digital mai daal diya

// === Gesture Definition Structure ===
struct PhraseGesture {
  const char* word;
  int pMin, pMax;
  int iMin, iMax;
  int mMin, mMax;
  int rMin, rMax;
  int tMin, tMax;
};

// === Calibrated Phrase Gestures ===
PhraseGesture phraseGestures[] = {

  { "I want food",                     104, 110, 410, 422, 375, 420, 378, 390, 0, 0 },
  { "I want water",                    104, 110, 425, 440, 360, 374, 378, 390, 0, 0 },
  { "I want to use the Washroom",      104, 110, 425, 440, 375, 420, 360, 375, 0, 0 },
  { "I'm Sleepy",                      111, 115, 425, 440, 375, 420, 378, 390, 0, 0 },
};

const int numPhrases = sizeof(phraseGestures) / sizeof(phraseGestures[0]);

void setup() {
  Serial.begin(9600);
  pinMode(THUMB_PIN, INPUT);

  // Initialize OLED display
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // I2C address 0x3C
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("System Ready");
  display.display();
  delay(1000);
}
void loop() {
  // Read flex sensor values
  int p = analogRead(PINKY_PIN);
  int i = analogRead(INDEX_PIN);
  int m = analogRead(MIDDLE_PIN);
  int r = analogRead(RING_PIN);
  int t = digitalRead(THUMB_PIN);

  // Detect gesture
  String phrase = detectPhrase(p, i, m, r, t);
  if (phrase == "") phrase = "I'm Chatterpalm";

  // Output to Serial
  Serial.print("P: "); Serial.print(p);
  Serial.print(" | I: "); Serial.print(i);
  Serial.print(" | M: "); Serial.print(m);
  Serial.print(" | R: "); Serial.print(r);
  Serial.print(" | T: "); Serial.print(t);
  Serial.print(" | Phrase: "); Serial.println(phrase);

  // Output to OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print("Phrase:");
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.print(phrase);
  display.display();

  delay(1000);
}

// === Phrase Matching Logic ===
String detectPhrase(int p, int i, int m, int r, int t) {
  for (int w = 0; w < numPhrases; w++) {
    PhraseGesture g = phraseGestures[w];
    if (p >= g.pMin && p <= g.pMax &&
        i >= g.iMin && i <= g.iMax &&
        m >= g.mMin && m <= g.mMax &&
        r >= g.rMin && r <= g.rMax &&
        t >= g.tMin && t <= g.tMax) {
      return g.word;
    }
  }
  return "";
}
