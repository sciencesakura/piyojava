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

        if (a == 0) {
            System.out.println(0);
        }
        if (a != 0) {
            System.out.println(1);
        }
        if (a == 42) {
            System.out.println(2);
        }
        if (a != 42) {
            System.out.println(3);
        }
        if (a < 42) {
            System.out.println(4);
        }
        if (a < 43) {
            System.out.println(5);
        }
        if (a <= 41) {
            System.out.println(6);
        }
        if (a <= 42) {
            System.out.println(7);
        }
        if (a > 41) {
            System.out.println(8);
        }
        if (a > 42) {
            System.out.println(9);
        }
        if (a >= 42) {
            System.out.println(10);
        }
        if (a >= 43) {
            System.out.println(11);
        }
        int[] i = null;
        if (h == null) {
            System.out.println(12);
        }
        if (i == null) {
            System.out.println(13);
        }
        if (h != null) {
            System.out.println(14);
        }
        if (i != null) {
            System.out.println(15);
        }
        if (h == i) {
            System.out.println(16);
        }
        if (h != i) {
            System.out.println(17);
        }
        i = h;
        if (h == i) {
            System.out.println(18);
        }
        if (h != i) {
            System.out.println(19);
        }
        for (int j = 0; j < 5; j++) {
            System.out.println(j);
        }

        System.out.println('A');
        char[] k = {'a', 'i', 'u'};
        System.out.println(k.length);
        System.out.println(k[0]);
        System.out.println(k[1]);
        System.out.println(k[2]);
        k[1] = 'x';
        System.out.println(k[1]);

        System.out.println("Hello, world!");
        System.out.println("你好 世界!");
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
