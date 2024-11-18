int pwr(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}
void mprn(int *a, int b) {
    printf("+++ MEM[%d] set to %d\n", b, a[b]);
}
void eprn(int *a,int b) {
    printf("+++ Standalone expression evaluates to %d\n", a[b]);
}
