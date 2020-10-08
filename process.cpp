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
        ::close(pipe_read[0]);
        ::close(pipe_write[1]);
        if(execl(path, NULL) == -1) {
            std::cerr << "exec error";
            exit(3);
        }
    }
    else {
        this->child_pid = cpid;
        ::close(pipe_read[1]);
        this->fd_read = pipe_read[0];
        ::close(pipe_write[0]);
        this->fd_write = pipe_write[1];
    }
}

Process::~Process() {
    if(this->fd_read > 0) {
        ::close(this->fd_read);
        this->fd_read = -1;
    }
    if(this->fd_write > 0) {
        ::close(this->fd_write);
        this->fd_write = -1;
    }
    kill(this->child_pid, SIGINT);
    wait(0);
}

size_t Process::write(const void* data, size_t len) {
    if(::write(this->fd_write, data, len) == len)
        return len;
    else
        throw -1;
}

void Process::writeExact(const void* data, size_t len) {
    for(int i=0; i<len; i++)
        write(static_cast<const char*>(data)+i, 1);
}

size_t Process::read(void* data, size_t len) {
    if(::read(this->fd_read, data, len) == len)
        return len;
    else
        throw -1;
}

void Process::readExact(void* data, size_t len) {
    for (int i=0; i<len; i++)
        read(static_cast<char*>(data)+i, 1);
}

void Process::closeStdin() {
    if(this->fd_write > 0) {
        ::close(this->fd_write);
        this->fd_write = -1;
    }  
}

void Process::close() {
    if(this->fd_read > 0) {
        ::close(this->fd_read);
        this->fd_read = -1;
    }
    if(this->fd_write > 0) {
        ::close(this->fd_write);
        this->fd_write = -1;
    }
    kill(this->child_pid, SIGINT);
    wait(0);
}
