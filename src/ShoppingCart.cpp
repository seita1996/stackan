#include "ShoppingCart.h"

ShoppingCart::ShoppingCart() {
  clear();
}

bool ShoppingCart::addProduct(const Product* product, int quantity) {
  if (product == nullptr || quantity <= 0) {
    Serial.println("Warning: Invalid product or quantity in addProduct");
    return false;
  }

  for (int i = 0; i < itemCount; i++) {
    if (items[i].product != nullptr && items[i].product->id == product->id) {
      items[i].quantity += quantity;
      return true;
    }
  }

  if (itemCount >= MAX_CART_ITEMS) {
    Serial.println("Warning: Cart is full");
    return false;
  }

  items[itemCount] = {product, quantity};
  itemCount++;
  return true;
}

bool ShoppingCart::removeProduct(String productId) {
  if (productId.length() == 0) {
    return false;
  }

  for (int i = 0; i < itemCount; i++) {
    if (items[i].product != nullptr && items[i].product->id == productId) {
      for (int j = i; j < itemCount - 1; j++) {
        items[j] = items[j + 1];
      }
      itemCount--;
      items[itemCount] = {nullptr, 0};
      return true;
    }
  }

  return false;
}

void ShoppingCart::clear() {
  itemCount = 0;
  for (int i = 0; i < MAX_CART_ITEMS; i++) {
    items[i] = {nullptr, 0};
  }
}

int ShoppingCart::calculateTotal() {
  return getTotalAmount();
}

int ShoppingCart::getTotalQuantity() const {
  int total = 0;
  for (int i = 0; i < itemCount; i++) {
    if (items[i].product != nullptr) {
      total += items[i].quantity;
    }
  }
  return total;
}

CartItem* ShoppingCart::getItemAt(int index) {
  return const_cast<CartItem*>(static_cast<const ShoppingCart*>(this)->getItemAt(index));
}

const CartItem* ShoppingCart::getItemAt(int index) const {
  if (index >= 0 && index < itemCount) {
    return &items[index];
  }
  return nullptr;
}

int ShoppingCart::getTotalAmount() const {
  int total = 0;
  for (int i = 0; i < itemCount; i++) {
    if (items[i].product != nullptr) {
      total += items[i].product->price * items[i].quantity;
    }
  }
  return total;
}
