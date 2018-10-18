/* 
 * File:   main.cpp
 * Author: c3156840
 *
 * Created on 23 July 2013, 11:28 AM
 */

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include<fstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ilcplex/ilocplex.h>


using namespace std;
#define b_min 50 
#define b_max 150
#define A_min 10 
#define A_max 30
#define z_min 1
#define z_max 10
#define N_Infinity -100000000
#define div 2


/*------------------------------------------------------------------------------
 
 -----------------------------Public Variables----------------------------------
 
 -----------------------------------------------------------------------------*/
int N_Objectives;
double Sparsity_Matrix;
double Sparsity_ObjFs;
int** ObjF;
int Row_N;
int Column_N;
int** Matrix;
int* RHS;
int Number_of_NonZeros;
int* Number_of_NonZeros_In_Each_Row;
int Counter;
int save;
bool done = 0;

int FLOAT;
int INT;
int BOOL;
int remain;
int setter;
bool assigned;
int Int_bin_counter = 0;
int PFL;
int PINT;
/*------------------------------------------------------------------------------
 
 ------------------------Declaring CPLEX information----------------------------
 
 -----------------------------------------------------------------------------*/
ILOSTLBEGIN
typedef IloArray<IloNumVarArray> NumVar2DArray;
IloEnv env;
IloModel model(env);
IloModel model_myself(env);
IloModel model_SOCP(env);
IloObjective cost;
IloNumVarArray x_con(env);
IloNumVarArray x_UR_con(env);
IloNumVarArray x_con_SOCP(env);
IloNumVarArray x_UR_con_SOCP(env);
IloExpr Expr(env);
IloCplex cplex(env);
IloRangeArray Int_to_binary(env);
IloExprArray Int_bin_holder(env);

