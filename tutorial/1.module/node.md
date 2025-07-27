sc_in, sc_out : input output port 를 구현하기 위한 class

SC_HAS_PROCESS() :
    - class 의 contructor 를 구현하기 위한 macro
    - class name 을 사용한다.

Adder(sc_module_name name) :
    - Adder class 의 constructor.
    - sc_module_name 을 parameter로 전달한다.
    - Adder 의 상위 class 인 sc_module 에 Adder 의 sc_module_name class 를 전달한다.

SC_THREAD(main) :
    - main 이라고 하는 thread 를 등록하는 함수.
    - thread 는 병렬로 수행되는 최소의 process 단위.
    - Verilog 의 always 구문에 대응

sensitive << a << b :
    - main 이라고 하는 thread 가 언제 SystemC Kernel 에 의해 수행되는지 알려주는 sensitive list
    - a 또는 b 의 값이 변결될 때마다 main 이 수행
    - Verilog 에서의 @() 구문에 대응

wait()
    - SystemC Kernel Scheduler 에게 수행 제어권을 넘김.
    - 선점형 OS 방식으로, main thread 가 수행되면 다른 어떤 thread 도 수행을 못 함.
    - 따라서 wait() 을 call 하여 제어권을 kernel에게 넘김.