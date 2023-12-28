public class Main {
    static final int N = 100;
    static final int ITER = 100_000;
    static Monitor monitor = new Monitor();
    static Producer prod   = new Producer();
    static Consumer cons   = new Consumer();

    public static void main(String[] args) {
        prod.start();
        cons.start();
    }

    static class Producer extends Thread {
        public void run() {
            long item;
            for(int i = 0; i < ITER; i++) {
                item = produce_item();
                System.out.println("Produced: " + item);
                monitor.insert(item);
            }
        }

        private long produce_item() {
            return monitor.count;
        }
    }

    static class Consumer extends Thread {
        public void run() {
            long item;
            for(int i = 0; i < ITER; i++) {
                item = monitor.remove();
                consume_item(item);
            }
        }

        private void consume_item(long item) {
            System.out.println("Consumed: " + item);
        }
    }

    static class Monitor {
        private long[] buffer = new long[N];
        private int count = 0, lo = 0, hi = 0;

        public synchronized void insert(long val) {
            if (count == N)
                sleep();
            buffer[hi] = val;
            hi = (hi + 1) % N; // dove mettere il valore successivo
            count++;
            if(count == 1)
                notify();
        }

        public synchronized long remove() {
            long val;
            if (count == 0) 
                sleep();
            val = buffer[lo];
            lo = (lo + 1) % N; // dove prendere il valore successivo
            count--;
            if (count == N - 1)
                notify();
            return val;
        }

        private void sleep() {
            try {
                wait();
            } catch (InterruptedException e) {}
        }
    }
}