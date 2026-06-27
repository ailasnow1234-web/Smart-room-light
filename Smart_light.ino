const int PIR_PIN = 13;   
const int LDR_PIN = 34;   
const int LED_PIN = 2;   
const int DARK_THRESHOLD = 1500; 

//TIMING CONFIGURATION
// 30000 milliseconds = 30 seconds.
const unsigned long LIGHT_ON_DURATION = 15000; 

unsigned long lastMotionTime = 0; // Stores the timestamp of the last detected motion
bool isLightOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure the light starts in the OFF state
  Serial.println("Smart Light 15-Second Timer System Ready...");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);  
  int pirStatus = digitalRead(PIR_PIN); 

  // 1. Core Logic: If it is NIGHT and MOTION is detected
  if (ldrValue > DARK_THRESHOLD && pirStatus == HIGH) {
    digitalWrite(LED_PIN, HIGH); // Turn the LED ON instantly (on the spot)
    isLightOn = true;
    lastMotionTime = millis();   // Continually reset the timer as long as there is movement
    Serial.println("--> Motion detected! Light ON & timer reset.");
  } 
  
  // 2. Timer Logic: If motion STOPS, begin the 30-second countdown
  if (isLightOn && pirStatus == LOW) {
    // Check if 30 seconds have passed since the last recorded motion
    if (millis() - lastMotionTime >= LIGHT_ON_DURATION) {
      digitalWrite(LED_PIN, LOW); // 30 seconds is up, turn the light OFF automatically
      isLightOn = false;
      Serial.println("--> 15 seconds passed. Light turned OFF.");
    } else {
      // Display the remaining countdown time on the Serial Monitor for testing
      unsigned long remainingTime = (LIGHT_ON_DURATION - (millis() - lastMotionTime)) / 1000;
      Serial.print("--> No motion. Light will turn off in: ");
      Serial.print(remainingTime);
      Serial.println(" seconds.");
    }
  }

  // Override: If it suddenly becomes daytime, turn the light off immediately to save power
  if (ldrValue <= DARK_THRESHOLD) {
    digitalWrite(LED_PIN, LOW);
    isLightOn = false;
  }

  delay(200); // Check the sensors every 0.2 seconds to keep the system responsive
}
