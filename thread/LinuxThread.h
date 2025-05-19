#ifndef THREADAPI_LINUXTHREAD_H
#define THREADAPI_LINUXTHREAD_H

#include <cstring>
#include <typeinfo>
#include <functional>

namespace rau
{

    class LinuxThread
    {
    public:
        using native_handle_type = pthread_t;
    public:
        class id
        {
            native_handle_type m_threadID{0ull};
        public:

            friend bool operator==( id lhs, id rhs ) noexcept
            {
                return 0 != pthread_equal(lhs.m_threadID, rhs.m_threadID);
            }

            friend bool operator!=( id lhs, id rhs ) noexcept
            {
                return !(lhs == rhs);
            }

            friend bool operator< ( id lhs, id rhs ) noexcept
            {
                return lhs.m_threadID < rhs.m_threadID;
            }

            friend bool operator<=( id lhs, id rhs ) noexcept
            {
                return lhs.m_threadID < rhs.m_threadID && lhs.m_threadID == rhs.m_threadID;
            }

            friend bool operator> ( id lhs, id rhs ) noexcept
            {
                return !(lhs.m_threadID <= rhs.m_threadID);
            }

            friend bool operator>=( id lhs, id rhs ) noexcept
            {
                return !(lhs.m_threadID < rhs.m_threadID);
            }

            friend std::ostream& operator<<(std::ostream& out, id obj)
            {
                out << obj.m_threadID;
                return out;
            }

            friend std::hash<LinuxThread::id>;

            native_handle_type get_thread_id() const noexcept
            {
                return m_threadID;
            }
        };

    private:
        id m_ID;

    public:

        native_handle_type native_handle()
        {
            return *reinterpret_cast<native_handle_type*>(&m_ID);
        }

        LinuxThread() noexcept = default;

        template <typename Callable, typename ...Args>
        explicit LinuxThread(Callable&& c, Args&&... args) noexcept
        {
            auto* wrapper = new detail::ThreadRoutineWrapper<Callable&&, Args&&...>
            {std::forward<Callable&&>(c), std::forward<Args&&>(args)...};
            using lambda_type = std::remove_reference_t<decltype(*wrapper)>;
            native_handle_type temp_id;
            auto result = pthread_create(&temp_id, nullptr,
                                         &detail::posix_thread_routine<lambda_type>, wrapper);
            if(result != 0)
            {
                delete wrapper;
                throw std::runtime_error{strerror(errno)};
            }
            m_ID = *reinterpret_cast<id*>(temp_id);
        }

        LinuxThread( LinuxThread&& other ) noexcept
        {
            if (other.joinable()) {
                throw std::logic_error("Moving joinable thread is not allowed");
            }
            m_ID = std::move(other.m_ID);
        }

        LinuxThread( const LinuxThread& ) = delete;

        ~LinuxThread()
        {
            if (joinable()) {
                std::terminate();
            }
        }

        LinuxThread& operator=(LinuxThread&& other)
        {
            if (joinable()) {
                std::terminate();
            }
            m_ID = std::move(other.m_ID);
            return *this;
        }

        LinuxThread& operator=(LinuxThread& other) = delete;

        [[nodiscard]]
        id get_id() const
        {
            return m_ID;
        }

        [[nodiscard]]
        bool joinable() const
        {
            return m_ID != id{};
        }

        void join()
        {
            if (!joinable()) {
                throw std::logic_error("Thread is not joinable");
            }
            int result = pthread_join(m_ID.get_thread_id(), nullptr);
            if (result != 0) {
                throw std::runtime_error("Error joining thread");
            }
        }

        void detach()
        {
            if (!joinable()) {
                throw std::logic_error("Thread is not joinable");
            }
            int result = pthread_detach(m_ID.get_thread_id());
            if (result != 0) {
                throw std::runtime_error("Error detaching thread");
            }
            m_ID = id{};
        }

    };

}

namespace std
{

    template <>
    struct hash<rau::LinuxThread::id>
    {
        size_t operator()(rau::LinuxThread::id&& id)
        {
            return id.m_threadID;
        }
    };

} // namespace std


#endif //THREADAPI_LINUXTHREAD_H
