#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include "Product.h"
#include <vector>

// 買い物かごのアイテム
struct CartItem {
  Product* product;
  int quantity;
};

// 買い物かごクラス
class ShoppingCart {
private:
  std::vector<CartItem> items;
  int totalAmount;

public:
  ShoppingCart();
  
  // 商品をカートに追加
  void addProduct(Product* product, int quantity = 1);
  
  // 商品をカートから削除
  bool removeProduct(String productId);
  
  // カートをクリア
  void clear();
  
  // 合計金額を計算
  int calculateTotal();
  
  // アイテム数を取得
  int getItemCount() { return items.size(); }
  
  // 特定位置のアイテムを取得
  CartItem* getItemAt(int index);
  
  // 合計金額を取得
  int getTotalAmount() { return totalAmount; }
};

#endif // SHOPPING_CART_H
