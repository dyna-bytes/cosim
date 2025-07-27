primitive channel 에는 대표적으로 sc_signal 과 sc_fifo 가 있다.

1. sc_signal<T>
이 channel 에는 sg_signal_inout_if<T> 의 read/write method 함수가 구현되어 있다. 즉, sc_signal은 verilog HDL의 wire 과 같은 개념으로써 module 과 module 간의 통신 매개체가 되는 것이다.

channel을 사용할 때 공통적으로 주의해야 하는 사항이 있다. 그것은 동시성을 위한 매커니즘인데 channel 의 read/write method 를 통해 어떤 값을 읽고 쓸 경우 그 결과는 시뮬레이션 단위 지연 시간 만큼 후에 반영이 된다는 것이다.

예를 들어 보자. 아래 a 라는 sc_signal에 초기값이 0이라고 가정하자.
```
sc_signal<int> a;
int y;

a.write(10);
y = a.read();
```

자 y에는 어떤 값이 있을까? 답은 0이다. 그럼 언제 10 이라는 값이 써지는 것일까? 아래 예제를 보자.

```
a.write(10);
wait();
y = a.read();
```
이제 y 를 출력하면 10이 들어가 있다.


{
    wait()
    - SystemC Kernel Scheduler 에게 수행 제어권을 넘김.
    - 선점형 OS 방식으로, main thread 가 수행되면 다른 어떤 thread 도 수행을 못 함.
    - 따라서 wait() 을 call 하여 제어권을 kernel에게 넘김.
}

SystemC kernel 이 현재 시간에 pending 되어 있는 모든 event 처리
(여기서는 a.write(10)을 진행하고 다음 event 로 simulation time을 증가 시키기. wait()) 를 해야 결과 값이 반영 되는 것이다.

-> 말을 진짜 못알아먹게 써놨는데,
1. port 에 값을 넘김 (event)
2. user -> kernel 로 제어권을 넘김 (wait)
3. kernel 이 event 를 처리함
4. 결과 값이 반영됨


2. sc_fifo<T>
sc_fifo 에는 blocking read/write method 와 non-blocking read/write method 그리고 몇 가지 FIFO 상태를 알 수 있는 method 가 구현되어 있다.

보통 functional modeling 이나 transactin level modeling 에서는 block read/write method 를 사용하고 있고, 실제 RLT level 의 HW 구현에는 non-blocking method 를 사용한다.

block method 는 read()/write() 이고 non-blocking method 는 nb_read()/nb_write() 이다.


- read() 를 call 하면 fifo 에 데이터가 있으면 return 하고 데이터가 없으면 데이터가 들어올 때까지 return 하지 않는다.
- nb_read() 를 call 하면 데이터가 있으면 데이터를 읽고 true 를 return 하고 데이터가 없으면 false 를 return 한다.
- write() 를 call 하면 fifo 에 빈 영역이 있으면 바로 write 하고 없으면 빈 영역이 생길 때 까지 기다렸다가 write 하고 return 한다.
- nb_write() 를 call 하면 빈 영역이 있으면 바로 write 하고 true 를 return 하고 빈 영역이 없으면 false 를 return 한다.

이 외에
- num_available() 은 읽을 수 있는 데이터의 개수를 리턴한다.
- num_free() 는 쓰기 가능한 빈 영역 개수를 리턴한다.
