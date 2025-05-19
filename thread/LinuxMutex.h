#ifndef THREADAPI_LINUXMUTEX_H
#define THREADAPI_LINUXMUTEX_H
#include <pthread.h>
#include <cerrno>
#include <stdexcept>

namespace rau
{
    class LinuxMutex
    {
    public:
        using native_handle_type = pthread_mutex_t;
    private:
        pthread_mutex_t mutex;
        static constexpr pthread_mutex_t initializeMutex() noexcept {
            pthread_mutex_t tempMutex = PTHREAD_MUTEX_INITIALIZER;
            return tempMutex;
        }
    public:
        constexpr LinuxMutex() noexcept : mutex(initializeMutex())
        {
            int result = pthread_mutex_init(&mutex, nullptr);
            if (result != 0){
                throw std::runtime_error("Mutex initialization failed. Not in a reliable state.");
            }
        }

        LinuxMutex( const LinuxMutex& ) = delete;

        LinuxMutex& operator=(const LinuxMutex&) = delete;

        ~LinuxMutex()
        {
            pthread_mutex_destroy(&mutex);
        }

        native_handle_type native_handle() const noexcept
        {
            return mutex;
        }

        void lock()
        {
            int result = pthread_mutex_lock(&mutex);
            if (result != 0)
            {
                throw std::runtime_error("Lock failed.");
            }
        }

        void unlock()
        {
            pthread_mutex_unlock(&mutex);
        }

        bool try_lock()
        {
            int result = pthread_mutex_trylock(&mutex);
            if (result == 0)
                return true;
            else if (result == EBUSY)
                return false;
        }
    };

    class LinuxMutexGuard
    {
    private:
        LinuxMutex& mutex;

    public:
        explicit LinuxMutexGuard(LinuxMutex& m) noexcept : mutex(m) {
            mutex.lock();
        }

        ~LinuxMutexGuard() {
            mutex.unlock();
        }

        LinuxMutexGuard(const LinuxMutexGuard&) = delete;
        LinuxMutexGuard& operator=(const LinuxMutexGuard&) = delete;
        LinuxMutexGuard(LinuxMutexGuard&&) = delete;
        LinuxMutexGuard& operator=(LinuxMutexGuard&&) = delete;
    };

} // namespace rau

#endif //THREADAPI_LINUXMUTEX_H