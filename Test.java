public class Test {

    static int s_a;

    static int s_b = 3 + 4;

    static int s_c;

    static {
        s_c = 1_000_000_007;
    }

    static int s_zero() {
        return 0;
    }

    static int s_add(int a, int b) {
        return a + b;
    }

    public static void main(String[] args) {
        int a = 42;
        int b = 256;
        int c = 65536;
        int d = a + b - c;
        int e = a * 3;
        int f = e / 6;
        int g = e % 7;
        System.out.println(a);
        System.out.println(b);
        System.out.println(c);
        System.out.println(d);
        System.out.println(e);
        System.out.println(f);
        System.out.println(g);

        System.out.println(s_a);
        System.out.println(s_b);
        System.out.println(s_c);
        s_a = 800;
        System.out.println(s_a);

        System.out.println(s_zero());
        System.out.println(s_add(s_a, 8));

        Test test = new Test();
        System.out.println(test.i_a);
        System.out.println(test.i_b);
        System.out.println(test.i_c);
        test.i_a = 777;
        System.out.println(test.i_a);

        System.out.println(test.i_zero());
        System.out.println(test.i_add(13, 8));

        int[] h = {10, 20, 30};
        System.out.println(h.length);
        System.out.println(h[0]);
        System.out.println(h[1]);
        System.out.println(h[2]);
        h[1] = 40;
        System.out.println(h[1]);
    }

    int i_a;

    int i_b = 90000;

    int i_c;

    Test() {
        i_c = i_b + 1;
    }

    int i_zero() {
        return 0;
    }

    int i_add(int a, int b) {
        return a + b;
    }
}
