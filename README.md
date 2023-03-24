# matrix-03-duggal-malinsky-ke

## **Distributed parallel computing**
1. Explain the architecture of your solution.

> The solution uses a version of the MPI vector multiplcation, to split the matrix up into vectors and then it sends them out to the wolfgand cluster and uses mmult_omp to multiply the vectors by the second matrix and then return the products in a single return vector which then places it into a resulting matrix. Overall
the solution only leans on 3 files, mmult (which is mostly unnecessary but there for error checking), mmult_omp which does the actual math of multiplying, & mmult_mpi_omp which handles the MPI broadcasting of the vectors and recieving of answers. They are all compiled together in the Makefile and tested with run.sh a shell script that calls main.c which runs SIMD multiplication, OMP multiplication, and Unoptimized multiplication to generate times for comparison. 

2. Explain the variations of algorithm you implemented.
> OMP:The basic algorithm uses three nested loops to iterate over each element of the resulting matrix and calculate its value based on the corresponding elements of the input matrices.The OMP parallelism is used to distribute the workload across multiple threads for improved performance. The #pragma omp parallel directive is used to create a parallel region, and the default(none) and shared clauses are used to specify the scope of the shared and private variables.
 
> SIMD: Takes two complex matrices and performs the same operation on multiple streams of data. within the loop a[i][k] factor is constant and the vectorized data is stored in blocks so all the number values can be loaded in at once. This algorithm was ran with and without the -O3 flag so we can see how the data changed when it was vectorized or not.
## **Teamwork**
1. Ziyi ke - Generated the graphs, wrote the OpenMP algorithm, wrote the automation in python.

2. Domenic K Malinsky - Wrote the SIMD algorithm and main file, altered the Matrix multiplication with OpenMP and MPI with assistance from rishi. 

3. Rishi Duggal - Implemented the algorithms into the existing code and ran through the compiling process.

Did you lock the master branch as explain in GitHW2 Lab 2 - Git? How did you proceed to review each other work?
  > We did lock the master branch before starting any code, when it came to reviewing each other's work it was fairly straightforward all of us met up when we     worked on the assignment so when we were writing the code everything was bounced off each other so we didn't have to review when merging with the main         branch since it was all one joint effort.

## **Full project life cycle**
1. Have you used a project board? How did you used it, or if you did not use a project board how did you plan and manage your project and team work?
> We created a project board at the start of the assignment just so we can have one location to view everything we needed to get done. Everytime we finished a task we made sure to mark it done so we knew what we needed to finish. We worked on each task as a team for the most part with some exceptions to the other task that only one of us worked on while the other two coded.
2. Is the usual cycle: write code, compile code,  run code, test code was the same when doing remote development on Wolfgand cluster. Did you need to adapt your way of working or use different tools?
> While the general cycle of writing, compiling, running, and testing code remains the same for remote development on Wolfgand cluster, there may be some adjustments needed in terms of tools and processes. For example, when working on a remote cluster, there may be additional steps involved in connecting to the cluster, and transferring files to and from the cluster.Additionally, depending on the specific tools and technologies used on the cluster, there may be additional steps involved in building and running the code. For example, if using MPI or OMP, the code may need to be compiled with specific compiler flags or run with specific command-line options to take advantage of parallel processing capabilities.

3. What kind of testing did you plan to use for this project? Did you consider measuring speed, memory consumption and validity of results for matrix multiplication. Did you consider that the code provided by the professor could have flaws?

> We tested our code by checking elapsed time, to see the speed at which the matrix multiplication was done. This, in essence was the main measurement for the lab as our matricies were dynamically allocated, and comparing times on a graph ended up making more sense from a visuals perspective. Of course the other part of testing was using the given compare matrices function to validate multiplication results, and to ensure that the math that was taking place was, in fact, legitimate and correct. Then we recorded the elapsed time.

4. Did you need to write code or use tools to generate random matrix of specific sizes? Did you put this in your plan? 
> We wrote a shell script to automate the process of running the program, and ran it 500 times increasing the matrix size by 1 each run. TO make the matrices, we used the generate matrix function, making generic runner programs that would call the algorithms with a generated matrix of that specified size. 

5. Did you put in your plan the work needed to generate tables or graphs? Did you automate this work?  
> We did automate the process of generating the graphs and tables needed to show our data. We did plan the time for graphing and comparing times as well.

6. What proportion of the tasks (and time) in your plan is  about writing variations on the matrix multiplication algorithm and what proportion is about testing and reporting activities?
> Generally most of our time and tasks was spent on creating variations of the algorithm, once we came up with an algorithm, testing took up almost all the remainder, with reporting work generally being split with 1 member working on it while the other two coded, tested, or were troubleshooting. 

## **Graph**
![Figure_1](https://user-images.githubusercontent.com/111989865/227429475-1f301d77-f99d-4c07-a2d6-4e61feb87563.png)
