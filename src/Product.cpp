#include "Product.h"

static Product PRODUCTS[] = {
  {"0", "りんご", 100, TFT_RED, 'A'},
  {"1", "バナナ", 150, TFT_YELLOW, 'B'},
  {"2", "ぎゅうにゅう", 200, TFT_WHITE, 'M'},
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
