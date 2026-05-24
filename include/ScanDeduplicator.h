#ifndef SCAN_DEDUPLICATOR_H
#define SCAN_DEDUPLICATOR_H

#include <M5Stack.h>

class ScanDeduplicator {
private:
  String lastData;
  unsigned long lastAcceptedAt;
  unsigned long intervalMs;
  bool hasLastData;

public:
  explicit ScanDeduplicator(unsigned long interval);

  bool shouldAccept(const String& data, unsigned long now);
  void reset();
};

#endif // SCAN_DEDUPLICATOR_H
