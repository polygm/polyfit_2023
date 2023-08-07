/*
 * polytfit 모듈 코드
 * version 0.0.1
*/

// 모터 연결핀
#define motorPin1 5 // IN1
#define motorPin2 4 // IN2
#define motorPin3 3 // IN3
#define motorPin4 2 // IN4

// 모터 48byj 스템모터
#define step 2048 // 1바퀴 스텝수

// 모터 회전 Loop 문 구현을 간단하게 하기위해 핀 배열 선언
int pinArray[4] = { motorPin1, motorPin2, motorPin3, motorPin4 };

// CW 시계방향 데이터를 전송
int CW[4] = { 0b1000, 0b0100, 0b0010,0b0001 };
// CCW 반시계방향 데이터를 전송
int CCW[4] = { 0b0001, 0b0010, 0b0100, 0b1000 };

// 초기화
void setup(){
  // 스텝모터 드라이브 보드의 IN 연결핀 출력으로 설정
  for(int i = 0 ; i < 4 ; i++ ) {
    pinMode(pinArray[i], OUTPUT);
  }

}

// 모터 48byj 스템모터 딜레이
void stepDelay()
{
  // 모터 부하가 없는 상태에서, 
  // 각 스텝 간격이 최소 3ms 이상이어야 정상적으로 동작합니다.
  delay(3);
}


void loop() {  
  

  motor_cw(90);
  motor_ccw(90);

  
}

// 모터 정방향
void motor_cw(int deg) {
  int temp;
  int angle;

  // 입력한 각도에 따른 펄스수
  // step = 2048
  angle = (step / 360) * deg;

  // CW방향 1바퀴 후
  // step = 2048 한바퀴
  for(int i = 0 ; i < angle ; i++)
  {
        temp = i % 4;
        digitalWrite(pinArray[0], bitRead(CW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CW[temp], 3));
        stepDelay();
  }

  delay(1000);
}

void motor_ccw(int deg)
{
  int temp;
  int angle;

  // 입력한 각도에 따른 펄스수
  // step = 2048
  angle = (step / 360) * deg;

  // CCW방향 1바퀴 후
  // step = 2048 한바퀴
  for(int i = 0 ; i < angle ; i++)
  {
        temp = i % 4;
        digitalWrite(pinArray[0], bitRead(CCW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CCW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CCW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CCW[temp], 3));
        stepDelay();
  }

  delay(1000);
}

