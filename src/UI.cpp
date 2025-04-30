#include "UI.h"

// コンストラクタ
UI::UI(M5GFX* disp) {
  display = disp;
  canvas = new M5Canvas(display);
  currentState = STATE_WELCOME;
  currentCartPage = 0;
}

// デストラクタ
UI::~UI() {
  if (canvas != nullptr) {
    delete canvas;
  }
}

// 初期化
void UI::init() {
  canvas->createSprite(320, 240);
  canvas->setTextSize(2);
  canvas->setTextColor(WHITE);
  showWelcomeScreen();
}

// 初期画面表示
void UI::showWelcomeScreen() {
  canvas->fillScreen(BLACK);
  canvas->setTextSize(2);
  canvas->setCursor(20, 80);
  canvas->println("お買いものゲーム");
  canvas->setCursor(20, 120);
  canvas->println("QRコードをスキャンしてね！");
  
  // 操作説明
  canvas->setTextSize(1);
  canvas->setCursor(10, 200);
  canvas->println("A:リセット B:買い物かご C:会計");
  
  canvas->pushSprite(0, 0);
  currentState = STATE_WELCOME;
}

// 商品表示
void UI::showProduct(Product* product) {
  if (product == nullptr) return;
  
  canvas->fillScreen(BLACK);
  
  // 商品名を表示
  canvas->setTextSize(3);
  canvas->setCursor(10, 10);
  canvas->println(product->name);
  
  // 金額を表示
  canvas->setTextSize(3);
  canvas->setCursor(10, 50);
  canvas->print(product->price);
  canvas->println("円");
  
  // 商品イラストを表示（中央）
  int x = (320 - product->width) / 2;
  int y = (240 - product->height) / 2;
  canvas->pushImage(x, y, product->width, product->height, product->image);
  
  // 操作説明
  canvas->setTextSize(1);
  canvas->setCursor(10, 200);
  canvas->println("A:リセット B:カートに追加 C:会計");
  
  canvas->pushSprite(0, 0);
  currentState = STATE_PRODUCT;
  
  // 成功音を鳴らす
  playSuccessSound();
}

// 商品が見つからないエラー表示
void UI::showProductNotFound() {
  canvas->fillScreen(BLACK);
  canvas->setTextSize(2);
  canvas->setCursor(20, 100);
  canvas->println("商品が見つかりません");
  
  // 操作説明
  canvas->setTextSize(1);
  canvas->setCursor(10, 200);
  canvas->println("A:リセット B:買い物かご C:会計");
  
  canvas->pushSprite(0, 0);
  
  // エラー音を鳴らす
  playErrorSound();
}

// スキャンエラー表示
void UI::showScanError() {
  canvas->fillScreen(BLACK);
  canvas->setTextSize(2);
  canvas->setCursor(20, 100);
  canvas->println("スキャンエラー");
  canvas->setCursor(20, 130);
  canvas->println("もう一度スキャンしてね");
  
  // 操作説明
  canvas->setTextSize(1);
  canvas->setCursor(10, 200);
  canvas->println("A:リセット B:買い物かご C:会計");
  
  canvas->pushSprite(0, 0);
  
  // エラー音を鳴らす
  playErrorSound();
}

// 買い物かご表示
void UI::showCart(ShoppingCart* cart, int page) {
  if (cart == nullptr) return;
  
  currentCartPage = page;
  canvas->fillScreen(BLACK);
  
  canvas->setTextSize(2);
  canvas->setCursor(10, 10);
  canvas->println("買い物かご");
  
  int itemCount = cart->getItemCount();
  if (itemCount == 0) {
    canvas->setCursor(10, 100);
    canvas->println("カートは空です");
  } else {
    // 1ページあたりの表示アイテム数
    const int ITEMS_PER_PAGE = 3;
    int startIdx = page * ITEMS_PER_PAGE;
    int endIdx = min(startIdx + ITEMS_PER_PAGE, itemCount);
    
    for (int i = startIdx; i < endIdx; i++) {
      CartItem* item = cart->getItemAt(i);
      if (item != nullptr) {
        int y = 40 + (i - startIdx) * 40;
        canvas->setCursor(10, y);
        canvas->print(item->product->name);
        canvas->print(" x");
        canvas->print(item->quantity);
        canvas->print("  ");
        canvas->print(item->product->price * item->quantity);
        canvas->println("円");
      }
    }
    
    // 合計金額表示
    canvas->setTextSize(2);
    canvas->setCursor(10, 170);
    canvas->print("合計: ");
    canvas->print(cart->getTotalAmount());
    canvas->println("円");
    
    // ページ表示（複数ページがある場合）
    if (itemCount > ITEMS_PER_PAGE) {
      int totalPages = (itemCount + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
      canvas->setCursor(240, 170);
      canvas->print(page + 1);
      canvas->print("/");
      canvas->print(totalPages);
    }
  }
  
  // 操作説明
  canvas->setTextSize(1);
  canvas->setCursor(10, 200);
  canvas->println("A:リセット B:前へ C:次へ");
  
  canvas->pushSprite(0, 0);
  currentState = STATE_CART;
}

// 会計画面表示
void UI::showCheckout(ShoppingCart* cart) {
  if (cart == nullptr) return;
  
  canvas->fillScreen(BLACK);
  
  canvas->setTextSize(3);
  canvas->setCursor(10, 40);
  canvas->println("お会計");
  
  canvas->setTextSize(3);
  canvas->setCursor(10, 100);
  canvas->print("合計: ");
  canvas->print(cart->getTotalAmount());
  canvas->println("円");
  
  // 操作説明
  canvas->setTextSize(1);
  canvas->setCursor(10, 200);
  canvas->println("A:買い物を続ける B:カートをクリア C:終了");
  
  canvas->pushSprite(0, 0);
  currentState = STATE_CHECKOUT;
  
  // 成功音を鳴らす
  playSuccessSound();
}

// 成功音を鳴らす
void UI::playSuccessSound() {
  // スピーカーで成功音を鳴らす
  M5.Speaker.tone(SCAN_SOUND_FREQ, 100);
  delay(100);
  M5.Speaker.mute();
}

// エラー音を鳴らす
void UI::playErrorSound() {
  // スピーカーでエラー音を鳴らす
  M5.Speaker.tone(ERROR_SOUND_FREQ, 100);
  delay(100);
  M5.Speaker.mute();
  delay(100);
  M5.Speaker.tone(ERROR_SOUND_FREQ, 100);
  delay(100);
  M5.Speaker.mute();
}
