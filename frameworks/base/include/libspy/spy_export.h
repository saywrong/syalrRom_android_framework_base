#ifdef __cplusplus
extern "C" {
#endif

void spy_zygote_init();

void spy_init();

extern "C"  void dalvik_invoke_watcher(const Method *curMethod, Method *callMethod, void *fp, void *thread);

#ifdef __cplusplus
}
#endif