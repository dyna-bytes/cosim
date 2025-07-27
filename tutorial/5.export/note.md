sc_port : sc_interface class 를 매개로 하여 channel 과 연결되고 이 channel 은 또다른 sc_port 와 연결된다. 즉, 하나의 모듈에서 다른 모듈과 연결되려면 반드시 channel 을 통해야 한다.

sc_export : channel 을 통할 필요 없이 직접 다른 모듈의 sc_port 와 연결할 수 있다. 즉, channel (interface의 구현) 를 module 안으로 가져온다는 의미이다.

기존 sc_port를 이용할 때는
A[port] -> C[channel] -> B[port]

라는 형태로 구성되는 반면 sc_export 를 이용하면

A[port] -> B[export[C]]

의 형태로 연결이 가능하다.
