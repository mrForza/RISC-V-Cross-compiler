int main() {
    int n1 = 1;
    int n2 = 1;
    int result = 0;
    int n = 5;
    while (n > 0) {
        result = n1 + n2;
        n1 = n2;
        n2 = result;
        n -= 1;
    }
}