# MPI Parallel Words Count

## Problem statement

We will be doing a version of map-reduce using MPI to perform word counting over a large number of files. There are 3 steps to this process:

1. Is to read in the master file list which will contain the names of 
   all the files that are to be counted. Note that only 1 of your processes should read this file. ( **Problem improvement**  : The master sends two integers, start and chunk size, the slave calculates the files to read based on these two integers. The improvements are on the network communication because the master doesn't send a string list, but only two integers. ) Then each of the processes should receive their portion of the file from the master process. Once a process has received its list of files to process, it should then read in each of the files and perform a word counting, keeping track of the frequency each word found in the files occurs. We will call the histogram produced the local histogram. This is similar to the map stage or map-reduce.
2. Is to then combine frequencies of words across processes. For example
   the word ‘cat’ might be counted in multiple processes and we need to add up all these occurrences. This is similar to the reduce stage of map-reduce. 
3. Is to have each of the processes send their local histograms to the 
   master process. The master process just needs to gather up all this information. Note that there will be duplicate words between processes. 
   The master should then print out the results to the screen.

## Input files

This project has been realized to read and analyze a words count on Plain Text Files (byte reading)

Example:

> ```
> Is not the Republic the vehicle of three or four great truths which,
> to Plato's own mind, are most naturally represented in the form of the
> State? Just as in the Jewish prophets the reign of Messiah, or 'the day
> of the Lord,' or the suffering Servant or people of God, or the 'Sun of
> righteousness with healing in his wings' only convey, to us at least,
> their great spiritual ideals, so through the Greek State Plato reveals
> to us his own thoughts about divine perfection, which is the idea of
> good--like the sun in the visible world; about human perfection, which
> is justice--about education beginning in youth and continuing in later
> years
> ```



## Output

The output into the folder "results", report all the word analyzed by processors involved in the computation and the time consumed by each of them. Only for the processor with rank 0 (the master), there are into the file all the words/ occurrences combined and the whole time computation.

An example file with rank 0 :

```
START REPORT FOR PROCESS WITH RANK 0: 
Word:  MUNNUT --- Value:  1  
Word:  MEDIATRIX --- Value:  1  
Word:  SLATTERNLY --- Value:  2 
END REPORT.
 TOTAL EXECUTION TIME: 1.220264 seconds 
 TOTAL WORDS ANALYZED: 4  

```

## Algorithm description

1. The main goal is to have a perfect division of the workload for each worker. To reach that each worker computes a balanced number of bytes,  namely, the master elaborates the sum of all the file  byte sizes (S), considering N processes, each worker analyze :

   ​                                                                        **chunk_size = S / N**

some workers have to compute also the **remainder of the  division** (**chunk_size + 1**), but the cost is marginal. 

2. The master has also to send to each worker the position where **each process must start to read the file**, because they must not read the words (bytes) get analyzed by the previous worker. Suppose that you want to know where the processor "i" has to start to read into the file and "j" are all the previous worker:

   ![start formula](http://ferrara.link/img/wordscount2019/workCalculation.png)

3. Each worker read all the file into the **"directory sample"** than starting from the first, it deletes all the files where the sum of the bytes until the current reading is **less than "start"**. After that the previous condition is no longer satisfied, it inserts in a linked list the files until the sum of these is **less than "chunk_size"** , the files that appear after the chunk_size condition are removed. Those are the files that the worker must analyze.

4. Each worker analyzes the own files and inserts in a hashmap the alphanumeric words that occur. There is a problem to manage if a worker is currently reading a word, but the byte **i = chunk_size** comes before analyzing this, it finishes to analyze it, and the next worker ignores this word and starts to read from the next word.

5. Each slave sends to the master that combines the analyzed words.



## Implementation details

Below the UML diagram :

![words count uml diagram](http://ferrara.link/img/wordscount2019/words-count-uml-diagram.jpg)

Let's describe its in details:

- fileparser: it offers the functions related to the operations on a file : 
  - list_of_file_path(),  reads all the files into the folder "sample" and return a linked list with filenames.
  - get_total_file_size(), returns the sum of all the files sizes (bytes).
- hashmap: it implements a hash map structure with the related functionality :
  - insert_or_increments(), inserts the read word if is not present, or increments if present.
  - report_hash_elements(), reports all the words into the hash map in a file into the folder "results", with the name of the process that analyzed them.
  - free_hashmap(): de-allocate all the memory related to the hash map structure.
- workerhelper: it manages all the files into the package, assigns the right number of the file to read and the right position for start the reading. It also resolves all the issues related to the reading from files in a parallel context. It allows communications between the main and the lower level (file parser and hashmap).
  - calculate word frequencies(): calculates the words that are in a file and the occurrences.
- mpihelper: if the processor is the master, it waits for the words and the occurrences from the slaves ("master_receiver()").
  If the processors are the slaves, it puts all the words and the occurrences in a structure where the size is chosen by the developer (through "#define WORDONTHENETWORK") and sends it to the master ("worker_sender()").

> Is not obvious that MPI_Send will send the specified size of the structure because the using of MPI_Send  allows the MPI implementation the maximum flexibility in choosing how to deliver your data. Is just because if you have a small number of bytes, is better don't stress the network sending a big structure

- main: the master sends the portion of bytes to read after computes its workload communicating with "workerhelper", finally it waiting for the results from the slaves(communicating with "mpihelper") for combine them. The slaves after that have been received the portion to analyze, they compute(trough "workerhelper") and send to the master, the words/occurrences through a defined struct (using "mpihelping").

### MPI Features

In this project has been used the following MPI features:

- MPI_Scatter ;
- MPI Derived Data Types (for the struct to send to the master);
- MPI_Send ;
- MPI_Ssend;



# Measuring Parallel Scaling Performance

The tests were performed on Amazon AWS EC2 cluster, with 5 instances (t2.xlarge).

Each instance has 4 vCPUs, so the tests were performed by increasing the number of processors by one at a time, and by adding other instances gradually (starting from 1 instance with 1 processor, until 5 instances with 20 processors).

## Weak scalability

Linear scaling is achieved if the run time stays constant while the 
workload is increased in direct proportion to the number of processors. In the case of weak scaling, linear scaling is achieved if the run time stays constant while the workload is increased in direct proportion
to the number of processors ( e.g.1 processor 1158762 bytes,  2 processors, 1158762 x 2 bytes) . 

![plot of weak scalability](http://ferrara.link/img/wordscount2019/weak-scalability-plot.jpg)



The chart shows us a stable trend when the sizes of the inputs are high, it is not theoretically perfect (the trend should be constant), because I have to consider also the network, send on it is more expensive than communicate through processors on the same machine, and there is also other overhead to communicate through MPI and for coordinate the work. But for better understand how much is the efficiency of my algorithm, I introduce a new measure:
**Weak scaling efficiency** that is the  amount of time to complete a work unit with 1 processing element (t1), and the amount of time to complete N of the same work units with N processing elements (tN), the weak scaling efficiency (as a percentage of linear) is given as: ( t1 / tN ) * 100%.
Below the cart representing this efficiency.

![weak efficiency bar plot](http://ferrara.link/img/wordscount2019/weak-efficiency.jpg)

The bar plot shows that the trend is quite stable with a high input size, but for a small input size, the efficiency is not appreciable (it is not convenient to use more processors that communicate for a small input size, because I need to consider also various costs, like send on the network, MPI manager, coordinator etc.).
In summary, the execution times seem not to change too much, there is a maximum efficiency of 98% and the minimum is around 45%, does it means that the algorithm should address larger problems in a reasonable amount of time by using more resources.

## Strong scalability

In that case the problem size stays fixed but the number of processing elements are increased. The goal is to show that the program does not waste too many cycles due to parallel overhead. 

![strong scalability  plot](http://ferrara.link/img/wordscount2019/strong-scalability-plot.jpg)



As shown in the graph, using 20 processors and the input sizes listed above, the communication overhead doesn't impact the performance. Is important to underline that the performances do not really improve when are used more than 15 processors, due to communication overhead. This meaning that for tests where the input is similar to our sizes, is useless to use more than 15 processors, but this is only a case related to our inputs sizes in an empirical way, with a bigger size input, adopt more than 15 processors will help to raise  the performances, but also in this case will there a point where the performances do not improve too much  compared to the processors that my system is using.

For better understanding the trade-off between processors used and performances obtained we introduce the "strong scaling efficiency" concept:
Strong scaling efficiency is the  amount of time to complete a work unit with 1 processing element ( t1 ), and the amount of time to complete the same unit of work with N processing elements ( tN ), the **strong scaling efficiency (as a percentage of linear)** is given as:
                                                                         **t1 / ( N * tN ) * 100%**

below our bar plot representing the measure.

![bar plot strong scaling efficiency](http://ferrara.link/img/wordscount2019/strong-efficiency.jpg)

The bar plot shows that bigger is the input size, higher is the efficiency of using more machines. We are able also to observe that in some case, the efficiency is more than 100%, due the splitting of a big workload to different processors, the results get better as the input size is increased.
This is valid for higher inputs sizes, because the benefits gain from the addition of more workers is more relevant than the communication overhead, resulting then in a higher scaling efficiency. However we are able also to see, that for small sizes, the efficiency is not as expected, the cost due to overhead in communication is higher than the cost of workload splitting.
In summary, there is a max efficiency of 120 % and the minimum is around 60%, a notable result.

 

## Instructions for run

> NOTE: refers to a system with a Linux OS (in this case Ubuntu).

Go in the project folder "wordcounts" and write in terminal :

`mkdir builds`

> NOTE: If is not present, you have to create also a folder called "results"

navigate into the folder created:

`cd builds`

install "cmake" :

```
apt-get -y install cmake
```

type :

`cmake  ..`

and after :

 `make`

now you are able to run the program in the cluster or in local [(learn how create a cluster)](https://github.com/Armando1514/MPI-for-AWS-EC2) :

`mpirun -np NUMBER_OF_PROCESSORS [--hostfile machinefile]  ./wordscount`
