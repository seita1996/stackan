#include <unity.h>
#include "test_helpers.h"

// テスト対象の共有インスタンス
ProductDB productDB;
QrScanner qrScanner(&mockSerial2, 9600);
ShoppingCart cart;

// 各テスト前の準備
void setUp(void) {
  productDB.init();
  cart.clear();
  qrScanner.init();
  mockSerial2.mockAvailable = false;
  mockSerial2.mockData = "";
}

void tearDown(void) {
  cart.clear();
}

// テスト関数のプロトタイプ
void test_product_init();
void test_product_find();
void test_product_edge_cases();
void test_qr_scanner_read();
void test_qr_scanner_multiple_scans();
void test_qr_scanner_timeout();
void test_qr_scanner_reset();
void test_cart_empty();
void test_cart_add_product();
void test_cart_remove_product();
void test_cart_clear();
void test_cart_multiple_quantities();
void test_cart_invalid_inputs();
void test_integration_scan_to_display();
void test_integration_add_to_cart();
void test_integration_invalid_qr();
void test_integration_shopping_process();
void test_integration_all_products_fit_in_cart();
void test_scan_deduplicator_blocks_same_qr_briefly();

void RUN_UNITY_TESTS() {
  UNITY_BEGIN();
  RUN_TEST(test_product_init);
  RUN_TEST(test_product_find);
  RUN_TEST(test_product_edge_cases);
  RUN_TEST(test_qr_scanner_read);
  RUN_TEST(test_qr_scanner_multiple_scans);
  RUN_TEST(test_qr_scanner_timeout);
  RUN_TEST(test_qr_scanner_reset);
  RUN_TEST(test_cart_empty);
  RUN_TEST(test_cart_add_product);
  RUN_TEST(test_cart_remove_product);
  RUN_TEST(test_cart_clear);
  RUN_TEST(test_cart_multiple_quantities);
  RUN_TEST(test_cart_invalid_inputs);
  RUN_TEST(test_integration_scan_to_display);
  RUN_TEST(test_integration_add_to_cart);
  RUN_TEST(test_integration_invalid_qr);
  RUN_TEST(test_integration_shopping_process);
  RUN_TEST(test_integration_all_products_fit_in_cart);
  RUN_TEST(test_scan_deduplicator_blocks_same_qr_briefly);
  UNITY_END();
}

// Arduino/PlatformIOフレームワークではsetupとloopが必要
void setup() {
  delay(2000);
  RUN_UNITY_TESTS();
}

void loop() {
  // 何もしない
}
