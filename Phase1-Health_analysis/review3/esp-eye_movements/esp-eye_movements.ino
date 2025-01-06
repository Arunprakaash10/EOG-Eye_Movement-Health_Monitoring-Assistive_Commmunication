#include <math.h>

#define SAMPLE_RATE 75
#define BAUD_RATE 115200
#define DATA_LENGTH 10


#define UP_DOWN_BLINK_PIN 34  
#define LEFT_RIGHT_PIN 35      
#define OUTPUT_PIN 13    
#define BUZZER_PIN 25    

// Global variables for blink detection
int data_index = 0;
bool peak = false;

// Filter buffers
static float data_buffer[DATA_LENGTH];
static float mean_buffer[DATA_LENGTH];
static float standard_deviation_buffer[DATA_LENGTH];

// Function prototypes
void detectUpDownEyeMovement(void *parameter);
void detectLeftRightEyeMovement(void *parameter);
void detectBlink(void *parameter);
float EOGFilter(float input);
bool GetPeak(float new_sample);

// Variables to hold the counts of eye movements
int upCount = 0;
int downCount = 0;
int rightCount = 0;
int leftCount = 0;
int blinkCount = 0;

// Timer variables
unsigned long startTime = 0;
unsigned long interval = 10000; // 10 seconds in milliseconds

void setup() {
  // Start Serial communication
  Serial.begin(BAUD_RATE);
  startTime = millis();
  
  // Set up the GPIO pins
  pinMode(UP_DOWN_BLINK_PIN, INPUT);
  pinMode(LEFT_RIGHT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Create FreeRTOS tasks for parallel execution
  xTaskCreate(detectUpDownEyeMovement, "UpDown", 4096, NULL, 1, NULL);
  xTaskCreate(detectLeftRightEyeMovement, "LeftRight", 4096, NULL, 1, NULL);
  xTaskCreate(detectBlink, "Blink", 4096, NULL, 1, NULL);
}

void loop() {
  if (millis() - startTime >= interval) {
    // Get the current time for the timestamp (in milliseconds)
    unsigned long currentTime = millis();

    // Send counts and timestamp over the serial line in CSV format
    Serial.print(currentTime); // Timestamp
    Serial.print(",");
    Serial.print(upCount);
    Serial.print(",");
    Serial.print(downCount);
    Serial.print(",");
    Serial.print(rightCount);
    Serial.print(",");
    Serial.print(leftCount);
    Serial.print(",");
    Serial.println(blinkCount);

    // Reset counts for the next 10-second interval
    upCount = 0;
    downCount = 0;
    rightCount = 0;
    leftCount = 0;
    blinkCount = 0;
    
    // Reset the timer for the next 10 seconds
    startTime = millis();
  }

  // Add a small delay to avoid overwhelming the serial line
  delay(100);
}

void detectUpDownEyeMovement(void *parameter) {
  int range1 = 0;

  while (true) {
    int sensor1 = analogRead(UP_DOWN_BLINK_PIN);
    int constrainedInput1 = constrain(sensor1, 350, 650);
    range1 = map(constrainedInput1, 350, 750, 0, 6);
    switch (range1) {
      case 0:
        //Serial.println("Down");
        downCount++;
        delay(100);
        break;
      case 4:
        //Serial.println("Up");
        upCount++;
        delay(100);
        break;
      default:
        //Serial.println("0");
        break;
    }
    delay(1000);
  }
}

void detectLeftRightEyeMovement(void *parameter) {
  int range1 = 0;

  while (true) {
    int sensor1 = analogRead(LEFT_RIGHT_PIN);
    int constrainedInput1 = constrain(sensor1, 350, 650);
    range1 = map(constrainedInput1, 350, 750, 0, 6);
    switch (range1) {
      case 0:
        //Serial.println("Left");
        leftCount++;
        delay(100);
        break;
      case 4:
        //Serial.println("Right");
        rightCount++;
        delay(100);
        break;
      default:
        //Serial.println("0");
        break;
    }
    delay(1000);
  }
}

void detectBlink(void *parameter) {
  static long timer = 0;
  timer -= 1000;

  while (true) {
    if (timer < 0) {
      timer += 1000000 / SAMPLE_RATE;
      float sensor_value = analogRead(UP_DOWN_BLINK_PIN);
      float signal = EOGFilter(sensor_value) / 512;
      peak = GetPeak(signal);
      Serial.print(signal);
      Serial.print(",");
      Serial.println(peak);
      digitalWrite(OUTPUT_PIN, peak);
      if (peak) blinkCount++;
    }
    delay(10); // Small delay to prevent blocking
  }
}

float EOGFilter(float input) {
  float output = input;
  
  // First section
  static float z1, z2;
  float x = output - 0.02977423 * z1 - 0.04296318 * z2;
  output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
  z2 = z1;
  z1 = x;
  
  // Second section
  static float z1b, z2b;
  float xb = output - 0.08383952 * z1b - 0.46067709 * z2b;
  output = 1.00000000 * xb + 2.00000000 * z1b + 1.00000000 * z2b;
  z2b = z1b;
  z1b = xb;

  // Third section
  static float z1c, z2c;
  float xc = output - -1.92167271 * z1c - 0.92347975 * z2c;
  output = 1.00000000 * xc + -2.00000000 * z1c + 1.00000000 * z2c;
  z2c = z1c;
  z1c = xc;

  // Fourth section
  static float z1d, z2d;
  float xd = output - -1.96758891 * z1d - 0.96933514 * z2d;
  output = 1.00000000 * xd + -2.00000000 * z1d + 1.00000000 * z2d;
  z2d = z1d;
  z1d = xd;

  return output;
}

bool GetPeak(float new_sample) {
  if (new_sample - mean_buffer[data_index] > (DATA_LENGTH * 1.2) * standard_deviation_buffer[data_index]) {
    data_buffer[data_index] = new_sample + data_buffer[data_index];
    peak = true;
  } else {
    data_buffer[data_index] = new_sample;
    peak = false;
  }

  // Calculate mean
  float sum = 0.0, mean, standard_deviation = 0.0;
  for (int i = 0; i < DATA_LENGTH; ++i){
    sum += data_buffer[(data_index + i) % DATA_LENGTH];
  }
  mean = sum / DATA_LENGTH;

  // Calculate standard deviation
  for (int i = 0; i < DATA_LENGTH; ++i){
    standard_deviation += pow(data_buffer[(i) % DATA_LENGTH] - mean, 2);
  }

  mean_buffer[data_index] = mean;
  standard_deviation_buffer[data_index] = sqrt(standard_deviation / DATA_LENGTH);

  data_index = (data_index + 1) % DATA_LENGTH;

  return peak;
}
