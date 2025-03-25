#include <ros.h>
#include <std_msgs/Int32.h>
#include <CytronMotorDriver.h>

// PID 제어 관련 상수 및 변수
#define ENCODER_A1 20
#define ENCODER_A2 21
#define ENCODER_A3 2
#define ENCODER_A4 3

#define TRIG 18   // 초음파 센서의 트리거 핀
#define ECHO 19  // 초음파 센서의 에코핀
#define TRIG1 16   // 초음파 센서의 트리거 핀
#define ECHO1 17  // 초음파 센서의 에코핀

// pd_control value
float kp = 0.1;
float kd = 0.05;
float prevError[4];

// 모터 객체 배열 생성
CytronMD motors[] = {
  CytronMD(PWM_PWM, 5, 6),  // 왼쪽 앞 바퀴 모터
  CytronMD(PWM_PWM, 7, 8),  // 오른쪽 앞 바퀴 모터
  CytronMD(PWM_PWM, 9, 10), // 왼쪽 뒷 바퀴 모터
  CytronMD(PWM_PWM, 11, 12) // 오른쪽 뒷 바퀴 모터
};

// 기본 속도 설정
const float w = 34;
float speeds[] = {w, w, w, w}; // 모터 속도 배열

// encoder zero const
volatile int encoderA[4] = {0, 0, 0, 0};
float direct[4] = {0, 0, 0, 0};
unsigned long timePrev = 0;
unsigned long timeCurr = 0;
float x = 0;
int move_default = 0;

// 초음파 value
volatile unsigned long distanceright;
volatile unsigned long distancefront;

// ros node 통신
ros::NodeHandle nh;

// ros구
std_msgs::Int32 outputMessage;
ros::Publisher pub("output", &outputMessage);

// 첫번째 과정(벽따라 이동)
void callBackFunction1(const std_msgs::Int32 &inputMessage) {
  if (inputMessage.data == 1) {
    scanright();
    scanfront();
    // 앞쪽 거리 조건을 우선적으로 처리
    if (distancefront > 0 && distancefront <= 30) {
      sd();
    } else {
      // 오른쪽 거리 조건을 처리
      if (distanceright > 0) {
        if (distanceright <= 10) {
          cross1();
        } else if (distanceright >= 25) {
          right();
        } else if (distanceright >= 8 && distanceright < 25) {
          straight();
        }
      }
    }
  }
}

// ros 카메라 구동

// 두번째 과정(회전)
void callBackFunction2(const std_msgs::Int32 &inputMessage) {
  if (inputMessage.data == 2) {
    scanright();
    scanfront();
  
    if (distanceright > 5) {
      right();
    } 
    else {
      unsigned long start_time = millis();
      sd();
      delay(15000);
      left();
      sd();
      rotate2();

      delay(13000);
      
      while (true) {
        scanright();
        scanfront();

        if (distancefront > 0 && distancefront <= 30) {
          sd();
        } else {
          if (distanceright > 0) {
            if (distanceright <= 10) {
              cross1();
            } else if (distanceright >= 25) {
              right();
            } else if (distanceright >= 8 && distanceright < 25) {
              straight();
            }
          }
        }

        // 특정 시간 후에 루프 종료
        if (millis() - start_time > 300000) {  // 300초 후 종료
          break;
        }
      }
    }
  }
}

void callBackFunction3(const std_msgs::Int32 &inputMessage) {
  if (inputMessage.data == 3) {
    scanright();
    scanfront();
    // 앞쪽 거리 조건을 우선적으로 처리
    if (distancefront > 0 && distancefront <= 30) {
      sd();
    } else {
      // 오른쪽 거리 조건을 처리
      if (distanceright > 0) {
        if (distanceright <= 10) {
          cross1();
        } else if (distanceright >= 25) {
          right();
        } else if (distanceright >= 8 && distanceright < 25) {
          straight();
        }
      }
    }
  }
}

ros::Subscriber<std_msgs::Int32> sub1("input1", &callBackFunction1);
ros::Subscriber<std_msgs::Int32> sub2("input2", &callBackFunction2);
ros::Subscriber<std_msgs::Int32> sub3("input3", &callBackFunction3);

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  // encode setup
  pinMode(ENCODER_A1, INPUT_PULLUP);
  pinMode(ENCODER_A2, INPUT_PULLUP);
  pinMode(ENCODER_A3, INPUT_PULLUP);
  pinMode(ENCODER_A4, INPUT_PULLUP);

  // interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_A1), ISR_encoderA0, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A2), ISR_encoderA1, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A3), ISR_encoderA2, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A4), ISR_encoderA3, FALLING);

  nh.initNode();
  nh.advertise(pub);
  nh.subscribe(sub1);
  nh.subscribe(sub2);
  nh.subscribe(sub3);
}

