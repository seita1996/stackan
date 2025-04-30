#ifndef QR_SCANNER_H
#define QR_SCANNER_H

#include <M5Stack.h>

// QRコードスキャナクラス
class QrScanner {
private:
  int baudRate;
  unsigned long lastReadTime;
  const unsigned long timeout = 5000; // タイムアウト 5秒

public:
  QrScanner(int baud = 9600);
  
  // 初期化
  void init();
  
  // QRコード読み取り
  bool readQrCode(String &data);
  
  // タイムアウトチェック
  bool isTimeout();
  
  // 状態リセット
  void reset();
};

#endif // QR_SCANNER_H
