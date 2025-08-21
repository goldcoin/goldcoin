// Win32 threading shim for Qt compatibility
// This provides the win32 threading symbols that Qt expects
// while using the posix threading compiler

#ifdef _WIN32

#include <windows.h>
#include <pthread.h>
#include <time.h>

// Map win32 threading functions to pthread equivalents
// These are the symbols Qt is looking for

int __gthr_win32_mutex_lock(pthread_mutex_t *mutex) {
    return pthread_mutex_lock(mutex);
}

int __gthr_win32_mutex_unlock(pthread_mutex_t *mutex) {
    return pthread_mutex_unlock(mutex);
}

int __gthr_win32_mutex_init(pthread_mutex_t *mutex) {
    return pthread_mutex_init(mutex, NULL);
}

int __gthr_win32_mutex_destroy(pthread_mutex_t *mutex) {
    return pthread_mutex_destroy(mutex);
}

int __gthr_win32_recursive_mutex_lock(pthread_mutex_t *mutex) {
    return pthread_mutex_lock(mutex);
}

int __gthr_win32_recursive_mutex_unlock(pthread_mutex_t *mutex) {
    return pthread_mutex_unlock(mutex);
}

int __gthr_win32_recursive_mutex_init(pthread_mutex_t *mutex) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int result = pthread_mutex_init(mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    return result;
}

int __gthr_win32_recursive_mutex_destroy(pthread_mutex_t *mutex) {
    return pthread_mutex_destroy(mutex);
}

// Condition variable support
typedef pthread_cond_t __gthread_cond_t;

int __gthr_win32_cond_init(__gthread_cond_t *cond) {
    return pthread_cond_init(cond, NULL);
}

int __gthr_win32_cond_destroy(__gthread_cond_t *cond) {
    return pthread_cond_destroy(cond);
}

int __gthr_win32_cond_wait(__gthread_cond_t *cond, pthread_mutex_t *mutex) {
    return pthread_cond_wait(cond, mutex);
}

int __gthr_win32_cond_signal(__gthread_cond_t *cond) {
    return pthread_cond_signal(cond);
}

int __gthr_win32_cond_broadcast(__gthread_cond_t *cond) {
    return pthread_cond_broadcast(cond);
}

int __gthr_win32_cond_timedwait(__gthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime) {
    return pthread_cond_timedwait(cond, mutex, abstime);
}

// Mutex init function pointer (Qt expects this to be non-NULL)
void* __gthr_win32_mutex_init_function = (void*)1;

#endif // _WIN32