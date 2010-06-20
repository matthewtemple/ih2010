#ifndef ih_net_engine_thread_h
#define ih_net_engine_thread_h

struct ih_net_engine_thread_t {
  void *engine_object;
  unsigned short thread_index;
};
typedef struct ih_net_engine_thread_t ih_net_engine_thread_t;

ih_net_engine_thread_t *ih_net_engine_create_thread(void *engine_object,
    unsigned short thread_index);

void ih_net_engine_destroy_thread(ih_net_engine_thread_t *engine_thread);

#endif
