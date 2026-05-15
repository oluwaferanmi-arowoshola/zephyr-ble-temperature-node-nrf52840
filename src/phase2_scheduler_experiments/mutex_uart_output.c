#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

#define MAIN_SLEEP    3000
#define THREAD1_SLEEP 1000
#define THREAD2_SLEEP 2000

void thread1_function(void *, void *, void *);
void thread2_function(void *, void *, void *);

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

struct k_thread thread1_data;
struct k_thread thread2_data;

K_MUTEX_DEFINE(print_mutex);

static void safe_print(const char *msg)
{
    k_mutex_lock(&print_mutex, K_FOREVER);
    printk("%s\n", msg);
    k_mutex_unlock(&print_mutex);
}

void thread1_function(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        safe_print("High-priority Thread 1 is running");
        k_sleep(K_MSEC(THREAD1_SLEEP));
    }
}

void thread2_function(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        safe_print("Low-priority Thread 2 is running");
        k_sleep(K_MSEC(THREAD2_SLEEP));
    }
}

int main(void)
{
    safe_print("Phase 2 Goal 3: Priority experiment with mutex");

    k_thread_create(&thread1_data, thread1_stack, STACK_SIZE,
                    thread1_function,
                    NULL, NULL, NULL,
                    4, 0, K_NO_WAIT);

    k_thread_create(&thread2_data, thread2_stack, STACK_SIZE,
                    thread2_function,
                    NULL, NULL, NULL,
                    7, 0, K_NO_WAIT);

    while (1) {
        safe_print("Main thread is running");
        k_sleep(K_MSEC(MAIN_SLEEP));
    }

    return 0;
}