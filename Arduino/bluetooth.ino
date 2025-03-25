#include <SoftwareSerial.h>           // 소프트웨어 시리얼 라이브러리 불러오기


SoftwareSerial BTSerial(7,8);        // 소프트웨어 시리얼 객체를 7(TX),8번(RX) 으로 생성


void setup()
{
  Serial.begin(9600);                // 시리얼 통신을 개시, 속도는 9600  
  BTSerial.begin(9600);              // 소프트웨어시리얼 통신 개시, 속도는 9600
}




void loop(){
  if(BTSerial.available())               // 블루투스 시리얼 통신으로 문자가 들어오면
  { 
    int c =BTSerial.read();       // 시리얼 창에 표시(시리얼 통신으로 출력)
    Serial.write(c);
  }
    
}
