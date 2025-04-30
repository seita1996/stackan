#include "QrScanner.h"

// コンストラクタ
QrScanner::QrScanner(int baud) {
  baudRate = baud;
  lastReadTime = 0;
}

// 初期化
void QrScanner::init() {
  Serial2.begin(baudRate);
  Serial2.flush();
  lastReadTime = millis();
}

// QRコード読み取り
bool QrScanner::readQrCode(String &data) {
  if (Serial2.available()) {
    data = Serial2.readStringUntil('\n');
    data.trim(); // 余分な空白や改行を削除
    
    if (data.length() > 0) {
      // デバッグ出力
      Serial.print("QRコード読み取り: ");
      Serial.println(data);
      
      lastReadTime = millis();
      return true;
    }
  }
  return false;
}

// タイムアウトチェック
bool QrScanner::isTimeout() {
  return (millis() - lastReadTime) > timeout;
}

// 状態リセット
void QrScanner::reset() {
  Serial2.flush();
  lastReadTime = millis();
}
