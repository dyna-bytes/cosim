Process : SystemC 의 기본적인 실행 단위

Process 의 종류
* Method
* Thread
* Clock Thread

각 process 는 sensitivity list 를 갖는데, sensitivity list 에 기술된 signal 에 event 가 발생할 때 process가 실행된다. 여기서 event 라 함은 signal 의 value 가 바뀌는 것이다.

1. Method
Method 는 sensitivity list 에 기술된 signal 의 event 가 발생할 경우 시작하고 순차적으로 실행된 후에 simulation kernel 로 return 한다.

2. Thread
Thread 는 Method 와 동일하나 code 사이에 wait() 문을 사용할 수 있다.
wait() 에 다양한 event 를 인자로 줄 수 있으므로 dynamic sensitive 기능이 가능하다.

예시 코드에서는 method 와 다르게 clock 을 선언하여 사용하고 있다.
일단 my_module 에 clock port 를 선언하였는데 이는 다음과 같이 선언한다.

sc_in_clk clk;

sc_in_clk 은 clock type 의 input port 를 가리키는 것이고 clk 은 clock 이름이다.
constructor 에서 my_thread() 함수를 SC_THREAD macro 를 이용하여 thread 로 선언해준다.
그리고 sensitive event 로 clk.pos() 함수를 지정해 준다. 이는 wait() 함수에서 'clk' clock 의 positive edge 가 발생할 때까지 기다린다는 의미이다.
Thread 에서는 보통 while(1) 과 같은 무한 loop 안에서 필요한 기능을 수행하고 이 loop 안에 최소한 하나의 wait() 함수를 호출해야 한다.
sc_main() 함수 안에서 아래와 같이 clock 을 선언해 준다.

sc_clock clk("clock", 5, SC_NS, 0,5, 0.0, SC_NS, true);

|----|----|
|parameter| 설명 |
|"clock" | 이름  |
|    5   | 주기  |
| SC_NS  | 주기의 타임 단위, nano second |
| 0.5    | duty cycle, 50% (0~1 사이 값) |
| 0.0    | 시작 delay |
| SC_NS  | 시작 delay 의 타임 단위 |
| true   | 처음 만나는 edge type. true : posedge, false : negedge |

3. Clock Thread

Clock thread 는 thread 의 특수한 형태이다. Clock Thread 는 clock 의 edge 에서만 trigger 되며 hardware flip-flop 의 특성을 기술하는데 유용하다.
Sensitivity list 를 따로 기술하지 않고 SC_THREAD 으 두번째 인자로 기술한다.