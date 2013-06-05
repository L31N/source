// MPU6000 SPI library using DMP internal sensor fusion solution
// version 1.05
// Authors : Jose Julio and ArduCopter/ArduPilot development team
// DMP code based on sample code from invensense and the work of Noah Zerkin (thanks for share)

// APM 2.0 Axis definition: 
// X axis pointing forward, Y axis pointing to the right and Z axis pointing down.
// Positive roll : right wing down
// Positive pitch : nose up
// Positive yaw : clockwise

// Internal MPU6000 axis definition
// X axis pointing to right, Y axis pointing forward and Z axis pointing up

#include "MPU6000.h"

volatile uint8_t _MPU6000_newdata;

// new stuff
void digitalWrite(int pin, int high) {}  // this asserts/releases the chip select
void _SPI_write(uint8_t reg, uint8_t data);
void delay(int v) {}

unsigned char pgm_read_byte(void* foo) {};unsigned char gm_read_byte(void* foo) {};
void attachInterrupt(int a, int b, int c) {};

int SPI_transfer(int mem_rw) { return 0; }
void _set_mpu_memory(uint8_t bank, uint8_t address, uint8_t num, uint8_t regs[]);

#define M_PI 3.1415
#define RISING 1
// end new stuff


// MPU6000 Initialization and configuration
void MPU6000_init(void)
{
  // SPI initialization
//  SPI.begin();
//  SPI.setClockDivider(SPI_CLOCK_DIV16);      // SPI at 1Mhz (on 16Mhz clock)
  delay(10);

  // MPU6000 chip select setup
//  pinMode(MPU6000_CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
  
  // Variable initialization
  _gyro_bias_from_gravity_gain = 0.00005;
  _yaw_compass_diff = COMPASS_NULL;
  _compass_correction_gain = 0.01;
  _gyro_bias_from_compass_gain = 0.00005;   // not used now

  // Chip reset
  _SPI_write(MPUREG_PWR_MGMT_1, BIT_H_RESET);
  delay(100);
  // Wake up device and select GyroZ clock (better performance)
  _SPI_write(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
  // Disable I2C bus (recommended on datasheet)
  _SPI_write(MPUREG_USER_CTRL, BIT_I2C_IF_DIS);
  // SAMPLE RATE
  _SPI_write(MPUREG_SMPLRT_DIV,0x04);     // Sample rate = 200Hz    Fsample= 1Khz/(4+1) = 200Hz     
  // FS & DLPF   FS=2000º/s, DLPF = 42Hz (low pass filter)
  _SPI_write(MPUREG_CONFIG, BITS_DLPF_CFG_42HZ);  // BITS_DLPF_CFG_20HZ BITS_DLPF_CFG_42HZ BITS_DLPF_CFG_98HZ
  _SPI_write(MPUREG_GYRO_CONFIG,BITS_FS_2000DPS);  // Gyro scale 2000º/s
  _SPI_write(MPUREG_ACCEL_CONFIG,BITS_FS_2G);           // Accel scele 2g (g=8192)  
  // INT CFG => Interrupt on Data Ready
  _SPI_write(MPUREG_INT_ENABLE,BIT_RAW_RDY_EN);         // INT: Raw data ready
  _SPI_write(MPUREG_INT_PIN_CFG,BIT_INT_ANYRD_2CLEAR);  // INT: Clear on any read
  // Oscillator set
  // _SPI_write(MPUREG_PWR_MGMT_1,MPU_CLK_SEL_PLLGYROZ);

  // MPU_INT is connected to INT 6. Enable interrupt on INT6
  attachInterrupt(6,MPU6000_newdata_int,RISING);
  
  _dmp_load_mem();   // load initial values to DMP memory
}

// Function to ask for new data (for polling on main loop)
uint8_t newdata()
{
        return(_MPU6000_newdata);
}

// Read raw data (gyros and accel) sensors on MPU6000
void read()
{
  uint8_t byte_H;
  uint8_t byte_L;
  uint8_t dump;

  // We start a SPI multibyte read of sensors
  uint8_t addr = MPUREG_ACCEL_XOUT_H | 0x80;      // Set most significant bit
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(addr);
  // Read AccelX
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  accelX = ((int)byte_H<<8)| byte_L;
  // Read AccelY
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  accelY = ((int)byte_H<<8)| byte_L;
  // Read AccelZ
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  accelZ = ((int)byte_H<<8)| byte_L;
  // Read Temp
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  mpu_temp = ((int)byte_H<<8)| byte_L;
  // Read GyroX
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  gyroX = ((int)byte_H<<8)| byte_L;
  // Read GyroY
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  gyroY = ((int)byte_H<<8)| byte_L;
  // Read GyroZ
  byte_H = SPI_transfer(0);
  byte_L = SPI_transfer(0);
  gyroZ = ((int)byte_H<<8)| byte_L;
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
}


// MPU6000 DMP initialization 
void dmp_init()
{
  uint8_t dump;
  uint8_t temp;

  _set_dmp_gyro_calibration();
  _set_dmp_accel_calibration();
  _apply_endian_accel();
  _set_mpu_sensors();
  _set_bias_none();
  _SPI_write(MPUREG_INT_ENABLE, 0x32);          //MPUREG_INT_ENABLE,  BIT_ZMOT_EN+BIT_FIFO_OVERFLOW_EN+BIT_DMP_INT_EN
  _set_fifo_interrupt();
  _send_quaternion();                        // By default we only send the quaternion to the FIFO (18 bytes packet size)
  //_send_gyro();
  //_send_accel();                           // !! FIFO_PACKET_SIZE 42
  _set_fifo_rate(MPU6000_200HZ);                                                // 200Hz DMP output rate

  // enable the DMP_INT
  _SPI_write(MPUREG_INT_ENABLE, 0x02);          //MPUREG_INT_ENABLE, BIT_DMP_INT_EN
  _SPI_write(MPUREG_PWR_MGMT_1, 0x03);          //MPUREG_PWR_MGMT_1, CLKSET=GYROZ

  _SPI_write(MPUREG_DMP_CFG_1, 0x03);           //MPUREG_DMP_CFG_1, 0x03
  _SPI_write(MPUREG_DMP_CFG_2, 0x00);           //MPUREG_DMP_CFG_2, 0x00

    //inv_state_change_fifo
  _SPI_write(MPUREG_BANK_SEL,0x01);          // D_1_178
  _SPI_write(MPUREG_MEM_START_ADDR, 0xB2);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(MPUREG_MEM_R_W);
  dump = SPI_transfer(0xFF);
  dump = SPI_transfer(0xFF);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);  

  // ?? FIFO ??
  _SPI_write(MPUREG_BANK_SEL,0x01);           // D_1_144
  _SPI_write(MPUREG_MEM_START_ADDR, 0x90);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(MPUREG_MEM_R_W);
  dump = SPI_transfer(0x09);
  dump = SPI_transfer(0x23);
  dump = SPI_transfer(0xA1);
  dump = SPI_transfer(0x35);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);  

  _SPI_write(MPUREG_USER_CTRL, 0x04);           //MPUREG_USER_CTRL, BIT_FIFO_RST

  FIFO_ready();

  _SPI_write(MPUREG_USER_CTRL, 0x00);           //MPUREG_USER_CTRL, 0
  _SPI_write(MPUREG_PWR_MGMT_1, 0x03);          //MPUREG_PWR_MGMT_1, CLKSET=GYROZ
  _SPI_write(MPUREG_PWR_MGMT_2, 0x00);          //MPUREG_PWR_MGMT_2, 0

  //_SPI_write(MPUREG_ACCEL_CONFIG, 0x00);              //MPUREG_ACCEL_CONFIG, 0

  // Configure motion parameters (probably not needed for us)
  _SPI_write(0x1F, 0x02);                                       //MPUREG_ACCEL_MOT_THR
  _SPI_write(0x21, 0x9C);                                       //MPUREG_ACCEL_ZRMOT_THR
  _SPI_write(0x20, 0x50);                                       //MPUREG_ACCEL_MOT_DUR,
  _SPI_write(0x22, 0x00);                                       //MPUREG_ACCEL_ZRMOT_THR,

  _SPI_write(MPUREG_USER_CTRL, 0x04);                           //MPUREG_USER_CTRL, BIT_FIFO_RST
  _SPI_write(MPUREG_USER_CTRL, 0x00);                           //MPUREG_USER_CTRL, 0
  _SPI_write(MPUREG_USER_CTRL, 0xC8);                           //MPUREG_USER_CTRL, BIT_DMP_EN+BIT_FIFO_EN+BIT_DMP_RST

  // Set the gain of the accel in the sensor fusion
  _set_sensor_fusion_accel_gain(0x80);   // default value

  //_FIFO_reset();  
}

