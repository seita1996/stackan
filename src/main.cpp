#include <M5Stack.h>
#include "Product.h"
#include "QrScanner.h"
#include "ScanDeduplicator.h"
#include "ShoppingCart.h"
#include "UI.h"

enum AppState {
  HOME,
  ITEM_ADDED,
  CART,
  CHECKOUT,
  MESSAGE
};

static const unsigned long SAME_SCAN_IGNORE_MS = 1500;
static const unsigned long MESSAGE_RETURN_MS = 2500;
static const unsigned long ITEM_RETURN_MS = 4000;
static const int CART_ITEMS_PER_PAGE = 3;

ProductDB productDB;
QrScanner qrScanner;
ShoppingCart cart;
UI ui(&M5.Lcd);

AppState appState = HOME;
unsigned long stateChangedAt = 0;
int cartPage = 0;
ScanDeduplicator scanDeduplicator(SAME_SCAN_IGNORE_MS);

static void setState(AppState nextState) {
  appState = nextState;
  stateChangedAt = millis();
}

static void showHome() {
  cartPage = 0;
  ui.showHome(&cart);
  setState(HOME);
}

static void showMessage(const char* title, const char* message) {
  ui.showMessage(title, message, &cart);
  setState(MESSAGE);
}

static bool extractProductId(const String& qrData, String& productId) {
  const String prefix = "product:";
  if (!qrData.startsWith(prefix)) {
    return false;
  }

  productId = qrData.substring(prefix.length());
  productId.trim();
  return productId.length() > 0;
}

static bool isDuplicateScan(const String& qrData) {
  return !scanDeduplicator.shouldAccept(qrData, millis());
}

static void handleQrCode(const String& qrData) {
  Serial.print("QR: ");
  Serial.println(qrData);

  if (isDuplicateScan(qrData)) {
    Serial.println("Ignored duplicate scan");
    return;
  }

  String productId;
  if (!extractProductId(qrData, productId)) {
    showMessage("よめないよ", "ちがうカードみたい");
    return;
  }

  const Product* product = productDB.findProductById(productId);
  if (product == nullptr) {
    showMessage("ないみたい", "べつのカードにしてね");
    return;
  }

  if (!cart.addProduct(product)) {
    showMessage("いっぱい", "かいけいしてね");
    return;
  }

  ui.showItemAdded(product, &cart);
  setState(ITEM_ADDED);
}

static void showCartPage(int page) {
  int itemCount = cart.getItemCount();
  if (itemCount == 0) {
    cartPage = 0;
  } else {
    int totalPages = (itemCount + CART_ITEMS_PER_PAGE - 1) / CART_ITEMS_PER_PAGE;
    cartPage = page % totalPages;
  }

  ui.showCart(&cart, cartPage);
  setState(CART);
}

static void showCheckout() {
  ui.showCheckout(&cart);
  setState(CHECKOUT);
}

static void handleButtons() {
  if (M5.BtnA.wasPressed()) {
    if (appState == CHECKOUT) {
      cart.clear();
    }
    showHome();
    return;
  }

  if (M5.BtnB.wasPressed()) {
    if (appState == CART) {
      showCartPage(cartPage + 1);
    } else {
      showCartPage(0);
    }
    return;
  }

  if (M5.BtnC.wasPressed()) {
    if (appState == CHECKOUT) {
      cart.clear();
      showHome();
    } else {
      showCheckout();
    }
  }
}

static void handleAutoReturn() {
  unsigned long elapsed = millis() - stateChangedAt;
  if (appState == MESSAGE && elapsed > MESSAGE_RETURN_MS) {
    showHome();
  } else if (appState == ITEM_ADDED && elapsed > ITEM_RETURN_MS) {
    showHome();
  }
}

void setup() {
  M5.begin(true, true, true, true);
  Serial.begin(115200);
  Serial.println("Stackan boot");

  productDB.init();
  qrScanner.init();
  ui.init();
  showHome();
}

void loop() {
  M5.update();
  handleButtons();

  String qrData;
  if (qrScanner.readQrCode(qrData)) {
    handleQrCode(qrData);
  }

  handleAutoReturn();
  delay(20);
}
