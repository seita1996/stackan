#ifndef QR_SCANNER_H
#define QR_SCANNER_H

#include <M5Stack.h>
#include <M5UnitQRCode.h>

class ISerial {
public:
  virtual bool available() = 0;
  virtual String readStringUntil(char terminator) = 0;
  virtual String readString() = 0;
  virtual void begin(long baudrate) = 0;
  virtual void begin(long baudrate, uint32_t config, int rxPin, int txPin) { begin(baudrate); }
  virtual void begin(long baudrate, uint32_t config, int rxPin, int txPin, bool invert) {
    begin(baudrate, config, rxPin, txPin);
  }
  virtual void flush() = 0;
  virtual ~ISerial() {}
};

class HardwareSerialWrapper : public ISerial {
private:
  HardwareSerial* serial;

public:
  explicit HardwareSerialWrapper(HardwareSerial* serial) : serial(serial) {}

  bool available() override { return serial->available(); }
  String readStringUntil(char terminator) override { return serial->readStringUntil(terminator); }
  String readString() override { return serial->readString(); }
  void begin(long baudrate) override { serial->begin(baudrate); }
  void begin(long baudrate, uint32_t config, int rxPin, int txPin) override {
    serial->begin(baudrate, config, rxPin, txPin);
  }
  void begin(long baudrate, uint32_t config, int rxPin, int txPin, bool invert) override {
    serial->begin(baudrate, config, rxPin, txPin, invert);
  }
  void flush() override { serial->flush(); }
};

class MockSerial : public ISerial {
public:
  String mockData;
  bool mockAvailable;

  MockSerial() : mockData(""), mockAvailable(false) {}

  bool available() override { return mockAvailable; }
  String readStringUntil(char terminator) override {
    (void)terminator;
    mockAvailable = false;
    return mockData;
  }
  String readString() override {
    mockAvailable = false;
    return mockData;
  }
  void begin(long baudrate) override { (void)baudrate; }
  void flush() override {}

  void simulateScan(String data) {
    mockData = data;
    mockAvailable = true;
  }
};

class QrScanner {
private:
  int baudRate;
  unsigned long lastReadTime;
  const unsigned long timeout = 5000;
  ISerial* serialPort;
  M5UnitQRCodeUART* unitQrCode;
  bool ownSerialPort;
  int rxPin;
  int txPin;
  bool invertSignal;
  bool useUnitDriver;

public:
  QrScanner(ISerial* serial, int baud = 9600);
  explicit QrScanner(int baud = 9600);
  ~QrScanner();

  void init();
  bool readQrCode(String& data);

  void setBaud(int baud);
  int getBaud() const { return baudRate; }

  void setPins(int rx, int tx);
  int getRxPin() const { return rxPin; }
  int getTxPin() const { return txPin; }

  void setInvert(bool invert);
  bool getInvert() const { return invertSignal; }

  bool isTimeout();
  void reset();
};

#endif // QR_SCANNER_H
