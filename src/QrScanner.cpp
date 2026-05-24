#include "QrScanner.h"

QrScanner::QrScanner(ISerial* serial, int baud)
  : baudRate(baud),
    lastReadTime(0),
    serialPort(serial),
    unitQrCode(nullptr),
    ownSerialPort(false),
    rxPin(-1),
    txPin(-1),
    invertSignal(false),
    useUnitDriver(false) {}

QrScanner::QrScanner(int baud)
  : baudRate(baud),
    lastReadTime(0),
    serialPort(new HardwareSerialWrapper(&Serial2)),
    unitQrCode(new M5UnitQRCodeUART()),
    ownSerialPort(true),
    rxPin(16),
    txPin(17),
    invertSignal(false),
    useUnitDriver(true) {}

QrScanner::~QrScanner() {
  if (unitQrCode != nullptr) {
    delete unitQrCode;
    unitQrCode = nullptr;
  }
  if (ownSerialPort && serialPort != nullptr) {
    delete serialPort;
    serialPort = nullptr;
  }
}

void QrScanner::init() {
  if (useUnitDriver && unitQrCode != nullptr) {
    Serial.print("Unit QRCode UART init RX");
    Serial.print(rxPin);
    Serial.print(" TX");
    Serial.print(txPin);
    Serial.print(" @ ");
    Serial.println(UNIT_QRCODE_UART_BAUD);

    if (!unitQrCode->begin(&Serial2, UNIT_QRCODE_UART_BAUD, rxPin, txPin)) {
      Serial.println("Warning: Unit QRCode UART init failed");
      return;
    }

    unitQrCode->setTriggerMode(AUTO_SCAN_MODE);
    baudRate = UNIT_QRCODE_UART_BAUD;
    lastReadTime = millis();
    return;
  }

  if (serialPort == nullptr) {
    Serial.println("Error: Serial port not set in QrScanner::init");
    return;
  }

  if (rxPin >= 0 && txPin >= 0) {
    serialPort->begin(baudRate, SERIAL_8N1, rxPin, txPin, invertSignal);
  } else {
    serialPort->begin(baudRate);
  }

  serialPort->flush();
  lastReadTime = millis();
}

bool QrScanner::readQrCode(String& data) {
  if (useUnitDriver && unitQrCode != nullptr) {
    if (!unitQrCode->available()) {
      return false;
    }

    data = unitQrCode->getDecodeData();
    data.trim();
    if (data.length() == 0) {
      Serial.println("Warning: Unit QRCode returned empty data");
      return false;
    }

    Serial.print("QR decode: ");
    Serial.println(data);
    lastReadTime = millis();
    return true;
  }

  if (serialPort == nullptr || !serialPort->available()) {
    return false;
  }

  data = serialPort->readStringUntil('\n');
  if (data.length() == 0 && serialPort->available()) {
    data = serialPort->readStringUntil('\r');
  }
  if (data.length() == 0 && serialPort->available()) {
    data = serialPort->readString();
  }

  data.trim();
  if (data.length() == 0) {
    Serial.println("Warning: Empty QR data received");
    return false;
  }

  String filtered = "";
  for (size_t i = 0; i < data.length(); i++) {
    char c = data.charAt(i);
    if (c >= 0x20 && c <= 0x7E) {
      filtered += c;
    }
  }

  data = filtered;
  if (data.length() == 0) {
    Serial.println("Warning: Non-printable QR data received");
    return false;
  }

  Serial.print("QR read: ");
  Serial.println(data);
  lastReadTime = millis();
  return true;
}

void QrScanner::setBaud(int baud) {
  baudRate = baud;
  init();
}

void QrScanner::setPins(int rx, int tx) {
  rxPin = rx;
  txPin = tx;
  init();
}

void QrScanner::setInvert(bool invert) {
  invertSignal = invert;
  init();
}

bool QrScanner::isTimeout() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime;

  if (currentTime < lastReadTime) {
    elapsedTime = (0xFFFFFFFF - lastReadTime) + 1 + currentTime;
  } else {
    elapsedTime = currentTime - lastReadTime;
  }

  return elapsedTime > timeout;
}

void QrScanner::reset() {
  if (serialPort == nullptr) {
    Serial.println("Error: Serial port not set in QrScanner::reset");
    return;
  }

  serialPort->flush();
  lastReadTime = millis();
}
