#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

#define MAIN_SLEEP 3000
#define THREAD1_SLEEP 1000
#define THREAD2_SLEEP 2000

void thread1_function(void *, void *, void *);
void thread2_function(void *, void *, void *);

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

struct k_thread thread1_data;
struct k_thread thread2_data;

void thread1_function(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        printk("High-priority Thread 1 is running\n");
        k_sleep(K_MSEC(THREAD1_SLEEP));
    }
}

void thread2_function(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        printk("Low-priority Thread 2 is running\n");
        k_sleep(K_MSEC(THREAD2_SLEEP));
    }
}

int main(void)
{
    printk("Phase 2 Goal 3: Priority experiment\n");

    k_thread_create(&thread1_data, thread1_stack, STACK_SIZE,
                    thread1_function,
                    NULL, NULL, NULL,
                    4, 0, K_NO_WAIT);

    k_thread_create(&thread2_data, thread2_stack, STACK_SIZE,
                    thread2_function,
                    NULL, NULL, NULL,
                    7, 0, K_NO_WAIT);

    while (1) {
        printk("Main thread is running\n");
        k_sleep(K_MSEC(MAIN_SLEEP));
    }

    return 0;
}