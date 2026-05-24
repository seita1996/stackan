#ifndef PRODUCT_H
#define PRODUCT_H

#include <M5Stack.h>

struct Product {
  String id;
  String name;
  const char* label;
  int price;
  uint16_t color;
  char icon;
};

class ProductDB {
public:
  ProductDB();

  void init();
  const Product* findProductById(const String& id) const;
  Product* findProductById(const String& id);
  int getProductCount() const;
  const Product* getProductAt(int index) const;
  Product* getProductAt(int index);
};

#endif // PRODUCT_H
