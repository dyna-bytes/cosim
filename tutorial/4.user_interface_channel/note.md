앞선 장에서 모듈간 통신을 위해 interface/port/channel 이 필요하다는 것을 배웠다. SystemC 에서 제공하는 in/output port 와 primitive channel 을 이용하여 하드웨어 통신을 모델링할 수 있었다. 그러나 SystemC 를 제대로 활용하기 위해서는 이것으로 부족하다. 기존의 RTL Level 모델링은 Verilog/VHDL 과 같은 HDL 이 더 편리할 것이다.


진정 SystemC 가 필요한 부분은 조금 더 high level 의 모델링과 검증 분야이다. 나중에 이야기할 기회가 있겠지만 SCV (SystemC Verification Standard)와 같은 library 로 검증 분야에서 SystemC 는 훌륭한 기능을 발휘할 수 있게 되었다. 그러면 high level modeling 이나 검증을 위해 가장 많이 사용하는 사용자 정의 인터페이스/채널에 대해 알아보자. 모듈간의 통신을 신호 레벨이 아닌 조금 더 상위 수준 즉, Transaction Level 에서 모델링 하기 위해서는 사용자가 전송을 데이터 타입을 정의하고 그 데이터를 읽고 쓰는 역할을 하는 채널을 만들어야 한다.


이 때 읽고/쓰는 함수를 실제 구현하는 것이 채널이고 그 가상함수를 정의하는 것이 인터페이스이다. 그리고 이 인터페이스를 모듈에서 사용할 수 있도록 연결하는 것이 포트이다.

예제를 하나 들어 보자. 우선 어드레스와 데이터로 구성된 데이터 타입을 정의하자.

```
typedef struct {
    int addr;
    int data;
} packet_t;
```
이 데이터 타입을 인자로 갖는 read()/write() 가상 함수를 정의하는 인터페이스는 아래와 같다.

```
class my_packet_if : public class sc_interface {
public:
    virtual void read(packet_t *rd_pkt) = 0;
    virtual void write(packet_t *wr_pkt) = 0;
};
```

그리고 이 인터페이스를 구현하는 채널은 아래와 같이 구현할 수 있다.

사용자 정의 채널은 sc_channel 클래스와 사용자 정의 인터페이스, 여기서는 my_packet_if 를 상속 받는다.

```
class my_channel : public class sc_channel, public class my_packet_if {
public:
    packet_t my_data;

    virtual void read(packet_t *rd_pkt) {
        rd_pkt->addr = my_data.addr;
        rd_pkt->data = my_data.data;
        return;
    }

    virtual void write(packet_t *wr_pkt) {
        my_data.addr = wr_pkt->addr;
        my_data.data = wr_pkt->data;
        return;
    }
};
```

그리고 이 인터페이스를 인자로 갖는 포트는 아래와 같이 선언할 수 있다.
```
sc_port<my_packet_if<packet_t>> my_port;
```

그러면 모듈 A와 B에 위의 port 가 선언되어 있다고 하면 두 모듈은 아래와 같이 연결 할 수 있다.

```
my_channel u_my_chn;

A.my_port(u_my_chn);
B.my_port(u_my_chn);
```

A 모듈에서 packet 을 write 하려면 어떻게 할까? 아래에 예를 들어 본다
```
packet_t my_pkt;

my_pkt.addr = 0x1234;
my_pkt.data = 0xff;

my_port->write(&my_pkt);
```

그리고 B 모듈에서 packet 을 read 하려면 다음과 같을 것이다.
```
packet_t my_pkt;

my_port->read(&my_pkt);

cout << "Packet Address = " << my_pkt.addr << endl;    cout << "Packet Data = " << my_pkt.data << endl;
```

위와 같이 사용자가 잘 정의한 인터페이스로 상세 pin to pin 인터페이스 대신 추상화된 데이터 타입의 인터페이스를 사용함으로써 보다 상위 레벨의 전송 모델을 구현할 수 있는 것이다.]