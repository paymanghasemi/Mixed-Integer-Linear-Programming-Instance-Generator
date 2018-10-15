# Multi-objective-Mixed-Integer-Linear-Programming-Instances
Set of solved instances using BOMILP-MMP.cpp
# Instances
We generated instances for three values of p ∈ { 2, 3, 4 } , which refers to
the number of objective functions in a problem. In each case, we have four sets of instances, namely,
pure integer instances n i = n, pure binary instances n b = n, mixed integer instances n i = 0.5n and
n c = 0.5n, and mixed binary instance n b = 0.5n and n c = 0.5n. The problems are as follows,

![Images](Images/problem.jpg)

Each set of instances contains 16 subclasses of instances based on the dimensions of the matrix A, where A is an M * N matrix, and each subclass contains 10 instances. To be specific, we assumed that m is in {200, 400, 800, 1600} and n=a*m where a is in {0.5, 1, 1.5, 2}. The sparsity of matrix A is 50%. The components of vector b and the entries of matrix A are randomly drawn from discrete uniform distributions [50, 150] and [1, 30], respectively. We set the vector d equal to zero. The sparsity of each row of the matrix D was also set to 50% and its components were drawn randomly from a discrete uniform distribution [1, 10].
It should be noted that, since all constraints are inequality and all coefficients of matrix A are nonnegative, the feasible regions are bounded.

We also note that, in cases with integer variables, we provided the converted instances. This is due to the fact that, in order to implement our algorithm, the integer variables must be converted to binary variables.

# instance.LP

The instances are written as CPLEX LP file as follows,

![Images](Images/instance.jpg)

where the first p x's are the objective functions respresenting the y1, y2,..., and yp of the problem, and the rest of x are the decision variables.

Real, binary, and integer decision variables are distributed randomly in the instances, and therefore, there is no pattern for separating their indexes.

# Compiling and Running

To compile the code, CPLEX (default version 12.7) must be installed on your computer. The default directory for CPLEX used is /opt/CPLEX/12.7/. Changing the directory of CPLEX to your preferred directory can be done either in the Makefile or through Netbeans. If you would like to do it in the Makefile you should go to nbproject/Makefile-Debug.mk and nbproject/Makefile-Release.mk and change all instances of /opt/CPLEX/12.7/ to your preferred directory. If you would like to do it through Netbeans, you can open the project in Netbeans and right click on the name of the project and choose Properties. You can then change the directory in the Include Directories box which is located in the C++ Compiler sub-menu. Moreover, you should also change the directory in the Libraries box which is located in the Linker sub-menu.

Compiling the project can be done using the Terminal by going to the directory in which the project is saved and typing ”make clean” followed by ”make” (you can also compile through Netbeans).

An instance can be solved by typing 

./Instance_Generator <*number of objective*> <*number of decision variables*> <*number of constraints*> <*sparsity of matrix A*> <*sparsity of objective's matrix D*> <*percent of continuous variables*> <*percent of integer variables*>


For better understanging, an example code to run is as follows

make clean

make

./Instance_Generator 2 4 5 0.5 0.5 25 25

This code generates an instance with two objectives, four decision variables, five constraints, 0.5 the sparsity of matrix A, 0.5 sparisty of matrix D, 25 percent of the decision variables are continuous, and 25 percent of variables are integers. and since the sumation of integers and continuous is not 100, the remaining 50 percent of the variables are binaries.

The code generates three files: Original.lp is the original instance, i.e. the integer variables are not converted. 1.lp is the binary form of the instance, and 2.lp is the relaxed form of the 1.lp.

# Supporting and Citing

The instances were generated as part of academic research. If you would like to help support it, please star the repository as such metrics may help us secure funding in the future. We would be grateful if you could cite:

[Ghasemi Saghand, P., Charkhgard, H., A criterion Space Search Algorithm for Mixed Integer Linear Maximum Multiplicative Programs: A Multi-objective Optimization Approach]
