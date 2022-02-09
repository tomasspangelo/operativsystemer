#include "b.h"

time_t parse_time(char time_string[]){
    struct tm t;
    strptime(time_string, "%Y-%m-%d %H:%M:%S", &t);
    return mktime(&t);
}

void add(struct alarm *a, int pid, char time_string[]) {
    struct alarm new_alarm = {.seconds = parse_time(time_string), .process_id = pid};
    a[pid] = new_alarm;

}

int main() {
    struct alarm a[] = {};
    int pid = 100;
    add(a, pid, "2022-01-31 15:15:23");
    printf("%lu %d", a[pid].seconds, a[pid].process_id);
}