#ifndef UI_H
#define UI_H

#include <M5Stack.h>
#include "Product.h"
#include "ShoppingCart.h"

class UI {
private:
  M5Display* display;

  void beginFrame(uint16_t background);
  void drawFooter(const char* left, const char* center, const char* right);
  void drawProductArt(const Product* product, int x, int y, int size);
  void drawTotals(const ShoppingCart* cart, uint16_t background);
  void drawText(int x, int y, const char* text, uint8_t size, uint16_t color, uint16_t background);

public:
  explicit UI(M5Display* disp);

  void init();
  void showHome(const ShoppingCart* cart);
  void showItemAdded(const Product* product, const ShoppingCart* cart);
  void showCart(const ShoppingCart* cart, int page);
  void showCheckout(const ShoppingCart* cart);
  void showMessage(const char* title, const char* message, const ShoppingCart* cart);
  void playSuccessSound();
  void playErrorSound();
};

#endif // UI_H
