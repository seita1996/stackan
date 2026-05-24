#include "UI.h"

static const uint16_t COLOR_BG = 0x4E7F;
static const uint16_t COLOR_PANEL = TFT_WHITE;
static const uint16_t COLOR_TEXT = 0x0841;
static const uint16_t COLOR_ACCENT = 0xFD20;
static const int ITEMS_PER_PAGE = 3;

UI::UI(M5Display* disp) : display(disp) {}

void UI::init() {
  display->begin();
  display->setRotation(1);
  display->setBrightness(180);
  display->setTextDatum(TL_DATUM);
  display->setTextWrap(false);
}

void UI::beginFrame(uint16_t background) {
  display->fillScreen(background);
  display->setTextDatum(TL_DATUM);
  display->setTextWrap(false);
}

void UI::drawText(int x, int y, const char* text, uint8_t size, uint16_t color, uint16_t background) {
  display->setTextSize(size);
  display->setTextColor(color, background);
  display->setCursor(x, y);
  display->print(text);
}

void UI::drawFooter(const char* left, const char* center, const char* right) {
  display->fillRect(0, 210, 320, 30, COLOR_TEXT);
  drawText(8, 219, left, 1, TFT_WHITE, COLOR_TEXT);
  drawText(117, 219, center, 1, TFT_WHITE, COLOR_TEXT);
  drawText(230, 219, right, 1, TFT_WHITE, COLOR_TEXT);
}

void UI::drawProductArt(const Product* product, int x, int y, int size) {
  if (product == nullptr) {
    return;
  }

  if (product->id == "0") {
    display->fillCircle(x + size / 2, y + size / 2 + 6, size / 2 - 6, product->color);
    display->fillRoundRect(x + size / 2 - 5, y + 4, 10, 22, 4, 0x7BE0);
    display->fillCircle(x + size / 2 + 20, y + 16, 12, TFT_GREEN);
  } else if (product->id == "1") {
    display->fillCircle(x + 36, y + 68, 22, product->color);
    display->fillCircle(x + 60, y + 52, 24, product->color);
    display->fillCircle(x + 84, y + 34, 20, product->color);
    display->fillCircle(x + 24, y + 78, 7, 0x8200);
    display->fillCircle(x + 98, y + 24, 7, 0x8200);
  } else {
    display->fillRoundRect(x + 20, y + 18, size - 40, size - 24, 8, TFT_WHITE);
    display->drawRoundRect(x + 20, y + 18, size - 40, size - 24, 8, TFT_BLUE);
    display->fillRect(x + 32, y + 8, size - 64, 18, TFT_BLUE);
    drawText(x + 44, y + 64, "MILK", 2, TFT_BLUE, TFT_WHITE);
  }
}

void UI::drawTotals(const ShoppingCart* cart, uint16_t background) {
  int quantity = cart == nullptr ? 0 : cart->getTotalQuantity();
  int total = cart == nullptr ? 0 : cart->getTotalAmount();

  display->setTextSize(2);
  display->setTextColor(COLOR_TEXT, background);
  display->setCursor(14, 14);
  display->printf("%d pcs", quantity);
  display->setCursor(212, 14);
  display->printf("%d yen", total);
}

void UI::showHome(const ShoppingCart* cart) {
  beginFrame(COLOR_BG);
  drawTotals(cart, COLOR_BG);

  display->fillRoundRect(18, 52, 284, 142, 8, COLOR_PANEL);
  drawText(50, 76, "SCAN ITEM", 3, COLOR_TEXT, COLOR_PANEL);
  drawText(48, 126, "Show QR card", 2, COLOR_TEXT, COLOR_PANEL);
  display->fillRoundRect(212, 112, 54, 54, 8, COLOR_ACCENT);
  display->fillRect(224, 124, 30, 30, COLOR_TEXT);
  display->fillRect(229, 129, 20, 20, COLOR_PANEL);

  drawFooter("A:RESET", "B:CART", "C:PAY");
}

