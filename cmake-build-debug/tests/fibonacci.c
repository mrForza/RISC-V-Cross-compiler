int main() {
    int n;
    read_int(n);
    int ai = 1;
    int ai2 = 1;
    int result;
    while (n - 2 > 0) {
        result = ai + ai2;
        ai = ai2;
        ai2 = result;
        n = n - 1;
    }
    write_int(result);
}