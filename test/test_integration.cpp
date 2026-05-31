#include <unity.h>
#include "test_helpers.h"

// テスト対象の共有インスタンス
extern ProductDB productDB;
extern QrScanner qrScanner;
extern ShoppingCart cart;

// QRコードスキャンから商品表示までの統合テスト
void test_integration_scan_to_display() {
  String qrData;
  
  // QRコードスキャンのシミュレーション
  mockSerial2.simulateScan("product:1");
  bool scanResult = qrScanner.readQrCode(qrData);
  
  // スキャン成功を確認
  TEST_ASSERT_TRUE(scanResult);
  TEST_ASSERT_EQUAL_STRING("product:1", qrData.c_str());
  
  // スキャンデータから商品を検索
  Product* product = productDB.findProductById(qrData.substring(8));
  
  // 商品が見つかることを確認
  TEST_ASSERT_NOT_NULL(product);
  TEST_ASSERT_EQUAL_STRING("バナナ", product->name.c_str());
  TEST_ASSERT_EQUAL(150, product->price);
}

// 商品をカートに追加する統合テスト
void test_integration_add_to_cart() {
  String qrData;
  
  // 商品1のスキャン
  mockSerial2.simulateScan("product:0");
  qrScanner.readQrCode(qrData);
  Product* product1 = productDB.findProductById(qrData.substring(8));
  
  // カートに追加
  cart.addProduct(product1);
  
  // 商品2のスキャン
  mockSerial2.simulateScan("product:2");
  qrScanner.readQrCode(qrData);
  Product* product2 = productDB.findProductById(qrData.substring(8));
  
  // カートに追加
  cart.addProduct(product2);
  
  // カートの状態を確認
  TEST_ASSERT_EQUAL(2, cart.getItemCount());
  TEST_ASSERT_EQUAL(300, cart.getTotalAmount()); // 100 + 200
  
  // 商品の詳細を確認
  CartItem* item1 = cart.getItemAt(0);
  TEST_ASSERT_EQUAL_STRING("りんご", item1->product->name.c_str());
  TEST_ASSERT_EQUAL(1, item1->quantity);
  
  CartItem* item2 = cart.getItemAt(1);
  TEST_ASSERT_EQUAL_STRING("ぎゅうにゅう", item2->product->name.c_str());
  TEST_ASSERT_EQUAL(1, item2->quantity);
}

// 無効なQRコードのテスト
void test_integration_invalid_qr() {
  String qrData;
  
  // 無効なQRコードのシミュレーション
  mockSerial2.simulateScan("invalid_format");
  bool scanResult = qrScanner.readQrCode(qrData);
  
  // スキャン自体は成功
  TEST_ASSERT_TRUE(scanResult);
  TEST_ASSERT_EQUAL_STRING("invalid_format", qrData.c_str());
  
  // 商品フォーマットでないため、見つからない
  TEST_ASSERT_FALSE(qrData.startsWith("product:"));
  
  // 存在しない商品IDのテスト
  mockSerial2.simulateScan("product:999");
  scanResult = qrScanner.readQrCode(qrData);
  TEST_ASSERT_TRUE(scanResult);
  
  Product* product = productDB.findProductById("999");
  TEST_ASSERT_NULL(product);
}

// 完全な買い物プロセスの統合テスト
void test_integration_shopping_process() {
  String qrData;
  
  // ステップ1: 商品1のスキャン
  mockSerial2.simulateScan("product:0");
  qrScanner.readQrCode(qrData);
  Product* product1 = productDB.findProductById(qrData.substring(8));
  cart.addProduct(product1);
  
  // ステップ2: 商品2のスキャン（2個）
  mockSerial2.simulateScan("product:1");
  qrScanner.readQrCode(qrData);
  Product* product2 = productDB.findProductById(qrData.substring(8));
  cart.addProduct(product2, 2);
  
  // ステップ3: カートの状態確認
  TEST_ASSERT_EQUAL(2, cart.getItemCount());
  TEST_ASSERT_EQUAL(400, cart.getTotalAmount()); // 100 + 2*150
  
  // ステップ4: 商品1を削除
  bool result = cart.removeProduct("0");
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  TEST_ASSERT_EQUAL(300, cart.getTotalAmount()); // 2*150
  
  // ステップ5: カートをクリア
  cart.clear();
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  TEST_ASSERT_EQUAL(0, cart.getTotalAmount());
}

void test_integration_all_products_fit_in_cart() {
  int expectedTotal = 0;

  for (int i = 0; i < productDB.getProductCount(); i++) {
    Product* product = productDB.getProductAt(i);
    TEST_ASSERT_NOT_NULL(product);
    TEST_ASSERT_TRUE(cart.addProduct(product));
    expectedTotal += product->price;
  }

  TEST_ASSERT_EQUAL(12, cart.getItemCount());
  TEST_ASSERT_EQUAL(12, cart.getTotalQuantity());
  TEST_ASSERT_EQUAL(expectedTotal, cart.getTotalAmount());

  CartItem* item8 = cart.getItemAt(8);
  TEST_ASSERT_NOT_NULL(item8);
  TEST_ASSERT_EQUAL_STRING("8", item8->product->id.c_str());
  TEST_ASSERT_EQUAL_STRING("もも", item8->product->name.c_str());

  CartItem* item9 = cart.getItemAt(9);
  TEST_ASSERT_NOT_NULL(item9);
  TEST_ASSERT_EQUAL_STRING("9", item9->product->id.c_str());
  TEST_ASSERT_EQUAL_STRING("アイスクリーム", item9->product->name.c_str());

  CartItem* item11 = cart.getItemAt(11);
  TEST_ASSERT_NOT_NULL(item11);
  TEST_ASSERT_EQUAL_STRING("11", item11->product->id.c_str());
  TEST_ASSERT_EQUAL_STRING("鶏肉", item11->product->name.c_str());
}

void test_scan_deduplicator_blocks_same_qr_briefly() {
  ScanDeduplicator deduplicator(1500);

  TEST_ASSERT_TRUE(deduplicator.shouldAccept("product:0", 1000));
  TEST_ASSERT_FALSE(deduplicator.shouldAccept("product:0", 1200));
  TEST_ASSERT_TRUE(deduplicator.shouldAccept("product:1", 1300));
  TEST_ASSERT_TRUE(deduplicator.shouldAccept("product:1", 2900));
}
