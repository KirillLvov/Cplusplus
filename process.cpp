#include "process.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

Process::Process(const std::string& path) {
    int pipe_read[2], pipe_write[2];
    if (pipe(pipe_read) == -1) {
        std::cerr << "pipe error";
        exit(1);
    }
    if (pipe(pipe_write) == -1) {
        std::cerr << "pipe error";
        exit(1);
    }
    
    pid_t cpid = fork();
    if (cpid == -1) {
        std::cerr << "fork error";
        exit(2);
    }
    
    if (cpid == 0) {
        close(pipe_read[0]);
        close(pipe_write[1]);
        execl(path, NULL);
    }
    else {
        this->child_pid = cpid;
        close(pipe_read[1]);
        this->fd_read = pipe_read[0];
        close(pipe_write[0]);
        this->fd_write = pipe_write[1];
    }
}

Process::~Process() {
    close(this->fd_read);
    close(this->fd_write);
}

size_t Process::write(const void* data, size_t len) {
    write(this->fd_write, data, len);
    return len;
}

void Process::writeExact(const void* data, size_t len) {
    for(int i=0; i<len; i++)
        write(data+i, len);
}

size_t Process::read(const void* data, size_t len) {
    read(this->fd_read, data, len);
    return len;
}

void Process::readExact(void* data, size_t len) {
    for (int i=0; i<len; i++)
        read(static_cast<char*>(data)+i, 1);
}

void Process::closeStdin() {
    close(this->fd_write);    
}

void Process::close() {
    close(this->fd_read);
    close(this->fd_write);
    kill(this->child_pid, SIGINT);
    wait(0);
}