// Function to change the dmp output rate (see constants in MPU6000.h)
void set_dmp_rate(uint8_t rate)
{
    _set_fifo_rate(rate);
}

// Get the quaternion and calculate DCM Rotation Matrix and Euler angles (in radians)
// DCM matrix and euler angles are aligned to APM 2.0 axis definition (X front, Y right, Z down)
void calculate()
{
  long q_long[4];
  uint8_t i,j;
  float sqw;
  float sqx;
  float sqy;
  float sqz;
  float invs;
  float tmp1;
  float tmp2;

  // we are using 16 bits resolution
  q_long[0] = (long) ((((unsigned long) _received_packet[0]) << 8) + ((unsigned long) _received_packet[1]));
  q_long[1] = (long) ((((unsigned long) _received_packet[4]) << 8) + ((unsigned long) _received_packet[5]));
  q_long[2] = (long) ((((unsigned long) _received_packet[8]) << 8) + ((unsigned long) _received_packet[9]));
  q_long[3] = (long) ((((unsigned long) _received_packet[12]) << 8) + ((unsigned long) _received_packet[13]));
  // Take care of sign
  for (i = 0; i < 4; i++ ){
    if(q_long[i] > 32767){
      q_long[i] -= 65536;
    }
    MPU_q[i] = ((float)q_long[i]) / 16384.0f;   // convert from fixed point to float
  }

  // Now we calculate the Rotation matrix in chip axis definition
  /*
  _DCM[0][0] = 2.0*(MPU_q[1]*MPU_q[1] + MPU_q[0]*MPU_q[0]) - 1.0;
   _DCM[0][1] = 2.0*(MPU_q[1]*MPU_q[2] - MPU_q[3]*MPU_q[0]);
   _DCM[0][2] = 2.0*(MPU_q[1]*MPU_q[3] + MPU_q[2]*MPU_q[0]);
   _DCM[1][0] = 2.0*(MPU_q[1]*MPU_q[2] + MPU_q[3]*MPU_q[0]);
   _DCM[1][1] = 2.0*(MPU_q[2]*MPU_q[2] + MPU_q[0]*MPU_q[0]) - 1.0;  
   _DCM[1][2] = 2.0*(MPU_q[2]*MPU_q[3] - MPU_q[1]*MPU_q[0]);
   _DCM[2][0] = 2.0*(MPU_q[1]*MPU_q[3] - MPU_q[2]*MPU_q[0]);
   _DCM[2][1] = 2.0*(MPU_q[2]*MPU_q[3] + MPU_q[1]*MPU_q[0]);
   _DCM[2][2] = 2.0*(MPU_q[3]*MPU_q[3] + MPU_q[0]*MPU_q[0]) - 1.0;
   */

  // Optimized code to calculate the DCM rotation matrix in chip axis definition
  sqw = MPU_q[0]*MPU_q[0];
  sqx = MPU_q[1]*MPU_q[1];
  sqy = MPU_q[2]*MPU_q[2];
  sqz = MPU_q[3]*MPU_q[3];

  // MPU quaternion solution is already normalized
  _DCM[0][0] =  sqx - sqy - sqz + sqw;
  _DCM[1][1] = -sqx + sqy - sqz + sqw;
  _DCM[2][2] = -sqx - sqy + sqz + sqw;

  tmp1 = MPU_q[1]*MPU_q[2];
  tmp2 = MPU_q[3]*MPU_q[0];
  _DCM[1][0] = 2.0 * (tmp1 + tmp2);
  _DCM[0][1] = 2.0 * (tmp1 - tmp2);

  tmp1 = MPU_q[1]*MPU_q[3];
  tmp2 = MPU_q[2]*MPU_q[0];
  _DCM[2][0] = 2.0 * (tmp1 - tmp2);
  _DCM[0][2] = 2.0 * (tmp1 + tmp2);
  tmp1 = MPU_q[2]*MPU_q[3];
  tmp2 = MPU_q[1]*MPU_q[0];
  _DCM[2][1] = 2.0 * (tmp1 + tmp2);
  _DCM[1][2] = 2.0 * (tmp1 - tmp2);  


  // Calculate the DCM rotation matrix aligned with our APM2.0 axis definition
  // chip axis definition : X pointing right, Y pointing forward, Z pointing up
  // APM 2.0 axis definition : X pointint forward, Y pointing right, Z pointing down
  // First Rotate 90º over Z axis 
  // Second Rotate 180º over X axis
  // R = [0 1 0, 1 0 0, 0 0 -1]      
  // DCM = R*_DCM*Rt
  // This code is a direct assignation, much more efficient than the matrix multiplications (but less flexible...)
  DCM[0][0] = _DCM[1][1];
  DCM[0][1] = _DCM[1][0];
  DCM[0][2] = -1.0*_DCM[1][2];
  DCM[1][0] = _DCM[0][1];
  DCM[1][1] = _DCM[0][0];
  DCM[1][2] = -1.0*_DCM[0][2];
  DCM[2][0] = -1.0*_DCM[2][1];
  DCM[2][1] = -1.0*_DCM[2][0];
  DCM[2][2] = _DCM[2][2];

  // Euler angles calculation from rotation matrix
  MPU_roll = atan2(DCM[2][1],DCM[2][2]);
  MPU_pitch = asin(-DCM[2][0]);
  MPU_yaw = atan2(DCM[1][0],DCM[0][0]);

}


