public class Test {

    static int a;

    static int zero() {
        return 0;
    }

    static int inc(int n) {
        return n + 1;
    }

    static int add(int a, int b) {
        return a + b;
    }

    public static void main(String[] args) {
        int a = 42;
        int b = 256;
        int c = 65536;
        int d = 0;
        int e = -42;
        int f = zero();
        a = add(inc(a), b) - c;
        b++;
        c = c / 71;
        d = e * 3;
        e = -d;
        Test.a = a;
        b = Test.a;
        Test g = new Test();
    }
}
