class Process
{
private:
    int fd_write_;
    int fd_read_;
    int child_pid_;
public:
	explicit Process(const std::string& path);
	~Process();

	size_t write(const void* data, size_t len);
	void writeExact(const void* data, size_t len);
	size_t read(void* data, size_t len);
	void readExact(void* data, size_t len);

	void closeStdin();

	void close();
};