// This functions performs an initial gyro offset calibration
// Board should be still for some seconds 
void gyro_offset_calibration()
{
  float aux_float[3]={
    0,0,0      };
  int bias_calibrated[3];
  int i,j;
  int aux_int;

  for(i=0;i<300;i++){
    read();                // Raw data read
    aux_float[0]=aux_float[0]*0.8+gyroX*0.2;
    aux_float[1]=aux_float[1]*0.8+gyroY*0.2;
    aux_float[2]=aux_float[2]*0.8+gyroZ*0.2;
    delay(10);
  }

  for (i=0;i<3;i++){
    bias_calibrated[i] = aux_float[i]*2.0;       // To avoid round errors from float to int...
    bias_calibrated[i] = bias_calibrated[i]>>1;
  }
  // Assign offsets to MPU gyro offset registers
  _gyro_offset_update(bias_calibrated[0],bias_calibrated[1],bias_calibrated[2]);

}

// This functions adds an offset to acceleromter readings
// This is usefull for dynamic acceleration correction (for example centripetal force correction)
// and for the initial offset calibration 
// Input, accel offsets for X,Y and Z in LSB units (as read from raw values)
void accel_set_offset(int accel_x_offset, int accel_y_offset, int accel_z_offset)
{
  long aux_long;
  int aux_int;
  uint8_t regs[2];  

  // Store in memory array
  MPU6000_ACCEL_OFFSET[0] = accel_x_offset;
  MPU6000_ACCEL_OFFSET[1] = accel_y_offset;
  MPU6000_ACCEL_OFFSET[2] = accel_z_offset;

  // Write accel offsets to DMP memory...
  aux_int = accel_x_offset>>1;                            // Transform to internal units  
  regs[0]=(aux_int>>8)&0xFF;
  regs[1]=aux_int&0xFF;
  _set_mpu_memory(0x01,0x08,2,regs);   // key KEY_D_1_8  Accel X offset
  aux_int = accel_y_offset>>1;
  regs[0]=(aux_int>>8)&0xFF;
  regs[1]=aux_int&0xFF;
  _set_mpu_memory(0x01,0x0A,2,regs);   // key KEY_D_1_10  Accel Y offset
  aux_int = accel_z_offset>>1;
  regs[0]=(aux_int>>8)&0xFF;
  regs[1]=aux_int&0xFF;
  _set_mpu_memory(0x01,0x02,2,regs);   // key KEY_D_1_2  Accel Z offset
}

// Return the actual accel offset (array int offset[3])
void accel_get_offset(int *accel_offset)
{
  accel_offset[0] = MPU6000_ACCEL_OFFSET[0];
  accel_offset[1] = MPU6000_ACCEL_OFFSET[1];
  accel_offset[2] = MPU6000_ACCEL_OFFSET[2];
}

// This functions performs an initial accel offset calibration
// Board should be flat and still for some seconds 
void accel_offset_calibration()
{
  float aux_float[3]={
    0,0,ACCEL_SCALE_G      }; // Initial expected values...
  int i,j;
  int aux_int;
  uint8_t aux_byte;

  // Take some readings and average values...
  for(i=0;i<300;i++){
    read();
    aux_float[0]=aux_float[0]*0.8+accelX*0.2;
    aux_float[1]=aux_float[1]*0.8+accelY*0.2;
    aux_float[2]=aux_float[2]*0.8+accelZ*0.2;
    delay(10);
  }
  MPU6000_ACCEL_OFFSET[0] = aux_float[0];
  MPU6000_ACCEL_OFFSET[1] = aux_float[1];
  MPU6000_ACCEL_OFFSET[2] = aux_float[2]-ACCEL_SCALE_G;
  
  accel_set_offset(MPU6000_ACCEL_OFFSET[0],MPU6000_ACCEL_OFFSET[1],MPU6000_ACCEL_OFFSET[2]);
} 

// Calculate tilt compensated compass heading
// mag componentes and roll/pitch should be in APM2.0 axis reference (X front, Y right, Z down)
// In this function we used some optimizations based on DCM matrix
float compass_angle(float mag_x, float mag_y, float mag_z, float declination)
{
  float headX;
  float headY;
  float heading;
  float cos_pitch = sqrt(1-(DCM[2][0]*DCM[2][0]));

  // Tilt compensated magnetic field X component:
  // headX = mag_x*cos_pitch + mag_y*sin_roll*sin_pitch + mag_z*cos_roll*sin_pitch;
  headX = mag_x*cos_pitch - mag_y*DCM[2][1]*DCM[2][0]/cos_pitch - mag_z*DCM[2][2]*DCM[2][0]/cos_pitch;
  // Tilt compensated magnetic field Y component:
  //headY = mag_y*cos_roll - mag_z*sin_roll;
  headY = mag_y*DCM[2][2]/cos_pitch - mag_z*DCM[2][1]/cos_pitch;
  // magnetic heading
  heading = atan2(-headY,headX);
  return(heading);

  // Declination correction (if supplied)
  if( fabs(declination) > 0.0 ){
      heading = _normalize_angle(heading + declination);      
  }
}

// Update the yaw correction using compass info (compass sensor fusion)
void update_yaw_compass_correction(float compass_heading)
{
  float corrected_yaw;

  if (_yaw_compass_diff == COMPASS_NULL){   
    _yaw_compass_diff = _normalize_angle(compass_heading-MPU_yaw);  // First time we took the compass solution
  }

  //if (fabs(MPU_yaw - compass_heading)>0.5){   // Error in radians
  // We have a big difference between gyro and compass ?? => compass problem ??
  // Whay should we do? Reduce compass_correction_gain?
  //}

  // Update the differential component to redude the error (it´s like a P control)
  _yaw_compass_diff += _normalize_angle(compass_heading - MPU_yaw)*_compass_correction_gain;
}

// Get the corrected yaw with the compass fusion
void get_yaw_compass_corrected()
{
  // safety check
  if ((_yaw_compass_diff<2*M_PI)&&(_yaw_compass_diff>-2*M_PI)){
    MPU_yaw_without_compass = MPU_yaw;
    MPU_yaw = _normalize_angle(MPU_yaw+_yaw_compass_diff);
  }
}

