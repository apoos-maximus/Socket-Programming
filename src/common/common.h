
void common_function(void);

void err_quit (const char *fmt, ...);

void log_info (const char* file_name, const char* func_name, int line_num, const char *fmt, ...);

void print_family(int family_af);

#define LOG_INFO(...) (log_info(__FILE__, __func__, __LINE__, __VA_ARGS__))