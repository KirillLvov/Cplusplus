#include <stdlib.h>

class Process
{
private:
    int fd_write;
    int fd_read;
    int child_pid;
public:
	explicit Process(const std::string& path);
	~Process();

	size_t wwrite(const void* data, size_t len);
	void writeExact(const void* data, size_t len);
	size_t read(void* data, size_t len);
	void readExact(void* data, size_t len);

	void closeStdin();

	void close();
};
