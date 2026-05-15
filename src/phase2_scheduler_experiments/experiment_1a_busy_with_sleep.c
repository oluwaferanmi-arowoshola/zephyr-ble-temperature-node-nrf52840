#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys_clock.h>
#include <stdio.h>

#define STACK_SIZE 1024

void high_priority_thread(void *, void *, void *);
void low_priority_thread(void *, void *, void *);

K_THREAD_STACK_DEFINE(high_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(low_stack, STACK_SIZE);

struct k_thread high_thread_data;
struct k_thread low_thread_data;

K_MUTEX_DEFINE(print_mutex);

static void safe_print(const char *thread_name, const char *message)
{
    int64_t ms = k_uptime_get();

    k_mutex_lock(&print_mutex, K_FOREVER);
    printk("[%lld ms] %s: %s\n", ms, thread_name, message);
    k_mutex_unlock(&print_mutex);
}

void high_priority_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    volatile uint32_t counter = 0;

    while (1) {
        counter++;

        if ((counter % 5000000U) == 0U) {
            safe_print("HIGH", "busy thread running, then sleeping 10 ms");
            k_sleep(K_MSEC(10));
        }
    }
}

void low_priority_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        safe_print("LOW", "periodic thread running, then sleeping 2000 ms");
        k_sleep(K_SECONDS(2));
    }
}

int main(void)
{
    safe_print("MAIN", "Phase 2 - Experiment 1A start");

    k_thread_create(&high_thread_data, high_stack, STACK_SIZE,
                    high_priority_thread,
                    NULL, NULL, NULL,
                    3, 0, K_NO_WAIT);

    k_thread_create(&low_thread_data, low_stack, STACK_SIZE,
                    low_priority_thread,
                    NULL, NULL, NULL,
                    7, 0, K_NO_WAIT);

    while (1) {
        safe_print("MAIN", "main thread running, then sleeping 3000 ms");
        k_sleep(K_SECONDS(3));
    }

    return 0;
}