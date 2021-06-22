# VirtualFactory ⚙ 

Assuming a factory that has a machine and this source code manages it ,the aim is to complete all of given orders before their deadline. While doing that, our top priority is to obtain shortest overall completion time.

# Further Explanation 👇

→ Virtual machine is able to complete operations given in the "Operations.txt" (in this file you can see how much meters it can process per second for each operation type and format of definition).

→ Switching between different types of operations takes preperation time and our aim is to minimize averall preperation time (you can examine "SetupDuration.txt" file to see transition durations and format of definition).

→ "Orders.txt" file contains orders which has an order code, order length to process,operation type and deadline (you can examine this file to see format of definition).

→ Program produces a text file named "Schedule.txt" and in this file you will find a list of completed orders.Each line has passed time,operation type,order code,processed length and switching delay info (you can examine this file also to see format of definition).

You can create your own operations,setup durations,orders and run the program in the same directory to get result specifically for your machine.

# Known Issues ❗

1.Program isn't optimized for large amount of orders (>~10000).Therefore it may not give expected results in those cases.
