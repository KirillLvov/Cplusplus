#include "process.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

Process::Process(const std::string& path) {
	int pipe_read[2], pipe_write[2];
	if (pipe(pipe_read) == -1)
		throw std::runtime_error("Pipe error");
	if (pipe(pipe_write) == -1) {
		::close(pipe_read[0]);
		::close(pipe_read[1]);
		throw std::runtime_error("Pipe error");
	}

	child_pid_ = fork();
	if (child_pid_ == -1) {
		::close(pipe_read[0]);
		::close(pipe_read[1]);
		::close(pipe_write[0]);
		::close(pipe_write[1]);
		throw std::runtime_error("Fork error");
	}
	
	if (child_pid_ == 0) {
		::close(pipe_read[0]);
		if(dup2(pipe_read[1], 1) == -1) {
			::close(pipe_read[1]);
			::close(pipe_write[0]);
			::close(pipe_write[1]);
			throw std::runtime_error("Dup2 error");
		}
		::close(pipe_write[1]);
		if(dup2(pipe_write[0], 0) == -1) {
			::close(pipe_write[0]);
			throw std::runtime_error("Dup2 error");
		}
		if(execl(path.c_str(), NULL) == -1)
			throw std::runtime_error("Exec error");
	}
	else {
		::close(pipe_read[1]);
		fd_read_ = pipe_read[0];
		::close(pipe_write[0]);
		fd_write_ = pipe_write[1];
	}
}

Process::~Process() {
    close();
}

size_t Process::write(const void* data, size_t len) {
	int ret = ::write(fd_write_, data, len);
	if(ret == -1)
		throw std::runtime_error("Write error");
	return ret;
}

void Process::writeExact(const void* data, size_t len) {
	size_t left = len;
	size_t ret;
	while(left > 0) {
		ret = write(static_cast<const char*>(data)+len-left, left);
	if (ret == 0)
		throw std::runtime_error("No write");
		left -= ret;
	}
}

size_t Process::read(void* data, size_t len) {
	int ret = ::read(fd_read_, data, len);
	if(ret == -1)
		throw std::runtime_error("Read error");
	return ret;
}

void Process::readExact(void* data, size_t len) {
	size_t left = len;
	size_t ret;
	while(left > 0) {
		ret = read(static_cast<char*>(data)+len-left, left);
		if (ret == 0)
			throw std::runtime_error("No read");
		left -= ret;
    }
}

void Process::closeStdin() {
	if(fd_write_ > -1) {
		::close(fd_write_);
		fd_write_ = -1;
	}  
}

void Process::close() {
	if(fd_read_ > -1) {
		::close(fd_read_);
		fd_read_ = -1;
	}
	if(fd_write_ > -1) {
		::close(fd_write_);
		fd_write_ = -1;
	}
	if(child_pid_ > 0) {
		kill(child_pid_, SIGINT);
		waitpid(child_pid_, NULL, 0);
		child_pid_ = -1;
	}
}
