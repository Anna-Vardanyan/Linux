#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include "daemon.cpp"

int main(int argc, char** argv) {

    int inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        std::cerr << "Error initializing inotify." << '\n';
        return 1;
    }

    int wd;
    for (std::size_t i = 1; i < argc; i++) {
        wd = inotify_add_watch(inotifyFd, argv[i], IN_ALL_EVENTS);
        if (wd == -1) {
            std::cerr << "Error adding watch to the directory." << '\n';
        }
        std::cout << "Watching " << argv[i] << '\n';
    }

    char buffer[sizeof(inotify_event) + NAME_MAX + 1];
    while (true) {
        int bytesRead = read(inotifyFd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            std::cerr << "Error reading inotify events." << '\n';
            return 1;
        }

        for (std::size_t i = 0; i < bytesRead;) {
            struct inotify_event* event = reinterpret_cast<struct inotify_event*>(&buffer[i]);
            if (event->mask & IN_CREATE) {
                std::cout << "File created: " << event->name << '\n';
            }
            if (event->mask & IN_MODIFY) {
                std::cout << "File modified: " << event->name << '\n';
            }
            if (event->mask & IN_DELETE) {
                std::cout << "File deleted: " << event->name << '\n';
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
    becomeDaemon(BD_NO_CHDIR);
    inotify_rm_watch(inotifyFd, wd);
    close(inotifyFd);

    return 0;
}