// Function to correct the gyroX and gyroY bias (roll and pitch) using the gravity vector from accelerometers
// We use the internal chip axis definition to make the bias correction because both sensors outputs (gyros and accels)
// are in chip axis definition
void gyro_bias_correction_from_gravity()
{
  float errorRollPitch[2];
  uint8_t i;

  // We take the accelerometer readings and cumulate to average them and obtain the gravity vector
  _accel_filtered[0] += accelX-MPU6000_ACCEL_OFFSET[0];
  _accel_filtered[1] += accelY-MPU6000_ACCEL_OFFSET[1];
  _accel_filtered[2] += accelZ-MPU6000_ACCEL_OFFSET[2];
  _accel_filtered_samples++;

  _gyro_bias_from_gravity_counter++;
  // We make the bias calculation and correction at a lower rate (25Hz)
  if (_gyro_bias_from_gravity_counter==GYRO_BIAS_FROM_GRAVITY_RATE){
    _gyro_bias_from_gravity_counter = 0;

    for (i=0;i<3;i++){
      _accel_filtered[i] = _accel_filtered[i]/_accel_filtered_samples;  // average
    }

    // Adjust ground reference : Accel Cross Gravity to obtain the error between gravity from accels and gravity from attitude solution
    // We use the  _DCM rotation matrix (in chip axis definition)
    // errorRollPitch are in Accel LSB units
    errorRollPitch[0] = _accel_filtered[1] * _DCM[2][2] - _accel_filtered[2] * _DCM[2][1];
    errorRollPitch[1] = _accel_filtered[2] * _DCM[2][0] - _accel_filtered[0] * _DCM[2][2];    

    // We correct gyroX and gyroY bias using the error vector
    _gyro_bias[0] += errorRollPitch[0]*_gyro_bias_from_gravity_gain;
    _gyro_bias[1] += errorRollPitch[1]*_gyro_bias_from_gravity_gain;

    // If bias values are greater than 1 LSB we update the hardware offset registers
    if (fabs(_gyro_bias[0])>1){
      _gyro_offset_update(-1*(int)_gyro_bias[0],0,0);
      _gyro_bias[0] -= (int)_gyro_bias[0];  // we remove the part that we have already corrected on registers...
    }
    if (fabs(_gyro_bias[1])>1){
      _gyro_offset_update(0,-1*(int)_gyro_bias[1],0);
      _gyro_bias[1] -= (int)_gyro_bias[1];
    }

    // Reset the accelerometer variables
    for (i=0;i<3;i++){
      _accel_filtered[i] = 0;
    }
    _accel_filtered_samples=0;
  }
}

void FIFO_reset() 
{
  uint8_t temp;
  temp = _SPI_read(MPUREG_USER_CTRL);
  temp = temp | 0x04;                           // FIFO RESET BIT
  _SPI_write(MPUREG_USER_CTRL, temp);
  _MPU6000_newdata = 0;   // clear new data flag
} 


// Read a packet from FIFO
void FIFO_getPacket()
{   
	uint8_t i;

  uint8_t dump;
  uint8_t return_value;
  uint8_t addr = MPUREG_FIFO_R_W | 0x80; // Set most significant bit  
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(addr);
  for(i = 0; i < _fifoCountL; i++){
    _received_packet[i] = SPI_transfer(0);
  }  
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
  _MPU6000_newdata = 0;   // clear new data flag
}

// New data packet in FIFO?
boolean FIFO_ready()
{
  _fifoCountH = _SPI_read(MPUREG_FIFO_COUNTH);
  _fifoCountL = _SPI_read(MPUREG_FIFO_COUNTL);
  //Serial.println(_fifoCountL, DEC);
  if(_fifoCountL == FIFO_PACKET_SIZE){ 
    return 1;
  }
  else{
    //We should not reach this point or maybe we have more than one packet (we should manage this!)
    //Serial.println(_fifoCountL, DEC);
    FIFO_reset();
    return 0;
  }
}

// Funtion to set the gain of the accel in the sensor fusion
void set_accel_fusion_gain(uint8_t gain)
{
  // safety checks
  if (gain < 0x20)  
    gain = 0x20;
  _set_sensor_fusion_accel_gain(gain);
}

// Funtion to set the gain of the bias correction from gravity
void set_gyro_bias_correction_from_gravity_gain(float gain)
{
  _gyro_bias_from_gravity_gain = gain;
}

// Funtion to set the gain of the compass fusion
void set_compass_correction_gain(float gain)
{
  _compass_correction_gain = gain;
}


/* ================ PRIVATE FUNCTIONS ==================== */

// MPU6000 INTERRUPT ON INT6
void MPU6000_newdata_int()
{
  _MPU6000_newdata++;
}

uint8_t _SPI_read(uint8_t reg)
{
  uint8_t dump;
  uint8_t return_value;
  uint8_t addr = reg | 0x80; // Set most significant bit
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(addr);
  return_value = SPI_transfer(0);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
  return(return_value);
}

void _SPI_write(uint8_t reg, uint8_t data)
{
  uint8_t dump;
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(reg);
  dump = SPI_transfer(data);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);  
}


// Function to write bytes into mpu memory (bank, address)  
void _set_mpu_memory(uint8_t bank, uint8_t address, uint8_t num, uint8_t regs[])
{
	uint8_t i;
  uint8_t dump;

  _SPI_write(MPUREG_BANK_SEL,bank);
  _SPI_write(MPUREG_MEM_START_ADDR,address);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(MPUREG_MEM_R_W);
  for(i = 0; i < num; i++){
    dump = SPI_transfer(regs[i]);
  }
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
}

//Apply default gyro calibration FS=2000dps and default orientation
void _set_dmp_gyro_calibration()
{       
  uint8_t regs[4];
  regs[0]=0x4C; 
  regs[1]=0xCD; 
  regs[2]=0x6C;
  _set_mpu_memory(0x03, 0x7B, 0x03, regs);              //FCFG_1 inv_set_gyro_calibration
  regs[0]=0x36; 
  regs[1]=0x56; 
  regs[2]=0x76;
  _set_mpu_memory(0x03, 0xAB, 0x03, regs);              //FCFG_3 inv_set_gyro_calibration
  regs[0]=0x02; 
  regs[1]=0xCB; 
  regs[2]=0x47; 
  regs[3]=0xA2;
  _set_mpu_memory(0x00, 0x68, 0x04, regs);              //D_0_104 inv_set_gyro_calibration
  regs[0]=0x00; 
  regs[1]=0x05; 
  regs[2]=0x8B; 
  regs[3]=0xC1;
  _set_mpu_memory(0x02, 0x18, 0x04, regs);              //D_0_24 inv_set_gyro_calibration
}

//Aply default accel calibration scale=2g and default orientation
void _set_dmp_accel_calibration()
{       
  uint8_t regs[6];
  regs[0]=0x00; 
  regs[1]=0x00; 
  regs[2]=0x00; 
  regs[3]=0x00;
  _set_mpu_memory(0x01, 0x0C, 0x04, regs);              //D_1_152 inv_set_accel_calibration
  regs[0]=0x0C; 
  regs[1]=0xC9; 
  regs[2]=0x2C; 
  regs[3]=0x97; 
  regs[4]=0x97; 
  regs[5]=0x97;
  _set_mpu_memory(0x03, 0x7F, 0x06, regs);              //FCFG_2 inv_set_accel_calibration
  regs[0]=0x26; 
  regs[1]=0x46; 
  regs[2]=0x66;
  _set_mpu_memory(0x03, 0x89, 0x03, regs);              //FCFG_7 inv_set_accel_calibration
  // accel range, 0x20,0x00 => 2g, 0x10,0x00=>4g    regs= (1073741824/accel_scale*65536)
  regs[0]=0x20; 
  regs[1]=0x00;
  _set_mpu_memory(0x00, 0x6C, 0x02, regs);              //D_0_108 inv_set_accel_calibration
}

void _apply_endian_accel()
{
  uint8_t regs[4];
  regs[0]=0x00; 
  regs[1]=0x00; 
  regs[2]=0x40; 
  regs[3]=0x00;
  _set_mpu_memory(0x01, 0xEC, 0x04, regs);   //D_1_236 inv_apply_endian_accel
}

