#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include "Product.h"

const int MAX_CART_ITEMS = 12;

struct CartItem {
  const Product* product;
  int quantity;
};

class ShoppingCart {
private:
  CartItem items[MAX_CART_ITEMS];
  int itemCount;

public:
  ShoppingCart();

  bool addProduct(const Product* product, int quantity = 1);
  bool removeProduct(String productId);
  void clear();
  int calculateTotal();
  int getItemCount() const { return itemCount; }
  int getTotalQuantity() const;
  CartItem* getItemAt(int index);
  const CartItem* getItemAt(int index) const;
  int getTotalAmount() const;
};

#endif // SHOPPING_CART_H
