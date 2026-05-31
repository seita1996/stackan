#include <unity.h>
#include "test_helpers.h"

// テスト対象の共有インスタンス
extern ProductDB productDB;

// 商品データ初期化のテスト
void test_product_init() {
  // 初期化後に商品数が正しいことを確認
  TEST_ASSERT_EQUAL(12, productDB.getProductCount());
  
  // 各商品のデータが正しく設定されていることを確認
  Product* p0 = productDB.getProductAt(0);
  TEST_ASSERT_NOT_NULL(p0);
  TEST_ASSERT_EQUAL_STRING("0", p0->id.c_str());
  TEST_ASSERT_EQUAL_STRING("りんご", p0->name.c_str());
  TEST_ASSERT_EQUAL_STRING("APPLE", p0->label);
  TEST_ASSERT_EQUAL(100, p0->price);
  TEST_ASSERT_EQUAL(TFT_RED, p0->color);
  TEST_ASSERT_EQUAL('A', p0->icon);
  
  Product* p1 = productDB.getProductAt(1);
  TEST_ASSERT_NOT_NULL(p1);
  TEST_ASSERT_EQUAL_STRING("1", p1->id.c_str());
  TEST_ASSERT_EQUAL_STRING("バナナ", p1->name.c_str());
  TEST_ASSERT_EQUAL_STRING("BANANA", p1->label);
  TEST_ASSERT_EQUAL(150, p1->price);
  TEST_ASSERT_EQUAL(TFT_YELLOW, p1->color);
  
  Product* p2 = productDB.getProductAt(2);
  TEST_ASSERT_NOT_NULL(p2);
  TEST_ASSERT_EQUAL_STRING("2", p2->id.c_str());
  TEST_ASSERT_EQUAL_STRING("ぎゅうにゅう", p2->name.c_str());
  TEST_ASSERT_EQUAL_STRING("MILK", p2->label);
  TEST_ASSERT_EQUAL(200, p2->price);
  TEST_ASSERT_EQUAL(TFT_WHITE, p2->color);

  Product* p9 = productDB.getProductAt(9);
  TEST_ASSERT_NOT_NULL(p9);
  TEST_ASSERT_EQUAL_STRING("9", p9->id.c_str());
  TEST_ASSERT_EQUAL_STRING("アイスクリーム", p9->name.c_str());
  TEST_ASSERT_EQUAL_STRING("ICE CREAM", p9->label);
  TEST_ASSERT_EQUAL(140, p9->price);

  Product* p11 = productDB.getProductAt(11);
  TEST_ASSERT_NOT_NULL(p11);
  TEST_ASSERT_EQUAL_STRING("11", p11->id.c_str());
  TEST_ASSERT_EQUAL_STRING("鶏肉", p11->name.c_str());
  TEST_ASSERT_EQUAL_STRING("CHICKEN", p11->label);
  TEST_ASSERT_EQUAL(260, p11->price);
}

// 商品検索のテスト
void test_product_find() {
  // 存在する商品IDで検索
  Product* product = productDB.findProductById("1");
  TEST_ASSERT_NOT_NULL(product);
  TEST_ASSERT_EQUAL_STRING("バナナ", product->name.c_str());

  product = productDB.findProductById("9");
  TEST_ASSERT_NOT_NULL(product);
  TEST_ASSERT_EQUAL_STRING("アイスクリーム", product->name.c_str());

  product = productDB.findProductById("10");
  TEST_ASSERT_NOT_NULL(product);
  TEST_ASSERT_EQUAL_STRING("さかな", product->name.c_str());
  
  // 存在しない商品IDで検索
  product = productDB.findProductById("999");
  TEST_ASSERT_NULL(product);
  
  // 無効な商品IDで検索
  product = productDB.findProductById("");
  TEST_ASSERT_NULL(product);
}

// 境界値テスト
void test_product_edge_cases() {
  // 範囲外のインデックスでのgetProductAt
  Product* product = productDB.getProductAt(-1);
  TEST_ASSERT_NULL(product);
  
  product = productDB.getProductAt(999);
  TEST_ASSERT_NULL(product);
}
