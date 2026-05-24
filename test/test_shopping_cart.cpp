#include <unity.h>
#include "test_helpers.h"

// テスト対象の共有インスタンス
extern ShoppingCart cart;
extern ProductDB productDB;

// テスト用商品データ
Product testProduct1;
Product testProduct2;

static void initTestProducts() {
  testProduct1 = {"test1", "テスト商品1", 100, TFT_RED, '1'};
  testProduct2 = {"test2", "テスト商品2", 200, TFT_YELLOW, '2'};
}

// 空のカートのテスト
void test_cart_empty() {
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  TEST_ASSERT_EQUAL(0, cart.getTotalAmount());
  TEST_ASSERT_NULL(cart.getItemAt(0));
}

// 商品追加のテスト
void test_cart_add_product() {
  initTestProducts();
  // 商品を追加
  cart.addProduct(&testProduct1);
  
  // カートのアイテム数と合計金額を確認
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  TEST_ASSERT_EQUAL(1, cart.getTotalQuantity());
  TEST_ASSERT_EQUAL(100, cart.getTotalAmount());
  
  // 追加した商品の情報を確認
  CartItem* item = cart.getItemAt(0);
  TEST_ASSERT_NOT_NULL(item);
  TEST_ASSERT_EQUAL_STRING("テスト商品1", item->product->name.c_str());
  TEST_ASSERT_EQUAL(1, item->quantity);
  
  // 同じ商品をもう一度追加（数量が増えるはず）
  cart.addProduct(&testProduct1);
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  TEST_ASSERT_EQUAL(2, cart.getTotalQuantity());
  TEST_ASSERT_EQUAL(200, cart.getTotalAmount());
  item = cart.getItemAt(0);
  TEST_ASSERT_EQUAL(2, item->quantity);
  
  // 別の商品を追加
  cart.addProduct(&testProduct2);
  TEST_ASSERT_EQUAL(2, cart.getItemCount());
  TEST_ASSERT_EQUAL(3, cart.getTotalQuantity());
  TEST_ASSERT_EQUAL(400, cart.getTotalAmount());
}

// 商品削除のテスト
void test_cart_remove_product() {
  initTestProducts();
  // 商品を追加
  cart.addProduct(&testProduct1);
  cart.addProduct(&testProduct2);
  TEST_ASSERT_EQUAL(2, cart.getItemCount());
  
  // 存在する商品を削除
  bool result = cart.removeProduct("test1");
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  TEST_ASSERT_EQUAL(200, cart.getTotalAmount());
  
  // 存在しない商品を削除
  result = cart.removeProduct("non-existent");
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  
  // 残りの商品を削除
  result = cart.removeProduct("test2");
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  TEST_ASSERT_EQUAL(0, cart.getTotalAmount());
  TEST_ASSERT_EQUAL(0, cart.getTotalQuantity());
}

// カートのクリアのテスト
void test_cart_clear() {
  initTestProducts();
  // 商品を追加
  cart.addProduct(&testProduct1);
  cart.addProduct(&testProduct2);
  TEST_ASSERT_EQUAL(2, cart.getItemCount());
  
  // カートをクリア
  cart.clear();
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  TEST_ASSERT_EQUAL(0, cart.getTotalAmount());
}

// 複数数量の商品を扱うテスト
void test_cart_multiple_quantities() {
  initTestProducts();
  // 数量を指定して商品を追加
  cart.addProduct(&testProduct1, 3);
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  TEST_ASSERT_EQUAL(300, cart.getTotalAmount());
  
  CartItem* item = cart.getItemAt(0);
  TEST_ASSERT_EQUAL(3, item->quantity);
  
  // さらに数量を追加
  cart.addProduct(&testProduct1, 2);
  TEST_ASSERT_EQUAL(1, cart.getItemCount());
  TEST_ASSERT_EQUAL(500, cart.getTotalAmount());
  item = cart.getItemAt(0);
  TEST_ASSERT_EQUAL(5, item->quantity);
}

// 無効な入力のテスト
void test_cart_invalid_inputs() {
  initTestProducts();
  // nullポインタを追加
  cart.addProduct(nullptr);
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  
  // 無効な数量を指定
  cart.addProduct(&testProduct1, 0);
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  
  cart.addProduct(&testProduct1, -1);
  TEST_ASSERT_EQUAL(0, cart.getItemCount());
  
  // 無効なインデックスでのgetItemAt
  TEST_ASSERT_NULL(cart.getItemAt(-1));
  TEST_ASSERT_NULL(cart.getItemAt(999));
}
