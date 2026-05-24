#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <unity.h>
#include "../include/Product.h"
#include "../include/ShoppingCart.h"
#include "../include/QrScanner.h"
#include "../include/ScanDeduplicator.h"
#include "../include/UI.h"
#include "../include/Images.h"

// グローバルなモックオブジェクト（MockSerialはQrScanner.hで定義）
extern MockSerial mockSerial2;

#endif // TEST_HELPERS_H
