# Android-Scheduler

## Introduction
As we all know, the kernel is a computer program which controls over everything in the system. This picture shows the traditional UNIX system structure. 
We can see that the kernel actually is a bridge between software and hardware, whose interface is a low-level abstraction layer.

CPU scheduling is the basis of multi-programmed operating systems, whose success depends on an observed property of processes: process execution consists of a cycle of CPU execution and I/O wait. Just as the picture shows. That is because the structure of process, we need CPU scheduling to get better efficiency.
There are many scheduling algorithms for scheduler. In this project, we focus on the SCHED_NORMAL, SCHED_RR, SCHED_FIFO. 
SCHED_NORMAL is similar to SCHED_OTHER in CFS. It is not real-time or preemptive, in which the priority has not any meaning. However, it is also limited by the time slice.
SCHED_RR, round-robin (RR), is a kind of real-time and time-sharing scheduling policy. It will not take up the CPU all the time because preemption is added to enable the system to switch between processes. After one time slice, it will release the CPU for the next process in the ready queue.
SCHED_FIFO, first-in first-out (FIFO), is also a real-time scheduling policy, which means the process requesting the CPU first is allocated the CPU first. When the CPU is free, it is allocated to the process at the head of the queue. Once the CPU has been allocated to a process, that process keeps the CPU until it releases the CPU, either by terminating or by requesting I/O or by preempted by another process which has a higher priority.

## Methods
Firstly, we need to design a program which can output the executing time of itself. The completion is simple, just using one or more loops to calculate something for many times. For the compiler in AVD will optimize programs automatically, the calculation need to have a higher computational complexity.

With the help of the sys/time.h header file, we can get current time from January 1, 1970 accurately. I can get the time with the same function at the beginning and the end of this program. The interval time is exactly the executing time.

Then the next task is to change the policy and priority of specified processed. Firstly, I specify the formula of input parameters: the name of the process, to change whether itself or its descendants, priority and scheduler policy in order. And check all the parameters.

Then I can set the priority according to input parameters. I notice that the priority should not bigger than the maximum of the corresponding policy. Actually, the range of priority of the real time process is 0~99. The bigger number, the higher priority, which is different from the law of others.

To achieve the second and third goals, we need to make use of the system call 273“pstree” in project 1, which save all the information of processes in buf. It can find out the PID according to the name of a process and find out all descendants of it. We should notice that “break” should be avoided in case some unexpected errors.

The reason of finding out the PID is that the function of changing the scheduler of specified process need the PID to be one parameter. （point it）
This function sets the scheduler policy and parameters by the specified policy and parameters in sched_param structure pointed by param respectively. The value of the member sched_proirity in sched_param structure can be any integrate which is in the range of the specified processes’s policy.

Then the next task is to modify the kernel. The first goal is to modify something proper in the kernel and change the default scheduler policy and priority of all descendants of process zygote.
It is acknowledged to us that the creation or replication of all processes need to call _do_fork function defined in kernel/fork.c which is architecture-independent. It begins with copy_process, which do the actual jobs for the new process and replicate the data of the parent process.
I notice that there is a step for sched_fork. 

For each process, we go through all its ancestor. If one of the name of ancestors is “main”, then it is a descendant of process zygote. Because all processes is created by the process with PID 0, so ancestor->pid!=0 is the terminating condition. The method of change the priority is similar to that in the last section.

From the given information of const struct sched_class rt_sched_class, I trace to the definition of pick_next_rt_entity.
“rt” stands for real-time, “rq” stands for runqueue, “idx = sched_find_first_bit(array->bitmap);” is for find out the first tasks which has the highest priority at the highest speed with the help of a bitmap of priority. The following statements is to pick out the next task which is determined by idx. In order to change the method of selecting, I just take place the statement of setting the value of idx with my own codes.

Firstly, if I want to get a random number, I must include linux/random.h. Then
we should notice that the index ought to be positive and no more than the maximum of the real-time priority. In addition, it may try to pick something empty in the queue. If it happens, just get another random number and repeat.

## Results (Part)
* Test the performance of new scheduler 

|   |No Change  |Default  |Difference |Random |Difference |
|---|-----------|---------|-----------|-------|-----------|
|3  |621        |635      |-2.3%      |612    |1.4%       |
|3  |582        |471      |19%        |697    |-20%       |
|3  |417        |481      |-15%       |580    |-39%       |
|30 |4142       |3831     |7.5%       |4210   |-1.6%      |
|30  |3883       |3877     |0.15%      |5128   |-32%      |
|30  |3687       |3606     |2.2%       |4549   |-23%      |
|300 |25636      |27267    |-6.3%      |30020  |-17%      |
|300 |23434      |24088    |-2.8%      |31176  |-33%      |
|300 |26485      |24221    |8.5%       |27979  |-5.6%     |

## Conclusion
* SCHED_RR > SCHED_FIFO > SCHED_NORMAL
  * Bigger number, Higher priority

* Zygote process create mostly application processes 

* Picking randomly makes the priority no sense.

## Reference

[1] Abraham Silberschatz, Peter Baer Galvin, and Greg Gagne, Operating System Concepts, 8th Edition, John Wiley & Sons, Inc.

[2] http://blog.csdn.net/xieqibao/article/details/6581975/

[3] https://en.wikipedia.org/wiki/Kernel_(operating_system)

[4] http://www.cnblogs.com/hanyan225/archive/2011/07/12/2103545.html

[5] http://blog.sina.com.cn/s/blog_7611a30d0100pysr.html
