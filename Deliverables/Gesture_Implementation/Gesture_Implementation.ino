// Both packages from: https://github.com/jrowberg/i2cdevlib
// This code is also slightly based on the MPU6050 example in said repo
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;


#define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO
//#define OUTPUT_PLOTTABLE_ACCELGYRO


bool blinkState = false;

void setup() {
  Serial.begin(9600);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // use the code below to change accel/gyro offset values
  /*
  Serial.println("Updating internal sensor offsets...");
  // -76	-2359	1688	0	0	0
  Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
  Serial.print("\n");
  accelgyro.setXGyroOffset(220);
  accelgyro.setYGyroOffset(76);
  accelgyro.setZGyroOffset(-85);
  Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
  Serial.print("\n");
  */

  pinMode(LED_BUILTIN, OUTPUT);
}

// For water gesture  
int minZ = -12000;
int maxZ = -10000;
// For reading sensors
int minY = 2000;
int maxY = 0;

bool triggered;

void loop() {
  // read raw accel/gyro measurements from device
  //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  #ifdef OUTPUT_READABLE_ACCELGYRO
      // display tab-separated accel/gyro x/y/z values
      Serial.print("a/g:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.print("\t");
      Serial.print(gx); Serial.print("\t");
      Serial.print(gy); Serial.print("\t");
      Serial.println(gz);
  #endif

  #ifdef OUTPUT_PLOTTABLE_ACCELGYRO
      Serial.print("ax:"); Serial.println(ax);
      Serial.print("ay:"); Serial.println(ay);
      Serial.print("az:"); Serial.println(az);
      Serial.print("gx:"); Serial.println(gx);
      Serial.print("gy:"); Serial.println(gy);
      Serial.print("gz:"); Serial.println(gz);
  #endif

  #ifdef OUTPUT_BINARY_ACCELGYRO
      Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
      Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
      Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
      Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
      Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
      Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
  #endif

  // blink LED to indicate activity
  //blinkState = !blinkState;
  digitalWrite(LED_BUILTIN, blinkState);

  if (az > minZ && ay < minY) { // Wait for all gestures to end
    triggered = false;
  }

  if (triggered) { // Prevent gesture from triggering action twice
    delay(200);
    return;
  }

  // At this point, no gesture is being made yet

  // Check for water gesture
  if (az < minZ) {
    delay(1000);
    if (az < maxZ) {
      Serial.println("doWaterGesture");
      triggered = true;
    }
  }

  // Check for read sensor gesture
  if (ay > minY) {
    delay(1000);
    if (ay > maxY) {
      Serial.println("doSenseGesture");
      triggered = true;
    }
  }
}

