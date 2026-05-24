#include <unity.h>
#include "test_helpers.h"

// テスト対象の共有インスタンス
extern QrScanner qrScanner;

// QRコード読み取りの基本テスト
void test_qr_scanner_read() {
  String data;
  
  // データがない場合
  bool result = qrScanner.readQrCode(data);
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_STRING("", data.c_str());
  
  // QRコードデータのシミュレーション
  mockSerial2.simulateScan("product:1");
  
  // データを読み取る
  result = qrScanner.readQrCode(data);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_STRING("product:1", data.c_str());
  
  // データを読み取った後はavailableがfalseになる
  result = qrScanner.readQrCode(data);
  TEST_ASSERT_FALSE(result);
}

// 複数回のスキャンテスト
void test_qr_scanner_multiple_scans() {
  String data;
  
  // 1回目のスキャン
  mockSerial2.simulateScan("product:0");
  bool result = qrScanner.readQrCode(data);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_STRING("product:0", data.c_str());
  
  // 2回目のスキャン
  mockSerial2.simulateScan("product:1");
  result = qrScanner.readQrCode(data);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_STRING("product:1", data.c_str());
  
  // 3回目のスキャン
  mockSerial2.simulateScan("product:2");
  result = qrScanner.readQrCode(data);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_STRING("product:2", data.c_str());
}

// タイムアウトのテスト
void test_qr_scanner_timeout() {
  // 初期状態ではタイムアウトしていない
  TEST_ASSERT_FALSE(qrScanner.isTimeout());
  
  // タイムアウト状態を確認（実際には時間を進めるのが難しいのでテストは限定的）
  // 実際の環境でのタイムアウトテストは、実機テストで行う必要があります
}

// リセット機能のテスト
void test_qr_scanner_reset() {
  String data;
  
  // QRコードデータのシミュレーション
  mockSerial2.simulateScan("product:1");
  
  // データを読み取る
  bool result = qrScanner.readQrCode(data);
  TEST_ASSERT_TRUE(result);
  
  // スキャナをリセット
  qrScanner.reset();
  
  // リセット後は利用可能なデータがない
  result = qrScanner.readQrCode(data);
  TEST_ASSERT_FALSE(result);
}