// To configure for SIX_AXIS output
void _set_mpu_sensors()
{
  uint8_t regs[6];
  regs[0]=0x0C; 
  regs[1]=0xC9; 
  regs[2]=0x2C; 
  regs[3]=0x97; 
  regs[4]=0x97; 
  regs[5]=0x97;
  _set_mpu_memory(0x03, 0x7F, 0x06, regs);   //FCFG_2  inv_set_mpu_sensors(INV_SIX_AXIS_GYRO_ACCEL);
}

// Turn on bias from no motion
void _set_bias_from_no_motion()
{
  uint8_t regs[4];
  regs[0]=0x0D; 
  regs[1]=0x35; 
  regs[2]=0x5D;
  _set_mpu_memory(0x04, 0x02, 0x03, regs);   //CFG_MOTION_BIAS inv_turn_on_bias_from_no_motion
  regs[0]=0x87; 
  regs[1]=0x2D; 
  regs[2]=0x35; 
  regs[3]=0x3D;
  _set_mpu_memory(0x04, 0x09, 0x04, regs);   //FCFG_5 inv_set_bias_update( INV_BIAS_FROM_NO_MOTION );
}

// Turn off internal bias correction
void _set_bias_none()
{
  uint8_t regs[4];
  regs[0]=0x98; 
  regs[1]=0x98; 
  regs[2]=0x98;
  _set_mpu_memory(0x04, 0x02, 0x03, regs);   //CFG_MOTION_BIAS inv_turn_off_bias_from_no_motion
  regs[0]=0x87; 
  regs[1]=0x2D; 
  regs[2]=0x35; 
  regs[3]=0x3D;
  _set_mpu_memory(0x04, 0x09, 0x04, regs);   //FCFG_5 inv_set_bias_update( INV_BIAS_FROM_NO_MOTION );
}

void _set_fifo_interrupt()
{
  uint8_t regs[1];
  regs[0]=0xFE;
  _set_mpu_memory(0x07, 0x86, 0x01, regs);   //CFG_6 inv_set_fifo_interupt
}

// Send quaternion data to FIFO
void _send_quaternion()
{
  uint8_t regs[5];
  regs[0]=0xF1; 
  regs[1]=0x20; 
  regs[2]=0x28; 
  regs[3]=0x30; 
  regs[4]=0x38;
  _set_mpu_memory(0x07, 0x41, 0x05, regs);   //CFG_8 inv_send_quaternion
  regs[0]=0x30;
  _set_mpu_memory(0x07, 0x7E, 0x01, regs);   //CFG_16 inv_set_footer
}

// Send gyro data to FIFO
void _send_gyro()
{
  uint8_t regs[4];
  regs[0]=0xF1; 
  regs[1]=0x28; 
  regs[2]=0x30; 
  regs[3]=0x38;
  _set_mpu_memory(0x07, 0x47, 0x04, regs);   //CFG_9 inv_send_gyro
}

// Send accel data to FIFO
void _send_accel()
{
  uint8_t regs[54];  
  regs[0]=0xF1; 
  regs[1]=0x28; 
  regs[2]=0x30; 
  regs[3]=0x38;
  _set_mpu_memory(0x07, 0x6C, 0x04, regs);   //CFG_12 inv_send_accel
}

// This functions defines the rate at wich attitude data is send to FIFO
// Rate: 0 => SAMPLE_RATE(ex:200Hz), 1=> SAMPLE_RATE/2 (ex:100Hz), 2=> SAMPLE_RATE/3 (ex:66Hz)
// rate constant definitions in MPU6000.h
void _set_fifo_rate(uint8_t rate)
{
  uint8_t regs[2];
  regs[0]=0x00; 
  regs[1]=rate;
  _set_mpu_memory(0x02, 0x16, 0x02, regs);              //D_0_22 inv_set_fifo_rate  
}

// This function defines the weight of the accel on the sensor fusion
// default value is 0x80
// The official invensense name is inv_key_0_96 (??)
void _set_sensor_fusion_accel_gain(uint8_t gain)
{
  uint8_t dump;
  //inv_key_0_96
  _SPI_write(MPUREG_BANK_SEL,0x00);             
  _SPI_write(MPUREG_MEM_START_ADDR, 0x60);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(MPUREG_MEM_R_W);
  dump = SPI_transfer(0x00);
  dump = SPI_transfer(gain);  // Original : 0x80    To test: 0x40,  0x20 (too less)
  dump = SPI_transfer(0x00);
  dump = SPI_transfer(0x00);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
}

// Load initial memory values into DMP memory banks
void _dmp_load_mem()
{
	int i, k;
	uint8_t j;
  uint8_t dump;

  for(i = 0; i < 7; i++){
    _SPI_write(MPUREG_BANK_SEL,i);      //MPUREG_BANK_SEL
    for(j = 0; j < 16; j++){
      uint8_t start_addy = j * 0x10;
      _SPI_write(MPUREG_MEM_START_ADDR,start_addy);
      digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
      dump = SPI_transfer(MPUREG_MEM_R_W);        
      for(k = 0; k < 16; k++){
        unsigned char byteToSend = pgm_read_byte(&(dmpMem[i][j][k]));
        dump = SPI_transfer((uint8_t) byteToSend);
      }
      digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
    }
  }

  _SPI_write(MPUREG_BANK_SEL,7);      //MPUREG_BANK_SEL
  for(j = 0; j < 8; j++){
    uint8_t start_addy = j * 0x10;
    _SPI_write(MPUREG_MEM_START_ADDR,start_addy);
    digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
    dump = SPI_transfer(MPUREG_MEM_R_W);          
    for(k = 0; k < 16; k++){
      unsigned char byteToSend = pgm_read_byte(&(dmpMem[7][j][k]));
      dump = SPI_transfer((uint8_t) byteToSend);
    }
    digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
  }

  _SPI_write(MPUREG_MEM_START_ADDR,0x80);
  digitalWrite(MPU6000_CHIP_SELECT_PIN, LOW);
  dump = SPI_transfer(MPUREG_MEM_R_W);

  for(k = 0; k < 9; k++){
    unsigned char byteToSend = pgm_read_byte(&(dmpMem[7][8][k]));
    dump = SPI_transfer((uint8_t) byteToSend);
  }
  digitalWrite(MPU6000_CHIP_SELECT_PIN, HIGH);
}


// Normalize an angle between -PI and PI
float _normalize_angle(float angle)
{
  if (angle>M_PI){
    return(angle-2*M_PI);
  }
  else if (angle<-M_PI){
    return(angle+2*M_PI);
  }
  else{
    return(angle);
  }
}

