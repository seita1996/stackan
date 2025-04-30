#include "ShoppingCart.h"

// コンストラクタ
ShoppingCart::ShoppingCart() {
  totalAmount = 0;
}

// 商品をカートに追加
void ShoppingCart::addProduct(Product* product, int quantity) {
  if (product == nullptr || quantity <= 0) return;
  
  // 既に同じ商品がカートにある場合は数量を追加
  for (size_t i = 0; i < items.size(); i++) {
    if (items[i].product->id == product->id) {
      items[i].quantity += quantity;
      calculateTotal(); // 合計金額を再計算
      return;
    }
  }
  
  // 新しい商品をカートに追加
  CartItem newItem = {product, quantity};
  items.push_back(newItem);
  calculateTotal(); // 合計金額を再計算
}

// 商品をカートから削除
bool ShoppingCart::removeProduct(String productId) {
  for (size_t i = 0; i < items.size(); i++) {
    if (items[i].product->id == productId) {
      items.erase(items.begin() + i);
      calculateTotal(); // 合計金額を再計算
      return true;
    }
  }
  return false; // 該当する商品が見つからない場合
}

// カートをクリア
void ShoppingCart::clear() {
  items.clear();
  totalAmount = 0;
}

// 合計金額を計算
int ShoppingCart::calculateTotal() {
  totalAmount = 0;
  for (size_t i = 0; i < items.size(); i++) {
    totalAmount += items[i].product->price * items[i].quantity;
  }
  return totalAmount;
}

// 特定位置のアイテムを取得
CartItem* ShoppingCart::getItemAt(int index) {
  if (index >= 0 && index < static_cast<int>(items.size())) {
    return &items[index];
  }
  return nullptr;
}