void UI::showItemAdded(const Product* product, const ShoppingCart* cart) {
  beginFrame(COLOR_BG);
  drawTotals(cart, COLOR_BG);

  display->fillRoundRect(12, 44, 296, 160, 8, COLOR_PANEL);
  drawProductArt(product, 22, 62, 104);

  const char* label = product->id == "0" ? "APPLE" : (product->id == "1" ? "BANANA" : "MILK");
  drawText(140, 62, label, 3, COLOR_TEXT, COLOR_PANEL);

  display->setTextSize(4);
  display->setTextColor(COLOR_TEXT, COLOR_PANEL);
  display->setCursor(140, 104);
  display->printf("%d", product->price);
  drawText(242, 126, "yen", 2, COLOR_TEXT, COLOR_PANEL);

  int quantity = 1;
  for (int i = 0; cart != nullptr && i < cart->getItemCount(); i++) {
    const CartItem* item = cart->getItemAt(i);
    if (item != nullptr && item->product != nullptr && item->product->id == product->id) {
      quantity = item->quantity;
      break;
    }
  }
  display->setTextSize(2);
  display->setCursor(142, 170);
  display->printf("in cart: %d", quantity);

  drawFooter("A:RESET", "B:CART", "C:PAY");
  playSuccessSound();
}

void UI::showCart(const ShoppingCart* cart, int page) {
  beginFrame(COLOR_BG);
  drawTotals(cart, COLOR_BG);

  display->fillRoundRect(10, 44, 300, 160, 8, COLOR_PANEL);
  drawText(22, 58, "CART", 3, COLOR_TEXT, COLOR_PANEL);

  int itemCount = cart == nullptr ? 0 : cart->getItemCount();
  if (itemCount == 0) {
    drawText(74, 118, "EMPTY", 3, COLOR_TEXT, COLOR_PANEL);
  } else {
    int totalPages = (itemCount + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    if (page < 0) {
      page = 0;
    }
    if (page >= totalPages) {
      page = totalPages - 1;
    }

    display->setTextSize(2);
    display->setTextColor(COLOR_TEXT, COLOR_PANEL);
    for (int row = 0; row < ITEMS_PER_PAGE; row++) {
      int index = page * ITEMS_PER_PAGE + row;
      const CartItem* item = cart->getItemAt(index);
      if (item == nullptr || item->product == nullptr) {
        continue;
      }

      const char* label = item->product->id == "0" ? "APPLE" : (item->product->id == "1" ? "BANANA" : "MILK");
      int y = 96 + row * 32;
      display->fillCircle(30, y + 10, 9, item->product->color);
      display->setCursor(48, y);
      display->printf("%s x%d", label, item->quantity);
      display->setCursor(216, y);
      display->printf("%d", item->product->price * item->quantity);
    }

    display->setTextSize(1);
    display->setCursor(270, 188);
    display->printf("%d/%d", page + 1, totalPages);
  }

  drawFooter("A:RESET", "B:NEXT", "C:PAY");
}

void UI::showCheckout(const ShoppingCart* cart) {
  beginFrame(0x07F3);

  display->fillRoundRect(14, 26, 292, 178, 8, COLOR_PANEL);
  drawText(54, 48, "CHECKOUT", 3, COLOR_TEXT, COLOR_PANEL);
  display->setTextSize(5);
  display->setTextColor(COLOR_TEXT, COLOR_PANEL);
  display->setCursor(44, 96);
  display->printf("%d", cart == nullptr ? 0 : cart->getTotalAmount());
  drawText(226, 138, "yen", 2, COLOR_TEXT, COLOR_PANEL);
  drawText(82, 174, "THANK YOU", 2, COLOR_TEXT, COLOR_PANEL);

  drawFooter("A:RESET", "B:CART", "C:NEXT");
  playSuccessSound();
}

void UI::showMessage(const char* title, const char* message, const ShoppingCart* cart) {
  beginFrame(COLOR_BG);
  drawTotals(cart, COLOR_BG);

  display->fillRoundRect(20, 58, 280, 130, 8, COLOR_PANEL);
  drawText(48, 82, title, 3, COLOR_TEXT, COLOR_PANEL);
  drawText(46, 132, message, 2, COLOR_TEXT, COLOR_PANEL);

  drawFooter("A:RESET", "B:CART", "C:PAY");
  playErrorSound();
}

void UI::playSuccessSound() {
  M5.Speaker.tone(1200, 70);
  delay(80);
  M5.Speaker.tone(1600, 90);
  delay(100);
  M5.Speaker.mute();
}

void UI::playErrorSound() {
  M5.Speaker.tone(440, 90);
  delay(110);
  M5.Speaker.mute();
}
