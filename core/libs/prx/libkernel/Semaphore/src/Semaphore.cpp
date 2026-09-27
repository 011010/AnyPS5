#include "prx/libkernel/Semaphore/include/Semaphore.hpp"

#include <stdexcept>
#include <string>
#include <utility>

KernelSemaPrivate::KernelSemaPrivate(std::int32_t initCount, std::int32_t maxCount, std::string name, bool isFifo)
 : name(std::move(name)), tokenCount(initCount), maxCount(maxCount), isFifo(isFifo) {
}

extern "C" {

int APS5_VABI sceKernelCreateSema(KernelSema* sem, const char* name, uint32_t attr, int init, int max, void* opt) {
 (void)opt;
 if (sem == nullptr || name == nullptr || attr > 2 || init < 0 || max <= 0 || init > max) {
  APS5_INVALID_ARG_EX;
 }

 *sem = new KernelSemaPrivate(init, max, std::string(name), attr == 1);
 return KERNEL_SEMA_OK;
}

int APS5_VABI sceKernelPollSema(KernelSema sem, int need) {
 if (sem == nullptr || need <= 0) {
  APS5_INVALID_ARG_EX;
 }

 std::lock_guard<std::mutex> lock(sem->mutex);
 if (sem->tokenCount < need) {
  return KERNEL_SEMA_ERROR_EBUSY;
 }
 sem->tokenCount -= need;
 return KERNEL_SEMA_OK;
}

int APS5_VABI sceKernelSignalSema(KernelSema sem, int count) {
 if (sem == nullptr || count <= 0) {
  APS5_INVALID_ARG_EX;
 }

 std::lock_guard<std::mutex> lock(sem->mutex);
 if (sem->tokenCount + count > sem->maxCount) {
  return KERNEL_SEMA_ERROR_EINVAL;
 }
 sem->tokenCount += count;
 sem->condition.notify_all();
 return KERNEL_SEMA_OK;
}

int APS5_VABI sceKernelWaitSema(KernelSema sem, int need, KernelUseconds* time) {
 if (sem == nullptr || need <= 0) {
  APS5_INVALID_ARG_EX;
 }

 std::unique_lock<std::mutex> lock(sem->mutex);
 ++sem->waiterCount;
 struct WaiterGuard {
  KernelSemaPrivate* sem;
  ~WaiterGuard() {
   --sem->waiterCount;
   sem->condition.notify_all();
  }
 } waiterGuard{sem};

 if (time == nullptr) {
  sem->condition.wait(lock, [&] { return sem->tokenCount >= need || sem->deleted; });
  if (sem->deleted) {
   return KERNEL_SEMA_ERROR_EACCES;
  }
  sem->tokenCount -= need;
  return KERNEL_SEMA_OK;
 }

 auto timeout = std::chrono::microseconds(*time);
 bool acquired = sem->condition.wait_for(lock, timeout, [&] { return sem->tokenCount >= need || sem->deleted; });
 if (sem->deleted) {
  return KERNEL_SEMA_ERROR_EACCES;
 }
 if (!acquired) {
  return KERNEL_SEMA_ERROR_ETIMEDOUT;
 }
 sem->tokenCount -= need;
 return KERNEL_SEMA_OK;
}

int APS5_VABI sceKernelCancelSema(KernelSema sem, int count, int* threads) {
 (void)sem;
 (void)count;
 (void)threads;
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceKernelDeleteSema(KernelSema sem) {
 if (sem == nullptr) {
  APS5_INVALID_ARG_EX;
 }

 std::unique_lock<std::mutex> lock(sem->mutex);
 sem->deleted = true;
 sem->condition.notify_all();
 while (sem->waiterCount > 0) {
  sem->condition.wait(lock);
 }
 lock.unlock();
 delete sem;
 return KERNEL_SEMA_OK;
}

}
