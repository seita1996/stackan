#ifndef PRODUCT_H
#define PRODUCT_H

#include <M5Stack.h>
#include <M5GFX.h>

// 商品データ構造体
struct Product {
  String id;          // 商品ID
  String name;        // 商品名
  int price;          // 価格
  const uint16_t* image; // イラスト（配列へのポインタ）
  uint16_t width;     // イラスト幅
  uint16_t height;    // イラスト高さ
};

// 商品データベースクラス
class ProductDB {
private:
  Product* products;
  int productCount;

public:
  ProductDB();
  ~ProductDB();
  
  // 商品データの初期化
  void init();
  
  // 商品を検索する関数
  Product* findProductById(String id);
  
  // 商品数を取得
  int getProductCount() { return productCount; }
  
  // 商品データを取得
  Product* getProductAt(int index);
};

#endif // PRODUCT_H
