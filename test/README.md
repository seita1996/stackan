# M5Stack QR Shopping Game テストガイド

このプロジェクトには、複数のユニットテストとエンドツーエンドテストが含まれています。これらのテストは、PlatformIO Test Runnerを使用して実行できます。

## テストの実行方法

### すべてのテストを実行

PlatformIO IDEを使用している場合：

1. PlatformIO IDEのサイドバーで「Testing」アイコンをクリックします
2. 「Run All Tests」ボタンをクリックします

コマンドラインを使用している場合：

```
cd /Users/tahara/Documents/PlatformIO/Projects/Stackan
pio test
```

### 特定のテストのみを実行

特定のテストのみを実行するには、以下のコマンドを使用します：

```
pio test -f test_product
```

`-f` オプションの後に実行したいテストファイル名（拡張子なし）を指定します。

## テスト内容

このプロジェクトには以下のテストが含まれています：

1. **ProductDBテスト** (test_product.cpp)
   - 商品データベースの初期化と商品検索機能をテスト
   - 境界値テストを含む

2. **ShoppingCartテスト** (test_shopping_cart.cpp)
   - カートへの商品追加/削除
   - 合計金額計算
   - 複数数量の商品管理
   - 無効な入力のハンドリング

3. **QRScannerテスト** (test_qr_scanner.cpp)
   - QRコード読み取り機能
   - タイムアウト機能
   - リセット機能

4. **統合テスト** (test_integration.cpp)
   - QRコードスキャンから商品表示までのフロー
   - 複数商品のカート管理
   - 完全な買い物プロセス

## モックオブジェクト

実際のハードウェアがなくてもテストできるよう、以下のモックオブジェクトを使用しています：

- **MockSerial**: Serial2の振る舞いをシミュレートするためのモッククラス（QRコードスキャナ通信用）

## テスト結果の確認

テスト実行後、PlatformIO IDEの「Terminal」タブにテスト結果が表示されます。すべてのテストが成功すると、以下のように表示されます：

```
Test    : test_product.cpp
Success : test_product_init
Success : test_product_find
Success : test_product_edge_cases
```

エラーが発生した場合は、具体的なエラー内容と行番号が表示されます。

## テストのカスタマイズ

新しいテストを追加する場合は、`test/`ディレクトリに新しいテストファイルを作成し、以下の構造に従ってください：

```cpp
#include <unity.h>
#include "test_helpers.h"

// テスト対象のインスタンス

// セットアップ関数
void setUp(void) {
  // テスト前の初期化
}

void tearDown(void) {
  // テスト後のクリーンアップ
}

// テスト関数
void test_my_function() {
  // テストコード
  TEST_ASSERT_EQUAL(expected, actual);
}

void RUN_UNITY_TESTS() {
  UNITY_BEGIN();
  RUN_TEST(test_my_function);
  UNITY_END();
}

// Arduino/PlatformIOフレームワークではsetupとloopが必要
void setup() {
  delay(2000);
  RUN_UNITY_TESTS();
}

void loop() {
  // 何もしない
}
```
