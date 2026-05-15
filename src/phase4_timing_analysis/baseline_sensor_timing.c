#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/settings/settings.h>
#include <zephyr/random/random.h>
#include <string.h>

#define STACK_SIZE 1024
#define SENSOR_PRIORITY 5
#define SENSOR_PERIOD_MS 2000

static int16_t temperature_c = 250; /* 25.0 C in tenths of a degree */
static int64_t last_sensor_update_ms = 0;
static uint32_t read_count = 0;
static uint32_t sensor_cycle_count = 0;

K_THREAD_STACK_DEFINE(sensor_stack, STACK_SIZE);
struct k_thread sensor_thread_data;

K_MUTEX_DEFINE(temp_mutex);

static ssize_t read_temp(struct bt_conn *conn,
                         const struct bt_gatt_attr *attr,
                         void *buf,
                         uint16_t len,
                         uint16_t offset)
{
    int16_t temp_copy;
    int64_t update_time_copy;
    int64_t now_ms;
    int64_t latency_ms;

    ARG_UNUSED(conn);

    k_mutex_lock(&temp_mutex, K_FOREVER);
    temp_copy = temperature_c;
    update_time_copy = last_sensor_update_ms;
    k_mutex_unlock(&temp_mutex);

    now_ms = k_uptime_get();
    latency_ms = now_ms - update_time_copy;
    read_count++;

    printk("BLE read #%u -> temp=%d.%d C, latency since sensor update = %lld ms\n",
           read_count,
           temp_copy / 10,
           (temp_copy < 0) ? -(temp_copy % 10) : (temp_copy % 10),
           latency_ms);

    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             &temp_copy, sizeof(temp_copy));
}

/* Custom service UUID: 12345678-1234-5678-1234-56789abcdef0 */
/* Custom temp characteristic UUID: 12345678-1234-5678-1234-56789abcdef1 */

BT_GATT_SERVICE_DEFINE(temp_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(
        BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0))),
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(
        BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)),
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        read_temp, NULL, NULL)
);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

void sensor_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    int64_t previous_update_ms = 0;

    while (1) {
        int16_t new_temp;
        int64_t now_ms;
        int64_t actual_period_ms = 0;
        int64_t period_error_ms = 0;

        new_temp = 200 + (sys_rand32_get() % 150);
        now_ms = k_uptime_get();

        if (previous_update_ms != 0) {
            actual_period_ms = now_ms - previous_update_ms;
            period_error_ms = actual_period_ms - SENSOR_PERIOD_MS;
        }

        k_mutex_lock(&temp_mutex, K_FOREVER);
        temperature_c = new_temp;
        last_sensor_update_ms = now_ms;
        k_mutex_unlock(&temp_mutex);

        sensor_cycle_count++;

        if (previous_update_ms == 0) {
            printk("Sensor cycle #%u -> temp=%d.%d C at %lld ms (first sample)\n",
                   sensor_cycle_count,
                   new_temp / 10,
                   (new_temp < 0) ? -(new_temp % 10) : (new_temp % 10),
                   now_ms);
        } else {
            printk("Sensor cycle #%u -> temp=%d.%d C at %lld ms, actual period = %lld ms, error = %+lld ms\n",
                   sensor_cycle_count,
                   new_temp / 10,
                   (new_temp < 0) ? -(new_temp % 10) : (new_temp % 10),
                   now_ms,
                   actual_period_ms,
                   period_error_ms);
        }

        previous_update_ms = now_ms;
        k_sleep(K_MSEC(SENSOR_PERIOD_MS));
    }
}

int main(void)
{
    int err;

    printk("Phase 4B: Scheduler timing measurement starting\n");

    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return 0;
    }

    printk("Bluetooth initialized\n");

    err = settings_load();
    if (err) {
        printk("settings_load failed (err %d)\n", err);
    } else {
        printk("Settings loaded\n");
    }

    err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        return 0;
    }

    printk("Advertising successfully started\n");

    k_thread_create(&sensor_thread_data, sensor_stack, STACK_SIZE,
                    sensor_thread,
                    NULL, NULL, NULL,
                    SENSOR_PRIORITY, 0, K_NO_WAIT);

    while (1) {
        k_sleep(K_SECONDS(5));
    }

    return 0;
}