int main(int argc, char *argv[]) {
    N_Objectives = atoi(argv[1]);
    Column_N = atoi(argv[2]);
    Row_N = atoi(argv[3]);
    Sparsity_Matrix = atof(argv[4]);
    Sparsity_ObjFs = atof(argv[5]);
    PFL= atoi(argv[6]);
    PINT = atoi(argv[7]);
    srand(time(NULL) + N_Objectives + Row_N + Column_N + atoi(argv[8]));


    //------------------------------------------------------------------------------
    ObjF = new int* [N_Objectives];
    for (int i = 0; i < N_Objectives; i++) {
        ObjF[i] = new int [Column_N];
    }
    for (int i = 0; i < N_Objectives; i++) {
        for (int j = 0; j < Column_N; j++) {
            ObjF[i][j] = 0;
        }
    }



    Matrix = new int* [Row_N];
    for (int i = 0; i < Row_N; i++) {
        Matrix[i] = new int [Column_N];
    }
    for (int i = 0; i < Row_N; i++) {
        for (int j = 0; j < Column_N; j++) {
            Matrix[i][j] = N_Infinity;
        }
    }
    RHS = new int [Row_N];
    /*------------------------------------------------------------------------------
 
     ------------------------------Generating Matrix--------------------------------
 
     -----------------------------------------------------------------------------*/
    Number_of_NonZeros = Row_N * Column_N*Sparsity_Matrix;
    Number_of_NonZeros_In_Each_Row = new int [Row_N];
    for (int i = 0; i < Row_N; i++) {
        Number_of_NonZeros_In_Each_Row[i] = 1;
    }
    Number_of_NonZeros -= Row_N;
    while (Number_of_NonZeros > 0) {
        for (int i = 0; i < Row_N; i++) {
            if (Number_of_NonZeros == 0) {
                break;
            }
            if (Column_N - Number_of_NonZeros_In_Each_Row[i] > 0) {
                Counter = (fmin(Number_of_NonZeros, Column_N - Number_of_NonZeros_In_Each_Row[i]) / div) + 1;
                save = (rand() % Counter + 1);
                Number_of_NonZeros_In_Each_Row[i] += save;
                Number_of_NonZeros -= save;
            }
        }
    }
    //------------Ensuring That Each Row and Each Variable are useful----------- 
    Counter = 0;
    for (int i = 0; i < Row_N; i++) {
        Matrix[i][Counter] = (rand() % (A_max - A_min + 1)) + A_min;
        Counter++;
        if (Counter == Column_N) {
            Counter = 0;
        }
        Number_of_NonZeros_In_Each_Row[i]--;
    }
    if (Row_N < Column_N) {
        while (Counter < Column_N) {
            for (int i = 0; i < Row_N; i++) {
                if (Counter >= Column_N) {
                    break;
                }
                if (Number_of_NonZeros_In_Each_Row[i] > 0) {
                    Matrix[i][Counter] = (rand() % (A_max - A_min + 1)) + A_min;
                    Counter++;
                    Number_of_NonZeros_In_Each_Row[i]--;
                }
            }

        }
    }
    //------------------Assigning Values to All Other Elements------------------ 
    for (int i = 0; i < Row_N; i++) {
        while (Number_of_NonZeros_In_Each_Row[i] > 0) {
            save = (rand() % Column_N);
            done = 0;
            while (done == 0) {
                if (save == Column_N) {
                    save = 0;
                }
                if (Matrix[i][save] < N_Infinity + 1) {
                    Matrix[i][save] = (rand() % (A_max - A_min + 1)) + A_min;
                    done = 1;
                } else {
                    save++;
                }
            }
            Number_of_NonZeros_In_Each_Row[i]--;
        }
    }
    //----------------------change Negative_Infinity to Zero--------------------
    for (int i = 0; i < Row_N; i++) {
        for (int j = 0; j < Column_N; j++) {
            if (Matrix[i][j] < N_Infinity + 1) {
                Matrix[i][j] = 0;
            }
        }
    }
    /*------------------------------------------------------------------------------
 
     --------------------------Generating Objectives--------------------------------
 
     -----------------------------------------------------------------------------*/
    for (int i = 0; i < N_Objectives; i++) {
        Number_of_NonZeros = fmax(1, Column_N * Sparsity_ObjFs);
        while (Number_of_NonZeros > 0) {
            save = (rand() % Column_N);
            done = 0;
            while (done == 0) {
                if (save == Column_N) {
                    save = 0;
                }
                if (ObjF[i][save] == 0) {
                    ObjF[i][save] = (rand() % (z_max - z_min + 1)) + z_min;
                    done = 1;
                } else {
                    save++;
                }
            }
            Number_of_NonZeros--;
        }
    }

    /*------------------------------------------------------------------------------
 
     -----------------------Generating right hand side------------------------------
 
     -----------------------------------------------------------------------------*/
    for (int i = 0; i < Row_N; i++) {
        RHS[i] = (rand() % (b_max - b_min + 1)) + b_min;
    }


    /*--------------------------------------------------------------------------
 
     ------------------------Generating Model---------------------------
 
   ---------------------------------------------------------------------------*/

    x_con_SOCP = IloNumVarArray(env, Column_N);
    remain = Column_N;
    FLOAT = ceil(Column_N * PFL / 100.0);
    remain -= FLOAT;
    INT = floor(Column_N * PINT / 100.0);
    remain -= INT;
    BOOL = remain;
    if(INT>0){
    x_con = IloNumVarArray(env, 4 * INT, 0.0, +IloInfinity, ILOBOOL);}
    for (int i = 0; i < Column_N; i++) {
        assigned = 0;
        while (assigned == 0) {
            setter = (rand() % 99 + 1);
            if (setter < 34 && FLOAT > 0) {
                x_con_SOCP[i] = IloNumVar(env, 0.0, +IloInfinity, ILOFLOAT);
                FLOAT--;
                assigned = 1;
            } else if (setter >= 34 && setter < 67 && INT > 0) {
                x_con_SOCP[i] = IloNumVar(env, 0.0, +IloInfinity, ILOINT);
                INT--;
                assigned = 1;

            } else if (setter >= 67 && BOOL > 0) {
                x_con_SOCP[i] = IloNumVar(env, 0.0, +IloInfinity, ILOBOOL);
                BOOL--;
                assigned = 1;
            }
        }
    }
    x_UR_con_SOCP = IloNumVarArray(env, N_Objectives, -IloInfinity, +IloInfinity, ILOFLOAT);
    for (int i = 0; i < N_Objectives; i++) {
        model_myself.add(x_UR_con_SOCP[i] == 0);
        model_SOCP.add(x_UR_con_SOCP[i] == 0);
    }

    for (int i = 0; i < N_Objectives; i++) {
        Expr.end();
        Expr = IloExpr(env);
        Expr += x_UR_con_SOCP[i];
        Int_bin_counter = 0;

        for (int j = 0; j < Column_N; j++) {
            if (x_con_SOCP[j].getType() != 1) {
                Expr -= (ObjF[i][j] * x_con_SOCP[j]);
            } else {
                Expr -= (ObjF[i][j] * (x_con[Int_bin_counter] + 2 * x_con[Int_bin_counter + 1] + 4 * x_con[Int_bin_counter + 2] + 8 * x_con[Int_bin_counter + 3]));
                Int_bin_counter += 4;
            }
        }
        model_myself.add(Expr == 0);
        Int_bin_counter = 0;
        Expr.end();
        Expr = IloExpr(env);
        Expr += x_UR_con_SOCP[i];
        for (int j = 0; j < Column_N; j++) {

            Expr -= (ObjF[i][j] * x_con_SOCP[j]);

        }
        model_SOCP.add(Expr == 0);
    }
    Int_bin_counter = 0;
    for (int i = 0; i < Row_N; i++) {
        Expr.end();
            Expr = IloExpr(env);
            for (int j = 0; j < Column_N; j++) {
                if (x_con_SOCP[j].getType() != 1) {
                    Expr += (Matrix[i][j] * x_con_SOCP[j]);
                } else {
                    Expr += (Matrix[i][j] * (x_con[Int_bin_counter] + 2 * x_con[Int_bin_counter + 1] + 4 * x_con[Int_bin_counter + 2] + 8 * x_con[Int_bin_counter + 3]));
                    Int_bin_counter += 4;
                }
            }
            model_myself.add(Expr <= RHS[i]);
        Int_bin_counter = 0;
        Expr.end();
            Expr = IloExpr(env);
            for (int j = 0; j < Column_N; j++) {
                Expr += (Matrix[i][j] * x_con_SOCP[j]);
            }
            model_SOCP.add(Expr <= RHS[i]);
    }
    for (int j = 0; j < Column_N; j++) {
            if (x_con_SOCP[j].getType() == 1) {
                model_myself.add(((x_con[Int_bin_counter] + 2 * x_con[Int_bin_counter + 1] + 4 * x_con[Int_bin_counter + 2] + 8 * x_con[Int_bin_counter + 3])) <= 15) ;
                Int_bin_counter += 4;
            }
        }
    cost = IloMaximize(env, 0);
    model_myself.add(cost);
    cplex.extract(model_myself);
    cplex.exportModel("1.lp");
    cplex.clear();
    model_myself.add(IloConversion(env, x_con, ILOFLOAT));
    if (PINT == 0) {
        model_myself.add(IloConversion(env, x_con_SOCP, ILOFLOAT));
    }
    cplex.extract(model_myself);
    cplex.exportModel("2.lp");
    cplex.clear();
    model_SOCP.add(cost);
    cplex.extract(model_SOCP);
    cplex.exportModel("Original.lp");
    cplex.clear();

    return 0;
}

