/*
 * polytfit 모듈 코드
 * version 0.0.2
*/

#define BeepPin 12 // 부저

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

int itr0, itr1;

void beep(int time)
{
  digitalWrite(BeepPin, HIGH);
  delay(time);
  digitalWrite(BeepPin, LOW);
}

// 모터 48byj 스템모터 딜레이
void stepDelay()
{
  // 모터 부하가 없는 상태에서, 
  // 각 스텝 간격이 최소 3ms 이상이어야 정상적으로 동작합니다.
  delay(3);
}



// 초기화
void setup(){
  // 전원 입력시 삑 (1번)
  pinMode(BeepPin, OUTPUT);
  beep(150); delay(150); 

  // 시리얼 초기화
    // ==========
  Serial.begin(115200);
  Serial.println("start");

  // 내부 LED 켜짐 (깜빡이지 x)
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  

  // 외부인터럽트 초기화
  // ==========
  pinMode(2, INPUT); // 인터럽트 0
  pinMode(3, INPUT); // 인터럽트 1


  // 상판 체크
  Serial.print("checking int0 =");
  itr0 = digitalRead(2);
  Serial.println( itr0 );

  Serial.print("checking int1 =");
  itr1 = digitalRead(3);
  Serial.println( itr1 );

  // 포토센서를 위한 인터럽트
  attachInterrupt( 
    digitalPinToInterrupt(2), 
    button1_rising, 
    RISING); // RISING : low -> high
  attachInterrupt( 
    digitalPinToInterrupt(2), 
    button1_falling, 
    FALLING); // FALLING : high -> low

  attachInterrupt( 
    digitalPinToInterrupt(3), 
    button2_rising, 
    RISING); // RISING : low -> high
  attachInterrupt( 
    digitalPinToInterrupt(3), 
    button2_falling, 
    FALLING); // FALLING : high -> low

  // 모터 초기화
  // ==========
  // 스텝모터 드라이브 보드의 IN 연결핀 출력으로 설정
  for(int i = 0 ; i < 4 ; i++ ) {
    pinMode(pinArray[i], OUTPUT);
  }

  // 초기화 동작 완료
  beep(100); delay(100); 
  beep(100);
}


void button1_falling()
{
  itr0 = digitalRead(2);
  Serial.print("int0 falling =");
  Serial.println( itr0 );
}

void button2_falling()
{
  itr1 = digitalRead(3);
  Serial.print("int1 falling =");
  Serial.println( itr1 );
}


int led_status = LOW;
bool led_status_changed = false;
void button1_rising()
{
  Serial.print("int0 rising =");
  mortor_enable1 = 0;
  Serial.print("모터 정지1=");
  Serial.println(mortor_enable1);

  angle = 0; // 계산된 펄스값, 즉시 초기화 => 멈춤
  digitalWrite(13, LOW);

  mortor_enable2 = 1;


}

void button2_rising()
{
  Serial.print("int1 rising =");
  mortor_enable2 = 0;
  Serial.print("모터 정지2=");
  Serial.println(mortor_enable2);

  angle = 0; // 계산된 펄스값, 즉시 초기화 => 멈춤
  digitalWrite(13, LOW);

  mortor_enable1 = 1;

}

int distance = 0;

void loop() {  

  itr0 = digitalRead(2); // 인터럽트0
  itr1 = digitalRead(3); // 인터럽트1

  if(itr0 == 0 && itr1 == 0) {
    // 상판이 연결되어있지 않음
    Serial.println("error:: photo senser open");
    // 센서오류
    beep(200); 
    delay(200); 
    beep(200);

  } else {
    // 정상동작
    cw();
    // motor_cw(0);
    // motor_ccw(0);
  }
  
  
}

int cw() {
  int temp;
  for(int i = 0 ; i < 2048 ; i++) {
    temp = i % 4;
    digitalWrite(pinArray[0], bitRead(CW[temp], 0));
    digitalWrite(pinArray[1], bitRead(CW[temp], 1));
    digitalWrite(pinArray[2], bitRead(CW[temp], 2));
    digitalWrite(pinArray[3], bitRead(CW[temp], 3));
    stepDelay();
  }

       
}

// 모터 정방향
int motor_cw(int deg) 
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
    //Serial.println(angle);
    // 센서오류, 동작 탈출
    if(itr0 == 0 && itr1 == 0) {
      Serial.println("error:: open");
      return 0;
    }

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

int motor_ccw(int deg)
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
    //Serial.println(angle);
    // 센서오류, 동작 탈출
    if(itr0 == 0 && itr1 == 0) {
      return 0;
    }
    
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
