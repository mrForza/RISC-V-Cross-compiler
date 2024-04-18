int main() {
    int n;
    int result = 1;
    read_int(n);
    while (n > 0) {
        result = result * n;
        n = n - 1;
    }
    write_int(result);
}