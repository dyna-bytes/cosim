SystemC 의 모듈은 class 이다. 모듈 간 통신은 class 간 통신과 같다.
그런데 class 라는 것은 sw component 이기 때문에 verilog와 같은 HDL 에서처럼 hardware 간의 통신을 위한 class 들이 따로 필요한 것이다.
module 과 module 은 channel 을 통해 연결된다. 그리고 이 channel 의 변수를 read/write 함으로써 data를 주고 받는다.
(class 와 class 를 연결하는 것 : channel. 이 channel 역시 class 이다.)


이를 위해 SystemC에서는 interface-method-call (IMC) 라는 방식을 사용한다.
즉, port에 연결된 interface의 method function 을 call 함으로써 channel의 값을 read/write 하는 방식이다. interface class에는 channel class의 데이터를 조작할 수 있는 가상 함수를 선언한다.
(어렵게 써놨지만 그냥 인터페이스용 함수 뚫는다는 뜻인듯)


실제 이 함수의 구현은 channel class 내부에서 이루어진다. 모든 interface class 는 sc_interface 로부터 파생되어야 한다. sc_interface class 에는 register_port라고 하는 가상 함수를 제공하고, channel class에서는 이 가상함수를 통해 port 와의 mapping 에 대한 적합성을 검증한다.
(sc_interface 상속해서 class 만들라는 뜻인듯.)


sc_module 에서 다른 모듈과의 통신을 위해서 input/output port class 를 정의해 주어야 한다. port class는 통신의 매개체로 이용되는 channel class의 멤버 함수를 호출하기 위하여 interface를 binding시켜주는 역할을 한다.
(
    class module A {
        class input port PA;
    };
    class module B {
        class output port PB;
    };
    class interface IF {
        class sc_interface parent;
    };

    IF.bind(A->PA, B->PB);

    뭐 이런 느낌?
)


channel class는 앞서 말한 대로 interface 의 가상 함수를 실제로 구현하여 channel class 내부의 데이터를 조작할 수 있도록 한다.

하나 예를 들어 보자.

```
class sender::public sc_module {
    sc_out<int> Y; // 포트 선언
};

class receiever::public sc_module {
    sc_in<int> A; // 포트 선언
};

sender u_sender("sender"); // 모듈 인스턴스
receiver u_receiver("receiver"); // 모듈 인스턴스

sc_signal<int> data; // 채널 선언

u_sender.Y(data); // 포트 매핑
u_receiver.A(data); // 포트 매핑
```
sender module 의 output port Y 와 receiver module 의 input port A 는 sc_signal 이라고  하는 channel class 'data' 로 연결된다.

sc_signal class 내부를 들여다보면 sc_signal_inout_if class 의 가상함수 read()/write() 가 구현되어 있다. sender module 에서는 Y.write(10); 과 같은 방법으로 Y port 에 데이터를 쓸 수 있다.
또한 receiver module 에서는 A.read(); 와 같은 방법으로 A port 의 데이터를 읽을 수 있다.
Y.write(10); 이라는 구문의 실행 절차를 분석해보면 Y class 의 write() 멤버함수가 실행되고 이 멤버 함수는 포트 매핑을 통해 연결된 sc_signal channel class 의 interface class 포인터를 통해 sg_signal channel class 내부에 구현된 write() 함수를 호출한다.


여러가지 다양한 interface, port, channel 을 사용자가 구현 할 수 있다. sc_signal 과 같은 channel 은 SystemC 의 primitive channel 이라고 부른다.

앞 장에서 예를 든 Adder module을 확장하여 Multiplier module을 만들고 서로 연결하는 code를 작성해보자.

위에서 설명한 것처럼 sc_signal 을 통해 서로 연결하여 데이터를 전달할 수 있음을 알 수 있다.