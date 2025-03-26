#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <CytronMotorDriver.h>

// 엔코더 핀 정의
#define ENCODER_A1 20
#define ENCODER_A2 21 
#define ENCODER_A3 2
#define ENCODER_A4 3

// 모터 객체 생성
CytronMD motors[] = {
  CytronMD(PWM_PWM, 5, 6),  // 왼쪽 앞 바퀴 모터
  CytronMD(PWM_PWM, 7, 8),  // 오른쪽 앞 바퀴 모터
  CytronMD(PWM_PWM, 9, 10), // 왼쪽 뒷 바퀴 모터
  CytronMD(PWM_PWM, 11, 12) // 오른쪽 뒷 바퀴 모터
};

// 엔코더 카운터와 모터 방향
volatile long encoderCounts[4] = {0, 0, 0, 0};
volatile int motorDirections[4] = {1, 1, 1, 1};

// ROS 설정
ros::NodeHandle nh;

// 엔코더 메시지 설정
std_msgs::Int32MultiArray encoder_msg;
int32_t encoder_data[4];  // 실제 데이터를 저장할 배열

// motor_speed topic 콜백 함수
void motorSpeedCallback(const std_msgs::Float32MultiArray& motor_speed) {
  if(motor_speed.data_length >= 4) {
    for(int i = 0; i < 4; i++) {
      motors[i].setSpeed(motor_speed.data[i]);
      motorDirections[i] = (motor_speed.data[i] >= 0) ? 1 : -1;
    }
  }
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("motor_speed", &motorSpeedCallback);
ros::Publisher encoderPub("encoder", &encoder_msg);

void setup() {
  // 엔코더 핀 설정
  pinMode(ENCODER_A1, INPUT_PULLUP);
  pinMode(ENCODER_A2, INPUT_PULLUP);
  pinMode(ENCODER_A3, INPUT_PULLUP);
  pinMode(ENCODER_A4, INPUT_PULLUP);

  // 엔코더 인터럽트 설정
  attachInterrupt(digitalPinToInterrupt(ENCODER_A1), ISR_encoder1, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A2), ISR_encoder2, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A3), ISR_encoder3, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A4), ISR_encoder4, FALLING);

  // 엔코더 메시지 초기화
  encoder_msg.data_length = 4;
  encoder_msg.data = encoder_data;

  // ROS 초기화
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(encoderPub);

  delay(1);  // 초기화를 위한 짧은 대기
}

// 엔코더 인터럽트 핸들러
void ISR_encoder1() { encoderCounts[0] += motorDirections[0]; }
void ISR_encoder2() { encoderCounts[1] += motorDirections[1]; }
void ISR_encoder3() { encoderCounts[2] += motorDirections[2]; }
void ISR_encoder4() { encoderCounts[3] += motorDirections[3]; }

void loop() {
  // 엔코더 값을 메시지 데이터 배열에 복사
  noInterrupts();  // 인터럽트 비활성화
  for(int i = 0; i < 4; i++) {
    encoder_data[i] = encoderCounts[i];
  }
  interrupts();    // 인터럽트 재활성화

  encoderPub.publish(&encoder_msg);
  nh.spinOnce();
  delay(20);  // 통신 주기를 좀 더 길게 설정
}