// Update gyro offsets with new values. New offset values are substracted to actual offset values.
// offset values in gyro LSB units (as read from registers)
void _gyro_offset_update(int offsetX, int offsetY, int offsetZ)
{
  int aux_int;

  if (offsetX != 0){
    // Read actual value
    aux_int = (_SPI_read(MPUREG_XG_OFFS_USRH)<<8) | _SPI_read(MPUREG_XG_OFFS_USRL);
    aux_int -= offsetX<<1;   // Adjust to internal units
    // Write to MPU registers
    _SPI_write(MPUREG_XG_OFFS_USRH, (aux_int>>8)&0xFF);
    _SPI_write(MPUREG_XG_OFFS_USRL, aux_int&0xFF);
  }
  if (offsetY != 0){
    aux_int = (_SPI_read(MPUREG_YG_OFFS_USRH)<<8) | _SPI_read(MPUREG_YG_OFFS_USRL);
    aux_int -= offsetY<<1;   // Adjust to internal units
    // Write to MPU registers
    _SPI_write(MPUREG_YG_OFFS_USRH, (aux_int>>8)&0xFF);
    _SPI_write(MPUREG_YG_OFFS_USRL, aux_int&0xFF);
  }
  if (offsetZ != 0){
    aux_int = (_SPI_read(MPUREG_ZG_OFFS_USRH)<<8) | _SPI_read(MPUREG_ZG_OFFS_USRL);
    aux_int -= offsetZ<<1;   // Adjust to internal units
    // Write to MPU registers
    _SPI_write(MPUREG_ZG_OFFS_USRH, (aux_int>>8)&0xFF);
    _SPI_write(MPUREG_ZG_OFFS_USRL, aux_int&0xFF);
  }             
}

// TO IMPLEMENT: CENTRIFUGAL FORCE CORRECTION!!
void accel_centrifugal_force_correction(float speed)
{
}

// NOT TESTED (ON DEVELOPMENT)
float _gyro_bias_correction_from_compass(float heading)
{
  float heading_y;
  float heading_x;
  float errorCourse;
  float errorYaw;

  //errorCourse= (_DCM[0][0]*heading_y) - (_DCM[1][0]*heading_x);  //Calculating YAW error
  errorCourse = _yaw_compass_diff;
  errorYaw = _DCM[2][2]*errorCourse;  // Calculate the component that affect the yaw gyro    

  // We correct gyroZ bias using the compass solution
  _gyro_bias[2] += errorYaw*_gyro_bias_from_compass_gain;

  // If bias values are greater than 1 LSB we update the hardware offset registers
  if (fabs(_gyro_bias[2])>1){
    _gyro_offset_update(0,0,-1*(int)_gyro_bias[2]);
    _gyro_bias[2] -= (int)_gyro_bias[2];  // we remove the part that we have already corrected on registers...
  }
	return 0; // robd
}


// ========= DMP MEMORY ================================

