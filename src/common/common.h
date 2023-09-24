
void common_function(void);

void err_quit (const char *fmt, ...);

void log_info (const char* func_name, const char *fmt, ...);

#define LOG_INFO(A, B) (log_info(__func__, A, B))