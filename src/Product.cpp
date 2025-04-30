#include "Product.h"
#include "Images.h"

// コンストラクタ
ProductDB::ProductDB() {
  productCount = 0;
  products = nullptr;
}

// デストラクタ
ProductDB::~ProductDB() {
  if (products != nullptr) {
    delete[] products;
  }
}

// 商品データの初期化
void ProductDB::init() {
  // 既存のデータがあれば解放
  if (products != nullptr) {
    delete[] products;
  }
  
  // 新しい商品データを作成（商品数は3つ）
  productCount = 3;
  products = new Product[productCount];
  
  // 商品データの設定
  products[0] = {"0", "りんご", 100, APPLE_IMG, 32, 32};
  products[1] = {"1", "バナナ", 150, BANANA_IMG, 32, 32};
  products[2] = {"2", "ぎゅうにゅう", 200, MILK_IMG, 32, 32};
  
  // 必要に応じて商品を追加
}

// 商品をIDで検索する関数
Product* ProductDB::findProductById(String id) {
  for (int i = 0; i < productCount; i++) {
    if (products[i].id == id) {
      return &products[i];
    }
  }
  return nullptr; // 該当する商品が見つからない場合
}

// インデックスで商品を取得
Product* ProductDB::getProductAt(int index) {
  if (index >= 0 && index < productCount) {
    return &products[index];
  }
  return nullptr;
}
