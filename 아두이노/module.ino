/*
 * polytfit 모듈 코드
 * version 0.0.1
*/

// 모터 연결핀
#define motorPin1 7 // IN1
#define motorPin2 6 // IN2
#define motorPin3 5 // IN3
#define motorPin4 4 // IN4

// 모터 48byj 스템모터
#define step 2048 // 1바퀴 스텝수

// 모터 회전 Loop 문 구현을 간단하게 하기위해 핀 배열 선언
int pinArray[4] = { motorPin1, motorPin2, motorPin3, motorPin4 };

// CW 시계방향 데이터를 전송
int CW[4] = { 0b1000, 0b0100, 0b0010,0b0001 };
// CCW 반시계방향 데이터를 전송
int CCW[4] = { 0b0001, 0b0010, 0b0100, 0b1000 };

// 모터 활성화: 1=동작, 0=정지
int mortor_enable1 = 1;
int mortor_enable2 = 1;
int angle; // 모터가 돌아가야 되는 계산돤 펄스

// 초기화
void setup(){
  // 내부 LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // 외부인터럽트 0
  pinMode(2, INPUT);

  // 스텝모터 드라이브 보드의 IN 연결핀 출력으로 설정
  for(int i = 0 ; i < 4 ; i++ ) {
    pinMode(pinArray[i], OUTPUT);
  }

  // 포토센서를 위한 인터럽트
  attachInterrupt( 
    digitalPinToInterrupt(2), 
    buttonPress1, 
    FALLING);

  attachInterrupt( 
    digitalPinToInterrupt(3), 
    buttonPress2, 
    FALLING);

  Serial.begin(115200);
  Serial.println("start");


}

// 모터 48byj 스템모터 딜레이
void stepDelay()
{
  // 모터 부하가 없는 상태에서, 
  // 각 스텝 간격이 최소 3ms 이상이어야 정상적으로 동작합니다.
  delay(3);
}

int led_status = LOW;
bool led_status_changed = false;
void buttonPress1()
{
  mortor_enable1 = 0;
  Serial.print("모터 정지1=");
  Serial.println(mortor_enable1);

  angle = 0; // 계산된 펄스값, 즉시 초기화 => 멈춤
  digitalWrite(13, LOW);

  mortor_enable2 = 1;

}

void buttonPress2()
{
  mortor_enable2 = 0;
  Serial.print("모터 정지2=");
  Serial.println(mortor_enable2);

  angle = 0; // 계산된 펄스값, 즉시 초기화 => 멈춤
  digitalWrite(13, LOW);

  mortor_enable1 = 1;

}


void loop() {  

  motor_cw(0);
  motor_ccw(0);


  

  
}

// 모터 정방향
void motor_cw(int deg) 
{
  int temp;

  if(mortor_enable1 == 1) {
    // 입력한 각도에 따른 펄스수
    if(deg == 0) {
      angle = step;
    } else {
      // step = 2048
      angle = (step / 360) * deg;
    }
  } else {
    angle = 0;
  }

  // CW방향 1바퀴 후
  // step = 2048 한바퀴
  for(int i = 0 ; i < angle ; i++)
  {
    Serial.println(angle);

    if(mortor_enable1 == 1) {
      temp = i % 4;
      digitalWrite(pinArray[0], bitRead(CW[temp], 0));
      digitalWrite(pinArray[1], bitRead(CW[temp], 1));
      digitalWrite(pinArray[2], bitRead(CW[temp], 2));
      digitalWrite(pinArray[3], bitRead(CW[temp], 3));
      stepDelay();
    }
  }


}

void motor_ccw(int deg)
{
  int temp;
  

  if(mortor_enable2 == 1) {
    // 입력한 각도에 따른 펄스수
    if(deg == 0) {
      angle = step;
    } else {
      // step = 2048
      angle = (step / 360) * deg;
    }
  } else {
    angle = 0;
  }

  // CCW방향 1바퀴 후
  // step = 2048 한바퀴
  for(int i = 0 ; i < angle ; i++)
  {
    Serial.println(angle);
    
    if(mortor_enable2 == 1) {
        temp = i % 4;
        digitalWrite(pinArray[0], bitRead(CCW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CCW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CCW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CCW[temp], 3));
        stepDelay();
    }
  }


}