// encoder_input
void ISR_encoderA0() {
  encoderA[0]++;
}

void ISR_encoderA1() {
  encoderA[1]++;
}

void ISR_encoderA2() {
  encoderA[2]++;
}

void ISR_encoderA3() {
  encoderA[3]++;
}

// 모터 속도 설정 함수
void setMotorSpeeds() {
  for (int i = 0; i < 4; i++) {
    motors[i].setSpeed(speeds[i] * direct[i]);
  }
}

// 모터 제어 함수들
void straight() {
  float temp[] = {1, 1, 1, 1};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
  delay(1100);
}

void right() {
  float temp[] = {1, -1, -1, 1};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
  delay(1000);
  sd();
}

void left() {
  float temp[] = {-1, 1, 1, -1};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
  delay(5000);
  sd();
}

void cross1() {
  float temp[] = {0, 1, 1, 0};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
  delay(1000);
  sd();
}

void cross2() {
  float temp[] = {1, 0, 0, 1};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
  delay(100);
  sd();
}

void rotate2() {
  float temp[] = {-1, 1, -1, 1};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
  delay(1100);
}

void stop() {
  float temp[] = {0, 0, 0, 0};
  memcpy(direct, temp, sizeof(direct));
  setMotorSpeeds();
}

void scanright() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  distanceright = pulseIn(ECHO, HIGH) / 58.2;
}

void scanfront() {
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  distancefront = pulseIn(ECHO1, HIGH) / 58.2;
}

void encoder_convert() {
  timeCurr = millis();

  if (timeCurr - timePrev > 1000) {
    timePrev = timeCurr;

    noInterrupts();
    // 인터럽트 발생 수치 계산 후 초기화
    for (int i = 0; i < 4; i++) {
      if (direct[i] == 1) {
        // forward 방향의 경우 아무 것도 하지 않음
      } else if (direct[i] == -1) {
        encoderA[i] = -encoderA[i]; // backward 방향의 경우 음수로 변환
      } else {
        encoderA[i] = 0; // 정지한 경우 0으로 설정
      }
    }
    interrupts();
  }
  pd_control_encoder();
  calcul();
  // 변경된 엔코더 값 출력
}

void pd_control_encoder() {
  float error[4];
  float derivative[4];
  float correction[4];
  int target[4]; // 각 바퀴별 목표치

  // 각 바퀴별 목표치 설정
  for (int i = 0; i < 4; i++) {
    if (encoderA[i] > 0) {
      target[i] = 827; // 양수인 경우의 목표치 설정
    } else if (encoderA[i] < 0) {
      target[i] = -827; // 음수인 경우의 목표치 설정
    } else {
      target[i] = 0; // 0인 경우의 목표치 설정
    }
  }

  // 각 바퀴마다 PD 제어 계산
  for (int i = 0; i < 4; i++) {
    // 오차 계산
    error[i] = target[i] - encoderA[i];
    
    // 미분 계산
    derivative[i] = error[i] - prevError[i];
    
    // 보정 계산 (PD 제어)
    correction[i] = kp * error[i] + kd * derivative[i];
    
    // 이전 오차 업데이트
    prevError[i] = error[i];
    
    // 각 바퀴의 속도 보정
    encoderA[i] += correction[i];
  }
}

// 속도 계산 함수
void calcul() {
  float w_rad[4];

  for (int i = 0; i < 4; i++) {
    w_rad[i] = encoderA[i] / 14.63;
  }

  for (int i = 0; i < 4; i++) {
    speeds[i] = abs(w_rad[i] * 2.19);
  }

  for (int i = 0; i < 4; i++) {
    encoderA[i] = 0;
  }

  float Vx = 0.076 / 4 * (w_rad[0] - w_rad[1] - w_rad[2] + w_rad[3]);
  float Vy = 0.076 / 4 * (w_rad[0] + w_rad[1] + w_rad[2] + w_rad[3]);
  
  if (move_default == 0) {
    x += Vx * 1.1;
  } else if (move_default == 1) {
    x -= Vx * 1.1;
  }
}

void sd() {
  stop();
  delay(100);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
  
