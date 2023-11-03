# CDMA-Program
CDMA(Code Division Multiplexing)을 구현한 프로그램
### CDMA란?


### 문제
CDMA를 구현하기 위한 프로그램을 작성하라. 칩 시퀀스의 길이는 8이라고 가정하고 전송하는 단말은 4대라고 가정한다.
프로그램은 4개의 전송 프로세스(t0, t1, t2, t3), 하나의 Joiner 프로세스, 그리고 4개의 수신 프로세스(r0, r1, r2, r3)를 포함한다.
메인 프로그램은 joiner 프로그램 역할도 하는데 먼저 표준 입력으로부터 4개의 바이폴라 형태로 된 칩 시퀀스를 읽어 들이고 네 쌍의 송수신 프로세스를 분기시킨다.
각 송수신기 프로세스(t0,r0; t1,r1; t2,r2; t3,r4)는 칩 시퀀스 하나씩을 할당받고 각 전송 프로세스는 1bit씩 할당받는다.
(첫 번째 비트는 t0, 두 번째 비트는 t1 등.)
각 송신기는 전송해야 할 신호를 계산하고(8bit의 시퀀스) 이것을 joiner 프로세스에 전달한다. 4개의 송신기로부터 신호를 받은 다음 joiner 프로세스는 신호들을 결합해 결합된 신호의 모든 수신 프로세스로 보낸다.
각 수신 프로세스는 수신된 비트를 계산하여 표준 출력으로 프린트한다. 프로세스 간 통신에는 파이프를 사용하라.

Write a program to implement CDMA. Assume that the length of a chip sequence is eight and the number of stations transmitting is four. Your program consists of three sets of processes: four transmitter processes (t0, t1, t2, and t3), one joiner process, and four receiver processes (r0, r1, r2, and r3). The main program, which also acts as the joiner process first reads four chip sequences (bipolar notation) from the standard input and a sequence of 4 bits (1 bit per transmitter process to be transmitted), and forks off four pairs of transmitter and receiver processes. Each pair of transmitter/receiver processes (t0,r0; t1,r1; t2,r2; t3,r3) is assigned one chip sequence and each transmitter process is assigned 1 bit (first bit to t0, second bit to t1, and so on). Next, each transmitter process computes the signal to be transmitted (a sequence of 8 bits) and sends it to the joiner process. After receiving signals from all four transmitter processes, the joiner process combines the signals and sends the combined signal to the four receiver processes. Each receiver process then computes the bit it has received and prints it to standard output. Use pipes for communication between processes.