//dmpMem from dmpDefaultMantis.c
//unsigned char dmpMem[8][16][16] PROGMEM = {
unsigned char dmpMem[8][16][16] = {
  {
    {
      0xFB, 0x00, 0x00, 0x3E, 0x00, 0x0B, 0x00, 0x36, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00    }
    ,
    {
      0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0xFA, 0x80, 0x00, 0x0B, 0x12, 0x82, 0x00, 0x01    }
    ,
    {
      0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x28, 0x00, 0x00, 0xFF, 0xFF, 0x45, 0x81, 0xFF, 0xFF, 0xFA, 0x72, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x7F, 0xFF, 0xFF, 0xFE, 0x80, 0x01    }
    ,
    {
      0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x3E, 0x03, 0x30, 0x40, 0x00, 0x00, 0x00, 0x02, 0xCA, 0xE3, 0x09, 0x3E, 0x80, 0x00, 0x00    }
    ,
    {
      0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00    }
    ,
    {
      0x41, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x2A, 0x00, 0x00, 0x16, 0x55, 0x00, 0x00, 0x21, 0x82    }
    ,
    {
      0xFD, 0x87, 0x26, 0x50, 0xFD, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x05, 0x80, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00    }
    ,
    {
      0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6F, 0x00, 0x02, 0x65, 0x32, 0x00, 0x00, 0x5E, 0xC0    }
    ,
    {
      0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0xFB, 0x8C, 0x6F, 0x5D, 0xFD, 0x5D, 0x08, 0xD9, 0x00, 0x7C, 0x73, 0x3B, 0x00, 0x6C, 0x12, 0xCC    }
    ,
    {
      0x32, 0x00, 0x13, 0x9D, 0x32, 0x00, 0xD0, 0xD6, 0x32, 0x00, 0x08, 0x00, 0x40, 0x00, 0x01, 0xF4    }
    ,
    {
      0xFF, 0xE6, 0x80, 0x79, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xD6, 0x00, 0x00, 0x27, 0x10    }
  }
  ,
  {
    {
      0xFB, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0xFA, 0x36, 0xFF, 0xBC, 0x30, 0x8E, 0x00, 0x05, 0xFB, 0xF0, 0xFF, 0xD9, 0x5B, 0xC8    }
    ,
    {
      0xFF, 0xD0, 0x9A, 0xBE, 0x00, 0x00, 0x10, 0xA9, 0xFF, 0xF4, 0x1E, 0xB2, 0x00, 0xCE, 0xBB, 0xF7    }
    ,
    {
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x0C    }
    ,
    {
      0xFF, 0xC2, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0xCF, 0x80, 0x00, 0x40, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x14    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x68, 0xB6, 0x79, 0x35, 0x28, 0xBC, 0xC6, 0x7E, 0xD1, 0x6C    }
    ,
    {
      0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB2, 0x6A, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x30    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x25, 0x4D, 0x00, 0x2F, 0x70, 0x6D, 0x00, 0x00, 0x05, 0xAE, 0x00, 0x0C, 0x02, 0xD0    }
  }
  ,
  {
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x01, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0xFF, 0xEF, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    }
  }
  ,
  {
    {
      0xD8, 0xDC, 0xBA, 0xA2, 0xF1, 0xDE, 0xB2, 0xB8, 0xB4, 0xA8, 0x81, 0x91, 0xF7, 0x4A, 0x90, 0x7F    }
    ,
    {
      0x91, 0x6A, 0xF3, 0xF9, 0xDB, 0xA8, 0xF9, 0xB0, 0xBA, 0xA0, 0x80, 0xF2, 0xCE, 0x81, 0xF3, 0xC2    }
    ,
    {
      0xF1, 0xC1, 0xF2, 0xC3, 0xF3, 0xCC, 0xA2, 0xB2, 0x80, 0xF1, 0xC6, 0xD8, 0x80, 0xBA, 0xA7, 0xDF    }
    ,
    {
      0xDF, 0xDF, 0xF2, 0xA7, 0xC3, 0xCB, 0xC5, 0xB6, 0xF0, 0x87, 0xA2, 0x94, 0x24, 0x48, 0x70, 0x3C    }
    ,
    {
      0x95, 0x40, 0x68, 0x34, 0x58, 0x9B, 0x78, 0xA2, 0xF1, 0x83, 0x92, 0x2D, 0x55, 0x7D, 0xD8, 0xB1    }
    ,
    {
      0xB4, 0xB8, 0xA1, 0xD0, 0x91, 0x80, 0xF2, 0x70, 0xF3, 0x70, 0xF2, 0x7C, 0x80, 0xA8, 0xF1, 0x01    }
    ,
    {
      0xB0, 0x98, 0x87, 0xD9, 0x43, 0xD8, 0x86, 0xC9, 0x88, 0xBA, 0xA1, 0xF2, 0x0E, 0xB8, 0x97, 0x80    }
    ,
    {
      0xF1, 0xA9, 0xDF, 0xDF, 0xDF, 0xAA, 0xDF, 0xDF, 0xDF, 0xF2, 0xAA, 0xC5, 0xCD, 0xC7, 0xA9, 0x0C    }
    ,
    {
      0xC9, 0x2C, 0x97, 0x97, 0x97, 0x97, 0xF1, 0xA9, 0x89, 0x26, 0x46, 0x66, 0xB0, 0xB4, 0xBA, 0x80    }
    ,
    {
      0xAC, 0xDE, 0xF2, 0xCA, 0xF1, 0xB2, 0x8C, 0x02, 0xA9, 0xB6, 0x98, 0x00, 0x89, 0x0E, 0x16, 0x1E    }
    ,
    {
      0xB8, 0xA9, 0xB4, 0x99, 0x2C, 0x54, 0x7C, 0xB0, 0x8A, 0xA8, 0x96, 0x36, 0x56, 0x76, 0xF1, 0xB9    }
    ,
    {
      0xAF, 0xB4, 0xB0, 0x83, 0xC0, 0xB8, 0xA8, 0x97, 0x11, 0xB1, 0x8F, 0x98, 0xB9, 0xAF, 0xF0, 0x24    }
    ,
    {
      0x08, 0x44, 0x10, 0x64, 0x18, 0xF1, 0xA3, 0x29, 0x55, 0x7D, 0xAF, 0x83, 0xB5, 0x93, 0xAF, 0xF0    }
    ,
    {
      0x00, 0x28, 0x50, 0xF1, 0xA3, 0x86, 0x9F, 0x61, 0xA6, 0xDA, 0xDE, 0xDF, 0xD9, 0xFA, 0xA3, 0x86    }
    ,
    {
      0x96, 0xDB, 0x31, 0xA6, 0xD9, 0xF8, 0xDF, 0xBA, 0xA6, 0x8F, 0xC2, 0xC5, 0xC7, 0xB2, 0x8C, 0xC1    }
    ,
    {
      0xB8, 0xA2, 0xDF, 0xDF, 0xDF, 0xA3, 0xDF, 0xDF, 0xDF, 0xD8, 0xD8, 0xF1, 0xB8, 0xA8, 0xB2, 0x86    }
  }
  ,
  {
    {
      0xB4, 0x98, 0x0D, 0x35, 0x5D, 0xB8, 0xAA, 0x98, 0xB0, 0x87, 0x2D, 0x35, 0x3D, 0xB2, 0xB6, 0xBA    }
    ,
    {
      0xAF, 0x8C, 0x96, 0x19, 0x8F, 0x9F, 0xA7, 0x0E, 0x16, 0x1E, 0xB4, 0x9A, 0xB8, 0xAA, 0x87, 0x2C    }
    ,
    {
      0x54, 0x7C, 0xB9, 0xA3, 0xDE, 0xDF, 0xDF, 0xA3, 0xB1, 0x80, 0xF2, 0xC4, 0xCD, 0xC9, 0xF1, 0xB8    }
    ,
    {
      0xA9, 0xB4, 0x99, 0x83, 0x0D, 0x35, 0x5D, 0x89, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0xB5, 0x93, 0xA3    }
    ,
    {
      0x0E, 0x16, 0x1E, 0xA9, 0x2C, 0x54, 0x7C, 0xB8, 0xB4, 0xB0, 0xF1, 0x97, 0x83, 0xA8, 0x11, 0x84    }
    ,
    {
      0xA5, 0x09, 0x98, 0xA3, 0x83, 0xF0, 0xDA, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xD8, 0xF1, 0xA5    }
    ,
    {
      0x29, 0x55, 0x7D, 0xA5, 0x85, 0x95, 0x02, 0x1A, 0x2E, 0x3A, 0x56, 0x5A, 0x40, 0x48, 0xF9, 0xF3    }
    ,
    {
      0xA3, 0xD9, 0xF8, 0xF0, 0x98, 0x83, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0x97, 0x82, 0xA8, 0xF1    }
    ,
    {
      0x11, 0xF0, 0x98, 0xA2, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xDA, 0xF3, 0xDE, 0xD8, 0x83, 0xA5    }
    ,
    {
      0x94, 0x01, 0xD9, 0xA3, 0x02, 0xF1, 0xA2, 0xC3, 0xC5, 0xC7, 0xD8, 0xF1, 0x84, 0x92, 0xA2, 0x4D    }
    ,
    {
      0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9    }
    ,
    {
      0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0x93, 0xA3, 0x4D    }
    ,
    {
      0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9    }
    ,
    {
      0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0xA8, 0x8A, 0x9A    }
    ,
    {
      0xF0, 0x28, 0x50, 0x78, 0x9E, 0xF3, 0x88, 0x18, 0xF1, 0x9F, 0x1D, 0x98, 0xA8, 0xD9, 0x08, 0xD8    }
    ,
    {
      0xC8, 0x9F, 0x12, 0x9E, 0xF3, 0x15, 0xA8, 0xDA, 0x12, 0x10, 0xD8, 0xF1, 0xAF, 0xC8, 0x97, 0x87    }
  }
  ,
  {
    {
      0x34, 0xB5, 0xB9, 0x94, 0xA4, 0x21, 0xF3, 0xD9, 0x22, 0xD8, 0xF2, 0x2D, 0xF3, 0xD9, 0x2A, 0xD8    }
    ,
    {
      0xF2, 0x35, 0xF3, 0xD9, 0x32, 0xD8, 0x81, 0xA4, 0x60, 0x60, 0x61, 0xD9, 0x61, 0xD8, 0x6C, 0x68    }
    ,
    {
      0x69, 0xD9, 0x69, 0xD8, 0x74, 0x70, 0x71, 0xD9, 0x71, 0xD8, 0xB1, 0xA3, 0x84, 0x19, 0x3D, 0x5D    }
    ,
    {
      0xA3, 0x83, 0x1A, 0x3E, 0x5E, 0x93, 0x10, 0x30, 0x81, 0x10, 0x11, 0xB8, 0xB0, 0xAF, 0x8F, 0x94    }
    ,
    {
      0xF2, 0xDA, 0x3E, 0xD8, 0xB4, 0x9A, 0xA8, 0x87, 0x29, 0xDA, 0xF8, 0xD8, 0x87, 0x9A, 0x35, 0xDA    }
    ,
    {
      0xF8, 0xD8, 0x87, 0x9A, 0x3D, 0xDA, 0xF8, 0xD8, 0xB1, 0xB9, 0xA4, 0x98, 0x85, 0x02, 0x2E, 0x56    }
    ,
    {
      0xA5, 0x81, 0x00, 0x0C, 0x14, 0xA3, 0x97, 0xB0, 0x8A, 0xF1, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9    }
    ,
    {
      0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x84, 0x0D, 0xDA, 0x0E, 0xD8, 0xA3, 0x29, 0x83, 0xDA    }
    ,
    {
      0x2C, 0x0E, 0xD8, 0xA3, 0x84, 0x49, 0x83, 0xDA, 0x2C, 0x4C, 0x0E, 0xD8, 0xB8, 0xB0, 0xA8, 0x8A    }
    ,
    {
      0x9A, 0xF5, 0x20, 0xAA, 0xDA, 0xDF, 0xD8, 0xA8, 0x40, 0xAA, 0xD0, 0xDA, 0xDE, 0xD8, 0xA8, 0x60    }
    ,
    {
      0xAA, 0xDA, 0xD0, 0xDF, 0xD8, 0xF1, 0x97, 0x86, 0xA8, 0x31, 0x9B, 0x06, 0x99, 0x07, 0xAB, 0x97    }
    ,
    {
      0x28, 0x88, 0x9B, 0xF0, 0x0C, 0x20, 0x14, 0x40, 0xB8, 0xB0, 0xB4, 0xA8, 0x8C, 0x9C, 0xF0, 0x04    }
    ,
    {
      0x28, 0x51, 0x79, 0x1D, 0x30, 0x14, 0x38, 0xB2, 0x82, 0xAB, 0xD0, 0x98, 0x2C, 0x50, 0x50, 0x78    }
    ,
    {
      0x78, 0x9B, 0xF1, 0x1A, 0xB0, 0xF0, 0x8A, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x8B, 0x29, 0x51, 0x79    }
    ,
    {
      0x8A, 0x24, 0x70, 0x59, 0x8B, 0x20, 0x58, 0x71, 0x8A, 0x44, 0x69, 0x38, 0x8B, 0x39, 0x40, 0x68    }
    ,
    {
      0x8A, 0x64, 0x48, 0x31, 0x8B, 0x30, 0x49, 0x60, 0xA5, 0x88, 0x20, 0x09, 0x71, 0x58, 0x44, 0x68    }
  }
  ,
  {
    {
      0x11, 0x39, 0x64, 0x49, 0x30, 0x19, 0xF1, 0xAC, 0x00, 0x2C, 0x54, 0x7C, 0xF0, 0x8C, 0xA8, 0x04    }
    ,
    {
      0x28, 0x50, 0x78, 0xF1, 0x88, 0x97, 0x26, 0xA8, 0x59, 0x98, 0xAC, 0x8C, 0x02, 0x26, 0x46, 0x66    }
    ,
    {
      0xF0, 0x89, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 0x31    }
    ,
    {
      0xA9, 0x88, 0x09, 0x20, 0x59, 0x70, 0xAB, 0x11, 0x38, 0x40, 0x69, 0xA8, 0x19, 0x31, 0x48, 0x60    }
    ,
    {
      0x8C, 0xA8, 0x3C, 0x41, 0x5C, 0x20, 0x7C, 0x00, 0xF1, 0x87, 0x98, 0x19, 0x86, 0xA8, 0x6E, 0x76    }
    ,
    {
      0x7E, 0xA9, 0x99, 0x88, 0x2D, 0x55, 0x7D, 0x9E, 0xB9, 0xA3, 0x8A, 0x22, 0x8A, 0x6E, 0x8A, 0x56    }
    ,
    {
      0x8A, 0x5E, 0x9F, 0xB1, 0x83, 0x06, 0x26, 0x46, 0x66, 0x0E, 0x2E, 0x4E, 0x6E, 0x9D, 0xB8, 0xAD    }
    ,
    {
      0x00, 0x2C, 0x54, 0x7C, 0xF2, 0xB1, 0x8C, 0xB4, 0x99, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0x81, 0x91    }
    ,
    {
      0xAC, 0x38, 0xAD, 0x3A, 0xB5, 0x83, 0x91, 0xAC, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9, 0x48, 0xD8    }
    ,
    {
      0x6D, 0xD9, 0x68, 0xD8, 0x8C, 0x9D, 0xAE, 0x29, 0xD9, 0x04, 0xAE, 0xD8, 0x51, 0xD9, 0x04, 0xAE    }
    ,
    {
      0xD8, 0x79, 0xD9, 0x04, 0xD8, 0x81, 0xF3, 0x9D, 0xAD, 0x00, 0x8D, 0xAE, 0x19, 0x81, 0xAD, 0xD9    }
    ,
    {
      0x01, 0xD8, 0xF2, 0xAE, 0xDA, 0x26, 0xD8, 0x8E, 0x91, 0x29, 0x83, 0xA7, 0xD9, 0xAD, 0xAD, 0xAD    }
    ,
    {
      0xAD, 0xF3, 0x2A, 0xD8, 0xD8, 0xF1, 0xB0, 0xAC, 0x89, 0x91, 0x3E, 0x5E, 0x76, 0xF3, 0xAC, 0x2E    }
    ,
    {
      0x2E, 0xF1, 0xB1, 0x8C, 0x5A, 0x9C, 0xAC, 0x2C, 0x28, 0x28, 0x28, 0x9C, 0xAC, 0x30, 0x18, 0xA8    }
    ,
    {
      0x98, 0x81, 0x28, 0x34, 0x3C, 0x97, 0x24, 0xA7, 0x28, 0x34, 0x3C, 0x9C, 0x24, 0xF2, 0xB0, 0x89    }
    ,
    {
      0xAC, 0x91, 0x2C, 0x4C, 0x6C, 0x8A, 0x9B, 0x2D, 0xD9, 0xD8, 0xD8, 0x51, 0xD9, 0xD8, 0xD8, 0x79    }
  }
  ,
  {
    {
      0xD9, 0xD8, 0xD8, 0xF1, 0x9E, 0x88, 0xA3, 0x31, 0xDA, 0xD8, 0xD8, 0x91, 0x2D, 0xD9, 0x28, 0xD8    }
    ,
    {
      0x4D, 0xD9, 0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x83, 0x93, 0x35, 0x3D, 0x80, 0x25, 0xDA    }
    ,
    {
      0xD8, 0xD8, 0x85, 0x69, 0xDA, 0xD8, 0xD8, 0xB4, 0x93, 0x81, 0xA3, 0x28, 0x34, 0x3C, 0xF3, 0xAB    }
    ,
    {
      0x8B, 0xF8, 0xA3, 0x91, 0xB6, 0x09, 0xB4, 0xD9, 0xAB, 0xDE, 0xFA, 0xB0, 0x87, 0x9C, 0xB9, 0xA3    }
    ,
    {
      0xDD, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x95, 0xF1, 0xA3, 0xA3, 0xA3, 0x9D, 0xF1, 0xA3, 0xA3, 0xA3    }
    ,
    {
      0xA3, 0xF2, 0xA3, 0xB4, 0x90, 0x80, 0xF2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3    }
    ,
    {
      0xA3, 0xB2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xB0, 0x87, 0xB5, 0x99, 0xF1, 0xA3, 0xA3, 0xA3    }
    ,
    {
      0x98, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x97, 0xA3, 0xA3, 0xA3, 0xA3, 0xF3, 0x9B, 0xA3, 0xA3, 0xDC    }
    ,
    {
      0xB9, 0xA7, 0xF1, 0x26, 0x26, 0x26, 0xD8, 0xD8, 0xFF    }
  }
};

