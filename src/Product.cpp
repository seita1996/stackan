#include "Product.h"

static Product PRODUCTS[] = {
  {"0", "りんご", "APPLE", 100, TFT_RED, 'A'},
  {"1", "バナナ", "BANANA", 150, TFT_YELLOW, 'B'},
  {"2", "ぎゅうにゅう", "MILK", 200, TFT_WHITE, 'M'},
  {"3", "パン", "BREAD", 120, 0xF5B6, 'P'},
  {"4", "たまご", "EGG", 180, 0xFFE0, 'E'},
  {"5", "にんじん", "CARROT", 90, 0xFC60, 'C'},
  {"6", "おにぎり", "RICE", 130, 0xFFFF, 'R'},
  {"7", "ジュース", "JUICE", 160, 0xF81F, 'J'},
  {"8", "おかし", "SNACK", 80, 0x07FF, 'S'},
  {"9", "アイス", "ICE", 140, 0xAEDC, 'I'},
};

static const int PRODUCT_COUNT = sizeof(PRODUCTS) / sizeof(PRODUCTS[0]);

ProductDB::ProductDB() {}

void ProductDB::init() {
  Serial.println("Product catalog ready");
}

const Product* ProductDB::findProductById(const String& id) const {
  if (id.length() == 0) {
    return nullptr;
  }

  for (int i = 0; i < PRODUCT_COUNT; i++) {
    if (PRODUCTS[i].id == id) {
      return &PRODUCTS[i];
    }
  }

  Serial.println("Product not found with ID: " + id);
  return nullptr;
}

Product* ProductDB::findProductById(const String& id) {
  return const_cast<Product*>(static_cast<const ProductDB*>(this)->findProductById(id));
}

int ProductDB::getProductCount() const {
  return PRODUCT_COUNT;
}

const Product* ProductDB::getProductAt(int index) const {
  if (index < 0 || index >= PRODUCT_COUNT) {
    return nullptr;
  }

  return &PRODUCTS[index];
}

Product* ProductDB::getProductAt(int index) {
  return const_cast<Product*>(static_cast<const ProductDB*>(this)->getProductAt(index));
}
