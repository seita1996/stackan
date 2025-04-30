#include <M5Stack.h>
#include <M5GFX.h>
#include "Product.h"
#include "QrScanner.h"
#include "ShoppingCart.h"
#include "UI.h"
#include "Images.h"

// グローバルインスタンス
M5GFX display;
ProductDB productDB;
QrScanner qrScanner(9600); // ボーレートは使用するスキャナに合わせて調整
ShoppingCart cart;
UI* ui;

// QRコードデータとUIの状態
String qrData = "";
bool newQrDetected = false;

// QRコードの処理
void handleQrCode() {
  Serial.print("QRコードを処理: ");
  Serial.println(qrData);
  
  // QRコードデータの先頭には識別子が付いている前提
  // 例: "product:0"、"product:1"など
  if (qrData.startsWith("product:")) {
    String productId = qrData.substring(8); // "product:"の後の文字列を取得
    Product* product = productDB.findProductById(productId);
    
    if (product != nullptr) {
      // 商品が見つかった場合、表示
      ui->showProduct(product);
    } else {
      // 未登録の商品の場合
      ui->showProductNotFound();
    }
  } else {
    // 無効なQRコード形式
    ui->showScanError();
  }
}

// ボタン操作の処理
void handleButtonPress() {
  // ボタンA（リセット/戻る）が押された場合
  if (M5.BtnA.wasPressed()) {
    switch (ui->getState()) {
      case UI::STATE_WELCOME:
      case UI::STATE_PRODUCT:
      case UI::STATE_CART:
        ui->showWelcomeScreen();
        break;
      case UI::STATE_CHECKOUT:
        // 会計画面から買い物を続ける
        ui->showWelcomeScreen();
        break;
    }
  }
  
  // ボタンB（アクション）が押された場合
  if (M5.BtnB.wasPressed()) {
    switch (ui->getState()) {
      case UI::STATE_WELCOME:
        // 買い物かごを表示
        ui->showCart(&cart);
        break;
      case UI::STATE_PRODUCT:
        // カートに商品を追加
        if (qrData.startsWith("product:")) {
          String productId = qrData.substring(8);
          Product* product = productDB.findProductById(productId);
          if (product != nullptr) {
            cart.addProduct(product);
            // 追加成功を表示（フラッシュメッセージなど）
            Serial.println("商品をカートに追加しました");
          }
        }
        break;
      case UI::STATE_CART:
        // 前のページへ
        if (ui->currentCartPage > 0) {
          ui->showCart(&cart, ui->currentCartPage - 1);
        }
        break;
      case UI::STATE_CHECKOUT:
        // カートをクリア
        cart.clear();
        ui->showWelcomeScreen();
        break;
    }
  }
  
  // ボタンC（進む/会計）が押された場合
  if (M5.BtnC.wasPressed()) {
    switch (ui->getState()) {
      case UI::STATE_WELCOME:
      case UI::STATE_PRODUCT:
        // 会計画面へ
        ui->showCheckout(&cart);
        break;
      case UI::STATE_CART:
        {
          // 次のページへ
          int itemCount = cart.getItemCount();
          const int ITEMS_PER_PAGE = 3;
          int totalPages = (itemCount + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
          
          if (ui->currentCartPage < totalPages - 1) {
            ui->showCart(&cart, ui->currentCartPage + 1);
          } else {
            // 最後のページから会計画面へ
            ui->showCheckout(&cart);
          }
        }
        break;
      case UI::STATE_CHECKOUT:
        // 買い物終了（リセット）
        cart.clear();
        ui->showWelcomeScreen();
        break;
    }
  }
}

void setup() {
  // M5Stackの初期化
  M5.begin();
  
  // ディスプレイの初期化
  display.begin();
  display.setRotation(1); // 画面の向き（必要に応じて調整）
  
  // UIの初期化
  ui = new UI(&display);
  ui->init();
  
  // QRコードスキャナの初期化
  qrScanner.init();
  
  // 商品データベースの初期化
  productDB.init();
  
  // デバッグ用シリアル通信
  Serial.begin(115200);
  Serial.println("M5Stack QR Shopping Game - 初期化完了");
}

void loop() {
  M5.update(); // ボタン状態の更新
  
  // ボタン操作の処理
  handleButtonPress();
  
  // QRコードの読み取り
  if (qrScanner.readQrCode(qrData)) {
    handleQrCode();
  }
  
  // タイムアウトチェック
  if (qrScanner.isTimeout() && ui->getState() == UI::STATE_PRODUCT) {
    // 商品表示後にタイムアウトした場合は初期画面に戻る
    ui->showWelcomeScreen();
  }
  
  delay(50); // 短い遅延
}
