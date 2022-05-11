#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include "Statistic.h"

Statistic accXStats;
Statistic accYStats;
Statistic accZStats;
Statistic gyrXStats;
Statistic gyrYStats;
Statistic gyrZStats;

uint32_t start;
uint32_t stop;

enum Target
{
  TARGET_IDLE,
  TARGET_FERRARI,
  TARGET_HAAS,
  TARGET_WILLIAMS
};

#define NUM_SAMPLES 240
#define CAR_TARGET TARGET_WILLIAMS

typedef struct
{
  float acc_x;
  float acc_y;
  float acc_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
} vector_t;

vector_t data[NUM_SAMPLES];

int i = 0;
int j = 0;

// Function prototypes
void save_acc_data(float x, float y, float z);
void save_gyro_data(float x, float y, float z);
void calculate_statistics(vector_t *data, int length);
void print_csv_header();
void print_data_csv();


void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(__FILE__);
  Serial.print("Demo Statistics lib ");
  Serial.println(STATISTIC_LIB_VERSION);
  start = millis();

  // Initialize IMU
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  print_csv_header();
}

void loop(void)
{
  // IMU variables
  float x, y, z;

  // Read IMU
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
  {
    // Read accelerometer
    IMU.readAcceleration(x, y, z);

    // Save accelerometer data
    save_acc_data(x, y, z);

    // Read gyroscope
    IMU.readGyroscope(x, y, z);

    // Save gyroscope data
    save_gyro_data(x, y, z);

    i++;
    if (i == NUM_SAMPLES)
    {
      i = 0;
      // Calculate statistics for each axis of accelerometer and gyroscope
      calculate_statistics(data, NUM_SAMPLES);
    }
  }
}

void save_acc_data(float x, float y, float z)
{
  data[i].acc_x = x;
  data[i].acc_y = y;
  data[i].acc_z = z;
}

void save_gyro_data(float x, float y, float z)
{
  data[i].gyro_x = x;
  data[i].gyro_y = y;
  data[i].gyro_z = z;
}

void calculate_statistics(vector_t *data, int length)
{
  start = millis();

  // Calculate statistics for each axis of accelerometer and gyroscope
  accXStats.clear(); // explicitly start clean
  accYStats.clear();
  accZStats.clear();
  gyrXStats.clear();
  gyrYStats.clear();
  gyrZStats.clear();

  for (int i = 0; i < length; i++)
  {
    accXStats.add(data[i].acc_x);
    accYStats.add(data[i].acc_y);
    accZStats.add(data[i].acc_z);
    gyrXStats.add(data[i].gyro_x);
    gyrYStats.add(data[i].gyro_y);
    gyrZStats.add(data[i].gyro_z);
  }
  stop = millis();

  // Print statistics
  print_data_csv();
}

void print_csv_header()
{
  // print the CSV header (ax0,ay0,az0,...,gx49,gy49,gz49,target)
  Serial.print("aXmin");
  Serial.print(",aXmax");
  Serial.print(",aXavg");
  Serial.print(",aXvar");
  Serial.print(",aXstdev");
  Serial.print(",aXunbiasStdDev");
  Serial.print(",aYmin");
  Serial.print(",aYmax");
  Serial.print(",aYavg");
  Serial.print(",aYvar");
  Serial.print(",aYstdev");
  Serial.print(",aYunbiasStdDev");
  Serial.print(",aZmin");
  Serial.print(",aZmax");
  Serial.print(",aZavg");
  Serial.print(",aZvar");
  Serial.print(",aZstdev");
  Serial.print(",aZunbiasStdDev");
  Serial.print(",gXmin");
  Serial.print(",gXmax");
  Serial.print(",gXavg");
  Serial.print(",gXvar");
  Serial.print(",gXstdev");
  Serial.print(",gXunbiasStdDev");
  Serial.print(",gYmin");
  Serial.print(",gYmax");
  Serial.print(",gYavg");
  Serial.print(",gYvar");
  Serial.print(",gYstdev");
  Serial.print(",gYunbiasStdDev");
  Serial.print(",gZmin");
  Serial.print(",gZmax");
  Serial.print(",gZavg");
  Serial.print(",gZvar");
  Serial.print(",gZstdev");
  Serial.print(",gZunbiasStdDev");
  Serial.print(",");
  Serial.println("target");
}

void print_data_csv()
{
  // print the CSV data (ax0,ay0,az0,...,gx49,gy49,gz49,target)
  Serial.print(accXStats.minimum());
  Serial.print(",");
  Serial.print(accXStats.maximum());
  Serial.print(",");
  Serial.print(accXStats.average());
  Serial.print(",");
  Serial.print(accXStats.variance());
  Serial.print(",");
  Serial.print(accXStats.pop_stdev());
  Serial.print(",");
  Serial.print(accXStats.unbiased_stdev());
  Serial.print(",");
  Serial.print(accYStats.minimum());
  Serial.print(",");
  Serial.print(accYStats.maximum());
  Serial.print(",");
  Serial.print(accYStats.average());
  Serial.print(",");
  Serial.print(accYStats.variance());
  Serial.print(",");
  Serial.print(accYStats.pop_stdev());
  Serial.print(",");
  Serial.print(accYStats.unbiased_stdev());
  Serial.print(",");
  Serial.print(accZStats.minimum());
  Serial.print(",");
  Serial.print(accZStats.maximum());
  Serial.print(",");
  Serial.print(accZStats.average());
  Serial.print(",");
  Serial.print(accZStats.variance());
  Serial.print(",");
  Serial.print(accZStats.pop_stdev());
  Serial.print(",");
  Serial.print(accZStats.unbiased_stdev());
  Serial.print(",");
  Serial.print(gyrXStats.minimum());
  Serial.print(",");
  Serial.print(gyrXStats.maximum());
  Serial.print(",");
  Serial.print(gyrXStats.average());
  Serial.print(",");
  Serial.print(gyrXStats.variance());
  Serial.print(",");
  Serial.print(gyrXStats.pop_stdev());
  Serial.print(",");
  Serial.print(gyrXStats.unbiased_stdev());
  Serial.print(",");
  Serial.print(gyrYStats.minimum());
  Serial.print(",");
  Serial.print(gyrYStats.maximum());
  Serial.print(",");
  Serial.print(gyrYStats.average());
  Serial.print(",");
  Serial.print(gyrYStats.variance());
  Serial.print(",");
  Serial.print(gyrYStats.pop_stdev());
  Serial.print(",");
  Serial.print(gyrYStats.unbiased_stdev());
  Serial.print(",");
  Serial.print(gyrZStats.minimum());
  Serial.print(",");
  Serial.print(gyrZStats.maximum());
  Serial.print(",");
  Serial.print(gyrZStats.average());
  Serial.print(",");
  Serial.print(gyrZStats.variance());
  Serial.print(",");
  Serial.print(gyrZStats.pop_stdev());
  Serial.print(",");
  Serial.print(gyrZStats.unbiased_stdev());
  Serial.print(",");
  Serial.println(CAR_TARGET);
}
