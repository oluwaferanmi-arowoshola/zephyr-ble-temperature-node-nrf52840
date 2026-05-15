#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

void thread_a(void *, void *, void *);
void thread_b(void *, void *, void *);

K_THREAD_STACK_DEFINE(stack_a, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_b, STACK_SIZE);

struct k_thread thread_a_data;
struct k_thread thread_b_data;

K_MUTEX_DEFINE(print_mutex);

static void safe_print(const char *thread_name, const char *message)
{
    int64_t ms = k_uptime_get();

    k_mutex_lock(&print_mutex, K_FOREVER);
    printk("[%lld ms] %s: %s\n", ms, thread_name, message);
    k_mutex_unlock(&print_mutex);
}

void thread_a(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        safe_print("THREAD_A", "running, then sleeping 1000 ms");
        k_sleep(K_SECONDS(1));
    }
}

void thread_b(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        safe_print("THREAD_B", "running, then sleeping 1000 ms");
        k_sleep(K_SECONDS(1));
    }
}

int main(void)
{
    safe_print("MAIN", "Phase 2 - Experiment 1C start");

    k_thread_create(&thread_a_data, stack_a, STACK_SIZE,
                    thread_a,
                    NULL, NULL, NULL,
                    5, 0, K_NO_WAIT);

    k_thread_create(&thread_b_data, stack_b, STACK_SIZE,
                    thread_b,
                    NULL, NULL, NULL,
                    5, 0, K_NO_WAIT);

    while (1) {
        safe_print("MAIN", "main thread running, then sleeping 3000 ms");
        k_sleep(K_SECONDS(3));
    }

    return 0;
}