#include "ScanDeduplicator.h"

ScanDeduplicator::ScanDeduplicator(unsigned long interval)
  : lastData(""), lastAcceptedAt(0), intervalMs(interval), hasLastData(false) {}

bool ScanDeduplicator::shouldAccept(const String& data, unsigned long now) {
  if (hasLastData && data == lastData && now - lastAcceptedAt < intervalMs) {
    return false;
  }

  lastData = data;
  lastAcceptedAt = now;
  hasLastData = true;
  return true;
}

void ScanDeduplicator::reset() {
  lastData = "";
  lastAcceptedAt = 0;
  hasLastData = false;
}
