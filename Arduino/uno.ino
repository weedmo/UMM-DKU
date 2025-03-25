#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h> // 소프트웨어 시리얼 라이브러리 불러오기


SoftwareSerial BTSerial(7, 8); // 소프트웨어 시리얼 객체를 7(TX), 8번(RX)으로 생성


#define RST_PIN 9  // reset핀은 9번으로 설정
#define SS_PIN 10  // SS핀은 10번으로 설정
                   // SS핀은 데이터를 주고받는 역할의 핀(SS = Slave Selector)


MFRC522 mfrc(SS_PIN, RST_PIN); // MFR522를 이용하기 위해 mfrc 객체를 생성해 줍니다.


void setup() {
  Serial.begin(9600); // 시리얼 통신, 속도는 9600
  BTSerial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
}


void loop() {
  static bool print123 = false;
  static bool resetPrint123 = false; // RFID 태그 감지 후 123 출력을 재개하도록 하는 플래그


  // 블루투스로 123 수신 여부 확인
  if (BTSerial.available()) {
    String receivedData = BTSerial.readString();
    if (receivedData == "123") {
      print123 = true;
      resetPrint123 = false; // 블루투스로 123 수신 시, RFID 태그 감지 플래그 초기화
    }
  }


  // "123"을 출력하는 상태
  if (print123) {
    Serial.println(123); // 123을 시리얼로 출력
    delay(1000); // 1초 딜레이
  }


  // RFID 태그 읽기
  if (mfrc.PICC_IsNewCardPresent() && mfrc.PICC_ReadCardSerial()) {
    print123 = false; // RFID 태그가 감지되면 "123" 출력 상태 해제
    Serial.print("Card UID: ");
    bool found27 = false; // 27을 찾았는지 여부
    bool found16 = false; // 16을 찾았는지 여부


    // UID 값을 시리얼 모니터에 출력하고 특정 값을 찾으면 플래그 설정S
    for (byte i = 0; i < mfrc.uid.size; i++) {
      Serial.print(mfrc.uid.uidByte[i]);
      Serial.print(" ");
      
      // 특정 값을 찾으면 플래그 설정
      if (mfrc.uid.uidByte[i] == 27) {
        found27 = true;
      }
      if (mfrc.uid.uidByte[i] == 16) {
        found16 = true;
      }
    }
    Serial.println();


    // 27을 찾은 경우 블루투스로 데이터 전송
    if (found27) {
      BTSerial.write("토마토 상세정보\n"); // 블루투스로 전송
      Serial.println("토마토 상세정보"); // 시리얼 모니터에 출력
    }


    // 16을 찾은 경우 블루투스로 데이터 전송 및 시리얼 모니터에 메시지 출력
    if (found16) {
      BTSerial.write("바나나 상세정보\n"); // 블루투스로 전송
      Serial.println("바나나 상세정보"); // 시리얼 모니터에 메시지 출력
    }


    mfrc.PICC_HaltA(); // UID를 읽은 후 태그 정지
    resetPrint123 = true; // RFID 태그 감지 후 123 출력을 재개하도록 설정
  }

}
