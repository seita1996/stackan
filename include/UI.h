#ifndef UI_H
#define UI_H

#include <M5Stack.h>
#include <M5GFX.h>
#include "Product.h"
#include "ShoppingCart.h"

// UIクラス
class UI {
public:
  // UI状態
  enum UIState {
    STATE_WELCOME,    // 初期画面
    STATE_PRODUCT,    // 商品表示
    STATE_CART,       // 買い物かご表示
    STATE_CHECKOUT    // 会計画面
  };

  // 現在のカートページ（main.cppからアクセスするため公開）
  int currentCartPage;

private:
  M5GFX* display;
  M5Canvas* canvas;
  
  UIState currentState;
  
  // サウンド
  const int SCAN_SOUND_FREQ = 1000;
  const int ERROR_SOUND_FREQ = 500;
  
public:
  UI(M5GFX* disp);
  ~UI();
  
  // 初期化
  void init();
  
  // 初期画面表示
  void showWelcomeScreen();
  
  // 商品表示
  void showProduct(Product* product);
  
  // 商品が見つからないエラー表示
  void showProductNotFound();
  
  // スキャンエラー表示
  void showScanError();
  
  // 買い物かご表示
  void showCart(ShoppingCart* cart, int page = 0);
  
  // 会計画面表示
  void showCheckout(ShoppingCart* cart);
  
  // 成功音を鳴らす
  void playSuccessSound();
  
  // エラー音を鳴らす
  void playErrorSound();
  
  // 現在の状態を取得
  UIState getState() { return currentState; }
  
  // 状態を設定
  void setState(UIState state) { currentState = state; }
};

#endif // UI_H
